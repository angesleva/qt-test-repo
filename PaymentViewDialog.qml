import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2
import HRApp.models.paymentInfo 1.0

Dialog {
    id: paymentViewDialog_

    property int employeeId: -1
    property var employeeName

    function displayPayments(employeeId_,employeeName_)
    {
        employeeId = employeeId_;
        employeeName = employeeName_;
        paymentInfoModel.setPaymentFilter(employeeId_);
        this.title = qsTr("Payment Details: ") + employeeName_
        this.open();
    }

    //focus: true
    //x:parent.x; y:parent.y + parent.height / 2; width:parent.width; height:parent.height / 2
    modal: true
    standardButtons: Dialog.Close
    contentItem:

        ListView {
        id:paymentListView

        clip:true
        anchors.top:paymentViewDialog_.header.bottom; anchors.bottom:paymentViewDialog_.bottom
        anchors.left:paymentViewDialog_.left; anchors.right:paymentViewDialog_.right
        headerPositioning: ListView.OverlayHeader
        header: Rectangle {
            id: paymentListHead

            width: paymentListView.width;  height: monthHeader.font.pixelSize + 20
            z: 2
            color: "lightsteelblue"

            GridLayout {
                id: paymentHeaderGrid

                anchors.left: parent.left; anchors.right:parent.right
                columns: 6

                Label {
                    id: monthHeader

                    text: qsTr("Month")
                    font.bold: true
                    Layout.topMargin: 10; Layout.bottomMargin: 10; Layout.leftMargin: 10
                    Layout.columnSpan: 2
                    Layout.preferredWidth: paymentHeaderGrid.width/3 - 5
                }
                Label {
                    id: yearHeader

                    text: qsTr("Year")
                    font.bold: true
                    Layout.topMargin: 10; Layout.bottomMargin: 10
                    Layout.columnSpan: 2
                    Layout.preferredWidth: paymentHeaderGrid.width/3 - 5
                }

                Label {
                    id: salaryHeader

                    text: qsTr("Salary")
                    font.bold: true
                    Layout.topMargin: 10; Layout.bottomMargin: 10
                    Layout.columnSpan: 2
                    Layout.preferredWidth: paymentHeaderGrid.width/3 - 5
                }
            }
        }

        delegate:PaymentListDelegate {
            id:paymentListDeleg

            width:paymentListView.width
        }
        model:PaymentInfoModel {
            id:paymentInfoModel
        }
    }
    onAccepted:
    {

    }
}

