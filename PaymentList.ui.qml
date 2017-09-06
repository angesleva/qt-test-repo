import QtQuick 2.7
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.2

Rectangle {
    id: mainrect

    anchors.fill: parent

    ListView {
        id: paymentListView

        anchors.rightMargin: 0; anchors.bottomMargin: 0; anchors.leftMargin: 0; anchors.topMargin: 0
        anchors.fill: parent

        delegate: PaymentListDelegate {
            id: paymentListDeleg

            anchors.fill: parent
        }

        model: paymentListMdl
    }
}
