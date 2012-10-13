/*
 * This file is part of the KDE wacomtablet project. For copyright
 * information and license terms see the AUTHORS and COPYING files
 * in the top-level directory of this distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "debug.h"

#include "tabletdatabase.h"

#include <KDE/KConfigGroup>
#include <KDE/KSharedConfig>
#include <KDE/KStandardDirs>

namespace Wacom {
/**
  * Private class for the d-pointer.
  */
class TabletDatabasePrivate {
public:
    KSharedConfig::Ptr companyConfig; //!< companies config file
    QString            dataDirectory; //!< optional path to the data directory, used for unit tests
};
}

using namespace Wacom;

TabletDatabase::TabletDatabase() : d_ptr( new TabletDatabasePrivate )
{
    Q_D( TabletDatabase );

    d->companyConfig = KSharedConfig::openConfig( KStandardDirs::locate( "data", QLatin1String( "wacomtablet/data/companylist" ) ), KConfig::SimpleConfig, "data" );

    if( d->companyConfig->groupList().isEmpty() ) {
        kError() << "Company list missing!";
    }
}

TabletDatabase::TabletDatabase(const QString& dataDirectory, const QString& companyFile) : d_ptr (new TabletDatabasePrivate)
{
    Q_D( TabletDatabase );

    d->dataDirectory = dataDirectory;
    d->companyConfig = KSharedConfig::openConfig(QString::fromLatin1("%1/%2").arg(dataDirectory).arg(companyFile), KConfig::SimpleConfig, "data" );

    if( d->companyConfig->groupList().isEmpty() ) {
        kError() << "Company list missing!";
    }
}


TabletDatabase::~TabletDatabase()
{
    delete this->d_ptr;
}



QString TabletDatabase::lookupBackend(const QString& companyId)
{
    KConfigGroup companyGroup;

    if (!lookupCompanyGroup(companyGroup, companyId)) {
        return QString();
    }

    return companyGroup.readEntry( "driver" );
}



bool TabletDatabase::lookupButtonMapping(QMap< QString, QString >& map, const QString& companyId, const QString& deviceId)
{
    KConfigGroup deviceGroup;

    if (!lookupDeviceGroup(deviceGroup, companyId, deviceId)) {
        kError() << "Device info not found for device ID: " << deviceId << " :: company " << companyId;
        return false;
    }

    int     i   = 1;
    QString key = QLatin1String("hwbutton1");

    while(deviceGroup.hasKey(key)) {
        map.insert( QString::number(i), deviceGroup.readEntry(key));
        key = QString::fromLatin1("hwbutton%1").arg(++i);
    }

    return true;
}


bool TabletDatabase::lookupDevice(TabletInformation& devinfo, const QString& deviceId)
{
    KConfigGroup companyGroup;
    QString      companyId = lookupCompanyId(deviceId);

    if (companyId.isEmpty() || !lookupCompanyGroup(companyGroup, companyId)) {
        kError() << "No company information found for device: " << deviceId;
        return false;
    }

    KConfigGroup deviceGroup;

    if (!lookupDeviceGroup(deviceGroup, companyGroup, deviceId)) {
        kDebug() << "Device info not found for device ID: " << deviceId << " :: company " << companyId;
        return false;
    }

    devinfo.set (TabletInfo::TabletId, deviceId.toUpper());
    devinfo.set (TabletInfo::CompanyId, companyId.toUpper());
    devinfo.set (TabletInfo::CompanyName, companyGroup.readEntry("name"));

    devinfo.set (TabletInfo::TabletModel, deviceGroup.readEntry("model"));
    devinfo.set (TabletInfo::TabletName, deviceGroup.readEntry("name"));

    if( deviceGroup.readEntry( "padbuttons" )  != QLatin1String( "0" ) ||
        deviceGroup.readEntry( "wheel" )       != QLatin1String( "no" ) ||
        deviceGroup.readEntry( "touchring" )   != QLatin1String( "no" ) ||
        deviceGroup.readEntry( "touchstripl" ) != QLatin1String( "no" ) ||
        deviceGroup.readEntry( "touchstripr" ) != QLatin1String( "no" ) ) {
        devinfo.setButtons(true);
    }
    else {
        devinfo.setButtons(false);
    }

    return true;
}



bool TabletDatabase::lookupCompanyGroup(KConfigGroup& companyGroup, const QString& companyId)
{
    Q_D( TabletDatabase );
    companyGroup = KConfigGroup( d->companyConfig, companyId.toLower() );

    if( companyGroup.keyList().isEmpty() ) {
        return false;
    }

    return true;
}



QString TabletDatabase::lookupCompanyId(const QString& deviceId)
{
    Q_D( TabletDatabase );

    foreach(const QString& companyId, d->companyConfig->groupList()) {

        KConfigGroup deviceGroup;

        if (lookupDeviceGroup(deviceGroup, companyId, deviceId)) {
            return companyId;
        }
    }

    return QString();
}



bool TabletDatabase::lookupDeviceGroup(KConfigGroup& deviceGroup, const QString& companyId, const QString& deviceId)
{
    KConfigGroup companyGroup;

    if (!lookupCompanyGroup(companyGroup, companyId)) {
        return false;
    }

    return lookupDeviceGroup(deviceGroup, companyGroup, deviceId);
}



bool TabletDatabase::lookupDeviceGroup(KConfigGroup& deviceGroup, KConfigGroup& companyGroup, const QString& deviceId)
{
    Q_D( const TabletDatabase );

    // lookup device list file
    QString deviceConfigFile = companyGroup.readEntry("listfile");

    if (deviceConfigFile.isEmpty()) {
        kError() << QString::fromLatin1("Company group '%1' does not have a device list file!").arg(companyGroup.name());
        return false;
    }

    // get full path to the device list file
    QString deviceConfigFilePath;

    if (d->dataDirectory.isEmpty()) {
        deviceConfigFilePath = KStandardDirs::locate("data", QString::fromLatin1 ("wacomtablet/data/%1").arg(deviceConfigFile));
    } else {
        deviceConfigFilePath = QString::fromLatin1("%1/%2").arg(d->dataDirectory).arg(deviceConfigFile);
    }

    // open the device list file
    KSharedConfig::Ptr deviceConfig = KSharedConfig::openConfig (deviceConfigFilePath, KConfig::SimpleConfig, "data");

    if( deviceConfig->groupList().isEmpty() ) {
        kDebug() << "Device list missing for company: " << companyGroup.readEntry( "name" );
        return false;
    }

    if (deviceConfig->groupList().isEmpty()) {
        kError() << QString::fromLatin1("Device configuration file '%1' is empty or does not exist!").arg(deviceConfigFilePath);
        return false;
    }

    // lookup device
    deviceGroup = KConfigGroup( deviceConfig, deviceId.toUpper() );

    if( deviceGroup.keyList().isEmpty() ) {
        return false;
    }

    return true;
}

