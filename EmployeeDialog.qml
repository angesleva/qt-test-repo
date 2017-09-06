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
import QtQuick.Window 2.2
import QtQuick.VirtualKeyboard 2.0

Dialog {
    id: dialog

    property bool creatingEmployee: true
    property int currentEmployeeId:-1
    property bool operationSuccess: false;
    property var dynamicLabelArray: []
    property var dynamicEditorArray: []
    property var contractTypeFields

    signal finished(var contractTypeFields) //Not actually used at the moment. tried to correct a bug with this.

    function createEmployee() {
        creatingEmployee = true;
        currentEmployeeId = -1;
        form.fullName.clear();
        form.soSecNbr.clear();

        dialog.title = qsTr("Add Employee");
        dialog.open();
        //Initialize the contract type (all)
        form.contractType.currentIndex=0;
        //Read the dynamic, contract dependent form fields
        contractTypeFields = form.grid.contractTypeModel2.contractTypeFields("",-1);
        //Update the for with dynamic fields using a js function.
        CommonVariablesAndMethods.replaceDynGridFields(form.grid, CommonVariablesAndMethods.label_editor, dynamicLabelArray, dynamicEditorArray, contractTypeFields);
        //CommonScript.replaceDynamicGridFields(form.grid, CommonScript.label_editor, dynamicLabelArray, dynamicEditorArray, contractTypeFields);
    }

    function editEmployee(employee) {
        creatingEmployee = false
        form.fullName.text = employee.fullName;
        form.soSecNbr.text = employee.soSecNbr;
        currentEmployeeId = employee.contactId;

        dialog.title = qsTr("Edit Employee Info");
        dialog.open();
        //Read the dynmic, contract dependent form fields
        contractTypeFields = form.grid.contractTypeModel2.contractTypeFields(employee.contractType, currentEmployeeId);
        //Update the current type selection to match the employee record.
        form.contractType.currentIndex = form.contractTypeModel2.getContractIndex(employee.contractType);
        //Add the dynamic form fields.

        CommonVariablesAndMethods.replaceDynGridFields(form.grid, CommonVariablesAndMethods.label_editor, dynamicLabelArray,dynamicEditorArray,contractTypeFields);
    }

//    function keypadTopCoord(yValue)
//    {
//        console.log("KeypadCoord")
//    }

    //object properties of this dialog
    //focus: true
    modal: false
    title: qsTr("Add Employee")
    standardButtons: Dialog.Ok | Dialog.Cancel

    contentItem: EmployeeDataForm {
        id: form

        grid.rows:3
        grid.columns:2

        //rows: desktopWidth > desktopHeight ? 2 : 4
        //grid.columns: CommonVariablesAndMethods.width > CommonVariablesAndMethods.height ? 2 : 4


        //A new contract type selection -> update the dynamic form fields.
        onContractTypeChanged2: {
            contractTypeFields = contractTypeModel2.contractTypeFields(form.contractType.currentText,currentEmployeeId)
            CommonVariablesAndMethods.replaceDynGridFields(form.grid, CommonVariablesAndMethods.label_editor, dynamicLabelArray,dynamicEditorArray,contractTypeFields);
            console.log("Leveys: ", CommonVariablesAndMethods.screenWidth);
        }
    }

    //Dialog closing default signal handling.
    onAccepted:
    {
        //Before the dialog is allowed to close itself, the form content has to be saved successfully or the operation cancelled.
        //Therefore the accepted signal will not automatically close the dialog.
        visible = true;
        if(form.grid.fullName.text.length>0 && form.grid.soSecNbr.text.length>0) {

            //1. if adding a new employee record.
            var labelTextArray = [];
            var editorTextArray = [];

            for(var i=0;i<dynamicLabelArray.length;i++) {
                //For saving purposes, the unlocalized version of the label text is always used.
                labelTextArray.push(contractTypeFields[2*i]);
                editorTextArray.push(dynamicEditorArray[i].text);
            }

            if(creatingEmployee==true) { //A new employee was being created.

                //The EmployeeModel provides an IF for updating both the common and dynamic fields.
                var saveResult = employeeView.model.addOrUpdateEmployeeRecord(form.fullName.text,form.soSecNbr.text,form.contractType.currentText, currentEmployee, labelTextArray, editorTextArray);
                if (saveResult === "OK") {
                    CommonVariablesAndMethods.displayMsgBox(qsTr("Operation successful"), qsTr("Employee record created"), parent, messageConnection);
                    operationSuccess = true;
                } else {
                    CommonVariablesAndMethods.displayMsgBox(qsTr("Operation failed"), saveResult, parent, messageConnection);
                }
            } else { //Updating an already existing employee data
                var updateResult = employeeView.model.addOrUpdateEmployeeRecord(form.fullName.text,form.soSecNbr.text,form.contractType.currentText, currentEmployeeId, labelTextArray, editorTextArray);
                if(updateResult === "OK") {
                    CommonVariablesAndMethods.displayMsgBox(qsTr("Operation successful"), qsTr("Employee Record updated"), parent, messageConnection);
                    operationSuccess = true;
                } else {
                    CommonVariablesAndMethods.displayMsgBox(qsTr("Operation failed"), updateResult, parent, messageConnection);
                }
            }
        }
        else
        {
            CommonVariablesAndMethods.displayMsgBox(qsTr("Invalid input"), qsTr("Name or social secr nbr missing"), parent, messageConnection);
        }
    }

    onRejected: finished(0);

    Connections
    {
        id:messageConnection

        onMsgBoxFinished:{
            //Closing the dialog only if the save or update operation was successfull.
            if(operationSuccess===true)
            {
                //This is  just a tiny detail:
                //In case the employee record details were displayed during the editing
                //operation, they'll have to be update to confirm the user
                //a change actually took place.
                //(This does not work yet - haven't found a way to call the delegate
                //to update its non-model mapped, dynamic fields.
                finished(contractTypeFields);
                close();
            }
        }
    }
}
