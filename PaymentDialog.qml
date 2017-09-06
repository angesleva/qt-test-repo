import QtQuick 2.7
import QtQuick.Controls 2.2

Dialog {
    id: paymentDialog

    property var paymentInputFields
    property var dynamicLabelArray: []
    property var dynamicEditorArray: []
    property var employee
    property bool operationSuccess: false;

    function inputPayment (employee_)
    {
        employee = employee_
        paymentDialog.title = qsTr("Input Payment for ") + employee.fullName;
        paymentDialog.open();
        //Updating the contract dependent fields from the model dynamically.
        paymentInputFields = paymentDialogForm.paymentDFormInfoModel.readPaymentInputfields(employee.contactId);
        CommonVariablesAndMethods.replaceDynGridFields(paymentDialogForm.paymentDialogLayout,
                                                       CommonVariablesAndMethods.label_editor, dynamicLabelArray, dynamicEditorArray, paymentInputFields)
    }

    standardButtons: Dialog.Ok | Dialog.Cancel

    //The actual form for month, year and payment info.
    contentItem: PaymentDialogForm {
        id: paymentDialogForm
    }

    onAccepted:
    {
        //Reading info from the dynamically created edit components to pass it to the model for saving.
        visible = true; //Not allowed to be hidden automatically.
        //1. if adding a new employee record.
        var labelTextArray = [];
        var editorTextArray = [];
        for(var i=0;i<dynamicLabelArray.length;i++)
        {
            labelTextArray.push(dynamicLabelArray[i].text);
            editorTextArray.push(dynamicEditorArray[i].text);
        }

        //Use the payment model to update time and payment info.
        var saveResult = paymentDialogForm.paymentDFormInfoModel.savePaymentInputFields(
                    paymentDialogForm.yearCombo.currentText,paymentDialogForm.monthCombo.currentText,
                    labelTextArray, editorTextArray, employee.contactId)

        //Oops, always indicating success...to be corrected - requires support from the model.
        CommonVariablesAndMethods.displayMsgBox(qsTr("Operation successful"), saveResult ,parent,
                                                messageConnection);
        operationSuccess = true;

    }

    Connections {
        id:messageConnection

        target: this
        onMsgBoxFinished:{
            //Closing the dialog only if the save or update operation was successfull.
            if(operationSuccess===true) close();
        }

    }

}

