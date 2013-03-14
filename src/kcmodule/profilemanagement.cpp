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

#include "profilemanagement.h"

// common
#include "dbustabletinterface.h"
#include "tabletinfo.h"
#include "devicetype.h"
#include "tabletprofile.h"
#include "property.h"

//KDE includes
#include <KDE/KDebug>

//Qt includes
#include <QtCore/QRegExp>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>

#include <QtCore/QDebug>

using namespace Wacom;

ProfileManagement::ProfileManagement()
    : m_profileManager(QLatin1String("tabletprofilesrc"))
{
    reload();
}

ProfileManagement::ProfileManagement(const ProfileManagement& )
    : m_profileManager(QLatin1String("tabletprofilesrc"))
{
    // this class is a singleton - no copying allowed
    reload();
}


ProfileManagement& ProfileManagement::operator=(const ProfileManagement& )
{
    // this class is a singleton - no copying allowed
    return *this;
}


ProfileManagement& ProfileManagement::instance()
{
    static ProfileManagement instance;
    return instance;
}


void ProfileManagement::createNewProfile( const QString &profilename )
{
    //get information via DBus
    QDBusReply<QString> deviceName = DBusTabletInterface::instance().getInformation(TabletInfo::TabletName);
    m_deviceName = deviceName;
    QDBusReply<QString> padName    = DBusTabletInterface::instance().getDeviceName(DeviceType::Pad);
    QDBusReply<QString> stylusName = DBusTabletInterface::instance().getDeviceName(DeviceType::Stylus);
    QDBusReply<QString> eraserName = DBusTabletInterface::instance().getDeviceName(DeviceType::Eraser);

    if( m_deviceName.isEmpty() || !padName.isValid() || !stylusName.isValid() || !eraserName.isValid() ) {
        kDebug() << "no device information are found. Can't create a new profile";
        return;
    }

    kDebug() << "create a new profile for :: device:" << m_deviceName << "pad:" << padName;


    m_profileManager.readProfiles(m_deviceName);
    TabletProfile tabletProfile = m_profileManager.loadProfile(profilename);
    DeviceProfile padDevice     = tabletProfile.getDevice(DeviceType::Pad);

    // use properties to set "string" values
    //padDevice.setProperty(Property::Button1, QLatin1String("1"));
    padDevice.setProperty(Property::Button2, QLatin1String("2"));
    padDevice.setProperty(Property::Button3, QLatin1String("3"));
    padDevice.setProperty(Property::Button4, QLatin1String("4"));
    padDevice.setProperty(Property::Button5, QLatin1String("5"));
    padDevice.setProperty(Property::Button6, QLatin1String("6"));
    padDevice.setProperty(Property::Button7, QLatin1String("7"));
    padDevice.setProperty(Property::Button8, QLatin1String("8"));
    padDevice.setProperty(Property::Button9, QLatin1String("9"));
    padDevice.setProperty(Property::Button10, QLatin1String("10"));
    padDevice.setProperty(Property::StripLeftUp, QLatin1String("11"));
    padDevice.setProperty(Property::StripLeftDown, QLatin1String("12"));
    padDevice.setProperty(Property::StripRightUp, QLatin1String("13"));
    padDevice.setProperty(Property::StripRightDown, QLatin1String("14"));
    //padDevice.setProperty(Property::RelWheelUp, QLatin1String("15"));
    //padDevice.setProperty(Property::RelWheelDown, QLatin1String("16"));
    padDevice.setProperty(Property::AbsWheelUp, QLatin1String("4"));
    padDevice.setProperty(Property::AbsWheelDown, QLatin1String("5"));

    tabletProfile.setDevice(padDevice);


    DeviceProfile stylusDevice = tabletProfile.getDevice(DeviceType::Stylus);

    //stylusDevice.setProperty(Property::Button1, QLatin1String("1")); // removed beacuse the wacomdriver has an awfull bug when reset button 1 to 1 (leftclick)
    stylusDevice.setProperty(Property::Button2, QLatin1String("2"));
    stylusDevice.setProperty(Property::Button3, QLatin1String("3"));
    stylusDevice.setProperty(Property::Mode, QLatin1String("absolute"));
    stylusDevice.setProperty(Property::PressureCurve, QLatin1String("0 0 100 100"));
    stylusDevice.setProperty(Property::Threshold, QLatin1String("27"));
    stylusDevice.setProperty(Property::Rotate, QLatin1String("none"));
    stylusDevice.setProperty(Property::TabletArea, QLatin1String("-1 -1 -1 -1"));

    QDBusReply<QString> stylusArea = DBusTabletInterface::instance().getProperty( DeviceType::Stylus, Property::Area );
    stylusDevice.setProperty(Property::Area, stylusArea.isValid() ? stylusArea.value() : QLatin1String("0 0 0 0"));

    QDBusReply<QString> stylusTpcButton = DBusTabletInterface::instance().getProperty( DeviceType::Stylus, Property::TabletPcButton );
    stylusDevice.setProperty(Property::TabletPcButton, stylusTpcButton.isValid() ? stylusTpcButton.value() : QLatin1String("on"));

    tabletProfile.setDevice(stylusDevice);


    DeviceProfile eraserDevice = tabletProfile.getDevice(DeviceType::Eraser);

    //eraserDevice.setProperty(Property::Button1, QLatin1String("1")); // removed beacuse the wacomdriver has an awfull bug when reset button 1 to 1 (leftclick)
    eraserDevice.setProperty(Property::Button2, QLatin1String("2"));
    eraserDevice.setProperty(Property::Button3, QLatin1String("3"));
    eraserDevice.setProperty(Property::Mode, QLatin1String("absolute"));
    eraserDevice.setProperty(Property::PressureCurve, QLatin1String("0 0 100 100"));
    eraserDevice.setProperty(Property::Threshold, QLatin1String("27"));

    // TODO RawFilter, Suppress, RawSample, MapToOutput

    eraserDevice.setProperty(Property::Rotate, QLatin1String("none"));
    eraserDevice.setProperty(Property::TabletArea, QLatin1String("-1 -1 -1 -1"));

    QDBusReply<QString> eraserArea = DBusTabletInterface::instance().getProperty( DeviceType::Eraser, Property::Area );
    eraserDevice.setProperty(Property::Area, eraserArea.isValid() ? eraserArea.value() : QLatin1String("0 0 0 0"));

    tabletProfile.setDevice(eraserDevice);


    // also add section for the touch if we have a touch tool
    QDBusReply<QString> touchName = DBusTabletInterface::instance().getDeviceName(DeviceType::Touch);

    QString validName = touchName.value();
    if( !validName.isEmpty() ) {

        DeviceProfile touchDevice = tabletProfile.getDevice(DeviceType::Touch);

        touchDevice.setProperty(Property::Rotate, QLatin1String("none"));
        touchDevice.setProperty(Property::TabletArea, QLatin1String("-1 -1 -1 -1"));
        touchDevice.setProperty(Property::Mode, QLatin1String("absolute"));
        touchDevice.setProperty(Property::TapTime, QLatin1String("250"));

        QDBusReply<QString> touchArea = DBusTabletInterface::instance().getProperty( DeviceType::Touch, Property::Area );
        touchDevice.setProperty(Property::Area, touchArea.isValid() ? touchArea.value() : QLatin1String("0 0 0 0"));

        QDBusReply<QString> touch = DBusTabletInterface::instance().getProperty( DeviceType::Touch, Property::Touch );
        touchDevice.setProperty(Property::Touch, touch.isValid() ? touch.value() : QLatin1String("on"));

        QDBusReply<QString> gesture = DBusTabletInterface::instance().getProperty( DeviceType::Touch, Property::Gesture );
        touchDevice.setProperty(Property::Gesture, gesture.isValid() ? gesture.value() : QLatin1String("on"));

        QDBusReply<QString> zoomDistance = DBusTabletInterface::instance().getProperty( DeviceType::Touch, Property::ZoomDistance );
        touchDevice.setProperty(Property::ZoomDistance, zoomDistance.isValid() ? zoomDistance.value() : QLatin1String("50"));

        QDBusReply<QString> scrollDistance = DBusTabletInterface::instance().getProperty( DeviceType::Touch, Property::ScrollDistance );
        touchDevice.setProperty(Property::ScrollDistance, scrollDistance.isValid() ? scrollDistance.value() : QLatin1String("50"));

        tabletProfile.setDevice(touchDevice);
    }

    m_profileManager.saveProfile(tabletProfile);
}

const QStringList ProfileManagement::availableProfiles()
{
    m_profileManager.readProfiles(m_deviceName);
    return m_profileManager.listProfiles();
}

void ProfileManagement::deleteProfile()
{
    m_profileManager.readProfiles(m_deviceName);
    m_profileManager.deleteProfile(m_profileName);

    m_profileName.clear();
    m_profileManager.reload();

    if (m_profileManager.listProfiles().isEmpty()) {
        createNewProfile();
        m_profileManager.reload();
    }
}

DeviceProfile ProfileManagement::loadDeviceProfile(const DeviceType& device)
{
    m_profileManager.readProfiles(m_deviceName);
    return m_profileManager.loadProfile(m_profileName).getDevice(device);
}


bool ProfileManagement::saveDeviceProfile(const DeviceProfile& profile)
{
    if (!m_profileManager.readProfiles(m_deviceName)) {
        return false;
    }

    TabletProfile tabletProfile = m_profileManager.loadProfile(m_profileName);
    tabletProfile.setDevice(profile);

    return m_profileManager.saveProfile(tabletProfile);
}



void ProfileManagement::setProfileName( const QString &name )
{
    m_profileName = name;
}

QString ProfileManagement::profileName() const
{
    return m_profileName;
}


void ProfileManagement::reload()
{
    QDBusReply<QString> deviceName  = DBusTabletInterface::instance().getInformation(TabletInfo::TabletName);

    if( deviceName.isValid() ) {
        m_deviceName = deviceName;
    }
}
