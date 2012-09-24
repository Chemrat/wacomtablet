/*
 * Copyright 2012 Alexander Maret-Huskinson <alex@maret.de>
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

#ifndef TESTXINPUTADAPTOR_H
#define TESTXINPUTADAPTOR_H
#include <calibrationdialog.h>

#include <QtTest>
#include <QtCore>

#include "tabletdependenttest.h"

namespace Wacom
{
class TestXinputAdaptor : public TabletDependentTest
{
    Q_OBJECT

public:

    TestXinputAdaptor(QObject* parent = 0);

protected:
    void initTestCaseDependent();

private slots:

    void testSetProperty();

};
}

QTEST_MAIN(Wacom::TestXinputAdaptor)

#endif
