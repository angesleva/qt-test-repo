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
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.1
import HRApp.models.employees 1.0
import HRApp.models.contractTypes 1.0
import "."

ItemDelegate {
    id: delegate

    property alias grid: grid
    property int leftMargin: 40
    property var dynamicTitleArray: []
    property var dynamicCLabelArray: []

    signal viewPaymentDetails(int contactId)
    signal delegateExpanded

    checkable: true

    contentItem: ColumnLayout {
        id: columnLayout

        //This grid holds the names and contract types.
        //When the delegate is not in expanded state, this is the only
        //part showing.
        GridLayout {
            id: gridLayout

            columns: 10; rowSpacing: 10

            Label {
                id: label1

                text: fullName
                font.bold: true
                elide: Text.ElideRight
                Layout.columnSpan: 5
                Layout.preferredWidth: columnLayout.width / 2 - 5
            }

            Label {
                text: contractType
                font.bold: false
                elide: Text.ElideLeft
                Layout.columnSpan: 5
                Layout.preferredWidth: columnLayout.width / 2 - 5
            }
        }

        //The 2nd level grid holds the details of an employee.
        //Also the dynamic fields are created at the end of this grid by delegates parents.
        GridLayout {
            id: grid

            visible: false
            Layout.leftMargin: leftMargin

            columns: 2; rowSpacing: 10; columnSpacing: 10

            Label {
                text: qsTr("Social secr nbr")
            }

            Label {
                text: soSecNbr
                font.bold: true
                elide: Text.ElideRight
                Layout.fillWidth: true
            }

            Label {
                text: qsTr("ContractType")
            }

            Label {
                id: contractTypeLabel

                text: contractType
                font.bold: true
                elide: Text.ElideRight
                Layout.fillWidth: true
            }
        }

        Button {
            id: paymentDetailsButton

            Layout.leftMargin: leftMargin
            visible: false
            text: qsTr("View payment details")
        }
    }

    //using this model just to request contract type dependent fields for dynamic updating of the 2nd level grid.
    ContractTypeModel {
        id: delegateContractTypeModel
    }

    Connections {
        target: delegate

        onStateChanged: {
            //Also here updating the contract type- dependent fields whenever the state changes
            var contractTypeFields = delegateContractTypeModel.contractTypeFields(
                        contractType, contactId)
            CommonVariablesAndMethods.replaceDynGridFields(grid,
                                                           CommonVariablesAndMethods.label_label, dynamicTitleArray, dynamicCLabelArray,contractTypeFields )

        }
    }

    Connections {
        target: paymentDetailsButton

        onClicked: {
            viewPaymentDetails(contactId)
            var testVariable = CommonVariablesAndMethods.label_label;
        }
    }

    states: [
        State {
            name: "expanded"
            when: delegate.checked

            PropertyChanges {
                target: grid
                visible: true
            }

            PropertyChanges {
                target: paymentDetailsButton
                visible: true
            }

            PropertyChanges {
                target: delegate
                spacing: 15
            }
        }
    ]
}
