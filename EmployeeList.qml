/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.7
import QtQuick.Controls 2.2
import HRApp.models.employees 1.0
import HRApp.models.contractTypes 1.0
import QtQuick.VirtualKeyboard 2.0
import QtQuick.Window 2.2

ApplicationWindow {
    id: window

    property int currentEmployee: -1 //Index

    width: Screen.desktopAvailableWidth//320
    height: Screen.desktopAvailableHeight //480
    visible: true
    title: qsTr("Employee List")

    Item {
        id: root
        anchors.centerIn: window.contentItem
        property bool portraitMode: Screen.desktopAvailableHeight > Screen.desktopAvailableWidth ? true : false
        rotation: portraitMode ? 90 : 0
        width: portraitMode ? window.height : window.width
        height: portraitMode ? window.width : window.height

        //The loader component is used to load all the temporarily displayed components, like
        //dialogs.
        Loader {
            id: mainLoader

            Connections {
                target: mainLoader.item
                //onFinished: popup.visible = false; //Closing the virtual keyboard popup
            }
        }

        //Menu activated by long press.
        Menu {
            id: employeeMenu

            x: parent.width / 2 - width / 2; y: parent.height / 2 - height / 2
            modal: true

            Label {
                padding: 10
                font.bold: true
                width: parent.width
                horizontalAlignment: Qt.AlignHCenter
                text: currentEmployee >= 0 ? employeeView.model.get(currentEmployee).fullName : ""
            }

            MenuItem {
                text: qsTr("Edit employee data")

                onTriggered: {
                    //mainLoader.source = ""; //Destroying hte previous source first to avoid an implicitHeight binding warning.
                    mainLoader.source = "EmployeeDialog.qml";

                    mainLoader.x = 0;
                    mainLoader.width = root.width;
                    mainLoader.item.width = mainLoader.width;
                    mainLoader.y = 0;
                    console.log("Current Employee: ", currentEmployee)
                    mainLoader.item.editEmployee(employeeView.model.get(currentEmployee));
                    mainLoader.item.z = -1;
                }
            }

            MenuItem {
                text: qsTr("Remove employee")
                onTriggered: employeeView.model.remove(currentEmployee)
            }

            MenuItem {
                text:qsTr("Add Payment")
                onTriggered: {
                    mainLoader.source = ""; //Destroying hte previous source first to avoid an implicitHeight binding warning.
                    mainLoader.source = "PaymentDialog.qml";
                    mainLoader.x = 0;
                    mainLoader.y = 0;
                    mainLoader.item.inputPayment(employeeView.model.get(currentEmployee));
                }
            }
        }

        //Payment dialog for adding new payments for employeees.
//        PaymentDialog {
//            id: paymentDialog

//            x:parent.width - paymentDialog.width
//        }

        //Payment view dialog for viewing the payment stats of a single employee.
//        PaymentViewDialog {
//            id:paymentViewDialog

//            x:parent.x; y:parent.y + parent.height / 2; width:parent.width; height:parent.height / 2
//        }

        //An area containing a group box.
        Rectangle //used to avoid using clip property
        {
            id: contractTypesArea

            anchors.left: parent.left; anchors.right: parent.right; anchors.top:window.top
            height: contractTypesGroup.height //Adapting to the size of the child component.
            color: "white"


            GroupBox {
                id: contractTypesGroup
                opacity: 1



                title: "Contract Types"
                anchors.left: parent.left; anchors.right: parent.right; anchors.top:window.top

                ContractTypeView {
                    id:contractView

                    anchors.fill:parent

                    onContractTypeChanged: employeeView.model.setContractFilter(index)
                }

            }
        }

        //EmployeeView contains the actual list view displaying the employee data.
        EmployeeView {
            id: employeeView

            z: -1 //The list has to be lower than other components in z order.
            anchors.top: contractTypesArea.bottom; anchors.bottom:parent.bottom; width: parent.width;
            anchors.topMargin: 10

            //Menu control opened by long press.
            onPressAndHold: {
                currentEmployee = index;
                employeeMenu.open();
            }

            //view payment button pressed in subview.
            onViewPaymentDetails2: {
                mainLoader.source = "PaymentViewDialog.qml";
                mainLoader.x = 0;
                mainLoader.y = root.height / 2;
                mainLoader.width = root.width;
                mainLoader.item.width = mainLoader.width;
                mainLoader.height = root.height / 2;
                mainLoader.item.height = mainLoader.height;
                console.log("Current Employee: ", currentEmployee);
                mainLoader.item.displayPayments(contactId, employeeView.model.get(currentEmployee).fullName);
                //paymentViewDialog.displayPayments(contactId, employeeView.model.get(currentEmployee).fullName);
            }

            //Maintaining the current employee selection on top level.
            onNewListSelection:
            {
                currentEmployee = index
            }
        }

        //A button for creating a new employee record.
        RoundButton {
            text: qsTr("+")
            highlighted: true
            anchors.right: parent.right; anchors.bottom: parent.bottom
            anchors.margins: 10
            onClicked: {
                CommonVariablesAndMethods.screenWidth = window.width;
                CommonVariablesAndMethods.screenHeight = window.height;
                mainLoader.source = ""; //Destroying hte previous source first to avoid an implicitHeight binding warning.
                currentEmployee = -1;
                mainLoader.source = "EmployeeDialog.qml";
                mainLoader.x = 0;
                mainLoader.width = parent.width;
                mainLoader.item.width = mainLoader.width;
                mainLoader.y = 0;
                mainLoader.item.createEmployee();
                mainLoader.item.z = -1;
                //popup.visible = true;
                console.log("Height: ", CommonVariablesAndMethods.screenHeight);
                console.log("Width: ", CommonVariablesAndMethods.screenWidth);


            }
        }


        //When using dialogs for input, inputPanel's z value doesn't seem to bring it on
        //top of the dialog.
        //As a work around, inputPanel lives in a popup that can cover a dialog.
        Popup {
            id:popup

            z:100
            x:0; y: parent.height - inputPanel.height //mainLoader.item.height
            height: inputPanel.height; width: parent.width
            visible: Qt.inputMethod.visible
            padding: 0
            closePolicy: Popup.NoAutoClose

            InputPanel {
                id: inputPanel
                y: Qt.inputMethod.visible ? parent.height - inputPanel.height : parent.height
                anchors.left: parent.left; anchors.right: parent.right
            }

//            onVisibleChanged: {
//                if(visible == true) mainLoader.item.keypadTopCoord(parent.height - inputPanel.height)

//            }
        }
    }
}
//}
