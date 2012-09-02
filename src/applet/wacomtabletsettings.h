/*
 * Copyright 2010 Jörg Ehrichs <joerg.ehichs@gmx.de>
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

#ifndef TABLETSETTINGS_H
#define TABLETSETTINGS_H

//Plasma
#include <KDE/Plasma/PopupApplet>

class QDBusInterface;
class QDBusServiceWatcher;
class KCModuleProxy;

namespace Wacom
{
class TabletApplet;

/**
  * This plasma applet provides easy to switch the tablet profiles.
  * Furthermore simple error detection and the possibility to change
  * the pad rotation and pen mode /absolute/relative) on the fly.
  */
class WacomTabletSettings : public Plasma::PopupApplet
{
    Q_OBJECT

public:
    /**
      * Constructor of the applet
      * @param parent the parent of this object
      * @param args required by KPlugin signature, ignored
      */
    WacomTabletSettings(QObject *parent, const QVariantList &args);

    /**
    * Default destructor
    **/
    ~WacomTabletSettings();

    /**
      * Initialize the applet (called by plasma automatically)
      *
      * Creates the TabletApplet widget and runs some test to check if the
      * DBus connection to the daemon is available and a tablet is connected.
      *
      * If the checks fail a error message will be shown until the daemon is
      * responsive again or a tablet is connected.
      */
    void init();

    /**
      * The graphics widget that displays the list of devices.
      *
      * @return the TabletApplet widget instance
      */
    QGraphicsWidget *graphicsWidget();

protected:
    /**
      * Called to create a configuration interface within the
      * given @p parent dialog.
      *
      * If invoked, this applet will add its configuration pages to the
      * given dialog.
      *
      * @param parent the dialog to add the interface to
      */
    void createConfigurationInterface(KConfigDialog *parent);

private slots:
    /**
      * This slot is called when the ok button in the configuration interface is used.
      *
      * Reimplements the save mechanism from the KCModule to ensure the changed values
      * are saved in the profile settings.
      */
    void configAccepted();

private:
    TabletApplet   *m_applet;           /**< The content widget for this applet. Contains all important values and functions */
    KCModuleProxy  *m_settingsWidget;   /**< Embedded KCM modules in the configuration dialog */
    QDBusInterface *m_tabletInterface;  /**< DBus /Tablet interface to the daemon */
    QDBusServiceWatcher *m_watcher;     /**< Watches over the conencted dbus service, changes the applet status when the service gets available / unavailable */

}; // CLASS
}  // NAMESPACE
#endif // HEADER PROTECTION
