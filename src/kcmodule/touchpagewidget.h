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

#ifndef TOUCHPAGEWIDGET_H
#define TOUCHPAGEWIDGET_H

#include <QtGui/QWidget>
#include <QtCore/QString>

namespace Wacom
{

class TouchPageWidgetPrivate;

/**
 * The "Touch" tab of the main KCM widget.
 */
class TouchPageWidget : public QWidget
{
    Q_OBJECT

public:

    explicit TouchPageWidget(QWidget* parent = 0);
    virtual ~TouchPageWidget();

    /**
     * Loads settings from the current profile and updates the widget accordingly.
     */
    void loadFromProfile();

    /**
     * Reinitializes the widget when a new tablet gets connected.
     */
    void reloadWidget();

    /**
     * Saves the current settings to the current profile.
     */
    void saveToProfile();


public slots:

    /**
     * Called when the user enables/disables gesture support.
     */
    void onGesturesModeChanged(int state);

    /**
     * Called when any profile property value changes.
     */
    void onProfileChanged();

    /**
     * Called when the user presses the touch screen mapping button.
     */
    void onScreenMappingClicked();

    /**
     * Called when the user presses the touch tablet mapping button.
     */
    void onTabletMappingClicked();

    /**
     * Called when the user enables/disables touch support.
     */
    void onTouchModeChanged(int state);

    /**
     * Called when the state of the absolute tracking mode changes.
     */
    void onTrackingModeAbsolute(bool activated);

    /**
     * Called when the state of the relative tracking mode changes.
     */
    void onTrackingModeRelative(bool activated);



signals:

    /**
     * Emitted when the user changes configuration settings.
     */
    void changed();


protected:

    /**
     * Gets the current value for the gesture support configuration.
     *
     * @return Either "on" or "off".
     */
    const QString getGestureSupportEnabled() const;

    /**
     * Gets the current screen area mapping in profile format.
     * Possible return values are:
     *
     * - "full"               : Full desktop selection.
     * - "mapX"               : Monitor X was selected (0 <= X < number of Screens)
     * - "x1 y1 width height" : An area was selected.
     *
     * @return The current screen area mapping in profile format.
     */
    const QString& getScreenAreaMapping() const;

    /**
     * Gets the minimum motion before sending a scroll gesture.
     *
     * @return A value >= 0 as string.
     */
    const QString getScrollDistance() const;

    /**
     * Gets the current tablet area mapping in profile format.
     * Possible return values are:
     *
     * "-1 -1 -1 -1" : The whole tablet area was selected.
     * "x1 y1 x2 y2" : An area was selected with the top left corner at (x1/y1) and
     *                 the bottom right corner at (x2/y2).
     *
     * @return The current tablet mapping in profile format.
     */
    const QString& getTabletAreaMapping() const;

    /**
     * Gets the minimum time between taps for a right click.
     *
     * @return A value >= 0 as string.
     */
    const QString getTapTime() const;

    /**
     * Gets the current value of the touch support configuration.
     *
     * @return Either "on" or "off".
     */
    const QString getTouchSupportEnabled() const;

    /**
     * Returns the current tracking mode configuration.
     *
     * @return Either "absolute" or "relative".
     */
    const QString getTrackingMode() const;

    /**
     * Gets the minimum distance for a zoom gesture.
     *
     * @return A value >= 0 as string.
     */
    const QString getZoomDistance() const;

    /**
     * Checks if gesture support is selected.
     *
     * @return True if gesture support is enabled, else false.
     */
    bool isGesturesSupportEnabled() const;

    /**
     * Checks if touch support is selected.
     *
     * @return True if touch support is enabled, else false.
     */
    bool isTouchSupportEnabled() const;

    /**
     * Sets a new value for gesture support and updates all widgets accordingly.
     *
     * @param value Either true or false.
     */
    void setGesturesSupportEnabled(bool value);

    /**
     * Sets a new screen area mapping and updates all widgets accordingly.
     * The given value has to be in profile format. Valid values are:
     *
     * - "full"               : Full desktop selection.
     * - "mapX"               : Monitor X gets selected (0 <= X < number of Screens)
     * - "x1 y1 width height" : An area gets selected.
     *
     * @param value The new screen area mapping selection.
     */
    void setScreenAreaMapping(const QString& value);

    /**
     * Sets the minimum motion before sending a scroll gesture and updates
     * all widgets accordingly.
     *
     * @param value A value >= 0 as string.
     */
    void setScrollDistance(const QString& value);

    /**
     * Sets a new tablet area mapping and updates all widgets accordingly.
     * The given value has to be in profile format. Valid values are:
     *
     * "-1 -1 -1 -1" : The whole tablet area gets selected.
     * "x1 y1 x2 y2" : An area gets selected with the top left corner at (x1/y1) and
     *                 the bottom right corner at (x2/y2).
     *
     * @param value The new tablet area mapping in profile format.
     */
    void setTabletAreaMapping(const QString& value);

    /**
     * Sets the minimum time between taps for a right click and updates
     * all widgets accordingly.
     *
     * @param value A value >= 0 as string.
     */
    void setTapTime(const QString& value);

    /**
     * Sets the tracking mode and updates all widgets accordingly.
     *
     * @param value Either "absolute" or "relative"
     */
    void setTrackingMode(const QString& value);

    /**
     * Sets a new value for touch support and updates all widgets accordingly.
     *
     * @param value Either true or false.
     */
    void setTouchSupportEnabled(bool value);

    /**
     * Sets the minimum distance for a zoom gesture and updates all widgets accordingly.
     *
     * @param value A value >= 0 as string.
     */
    void setZoomDistance(const QString& value);


private:

    /**
     * Initializes this widget. Must only be called once by a constructor.
     */
    void setupUi();

    Q_DECLARE_PRIVATE( TouchPageWidget )
    TouchPageWidgetPrivate *const d_ptr; //!< D-Pointer for this class.

}; // CLASS
}  // NAMESPACE
#endif // HEADER PROTECTION