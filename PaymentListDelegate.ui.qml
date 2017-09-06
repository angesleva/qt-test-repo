import QtQuick 2.7
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.2
import HRApp.models.paymentInfo 1.0

ItemDelegate {
    id: paymentItemDelegate

    contentItem: Rectangle {
        id: paymentItemRect

        anchors.top: paymentItemDelegate.top; anchors.bottom: paymentItemDelegate.bottom
        anchors.left: paymentItemDelegate.left; anchors.right: paymentItemDelegate.right
        color: index % 2 == 0 ? "white" : "lightgray"

        GridLayout {
            id: paymentGrid

            anchors.left: parent.left; anchors.right: parent.right //The widgth of Rectangle
            columns: 6

            Label {
                id: monthText

                text: paymentMonth
                Layout.columnSpan: 2
                Layout.preferredWidth: paymentGrid.width / 3 - 5
                Layout.leftMargin: 10; Layout.topMargin: 10
            }
            Label {
                id: yearText

                text: paymentYear
                Layout.columnSpan: 2
                Layout.preferredWidth: paymentGrid.width / 3 - 5
                Layout.topMargin: 10
            }
            Label {
                id: salaryText
                text: paymentSum
                Layout.columnSpan: 2
                Layout.preferredWidth: paymentGrid.width / 3 - 5
                Layout.topMargin: 10
            }
        }
    }
}
