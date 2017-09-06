import QtQuick 2.7
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.2
import HRApp.models.paymentInfo 1.0

GridLayout {
    id: paymentDialogLayout

    property alias paymentDFormInfoModel: paymentDFormInfoModel
    property alias paymentDialogLayout: paymentDialogLayout
    property alias yearCombo: yearCombo
    property alias monthCombo: monthCombo
    property string placeholderText: qsTr("<enter>")

    columns: 1

    GroupBox {
        id: yearBox

        spacing: 5
        title: qsTr("Year")
        Layout.fillWidth: parent

        ComboBox {
            id: yearCombo

            anchors.fill: parent
            //Nothing fancy here - just for the concept.
            model: ["2016", "2017", "2018"]
        }
    }

    GroupBox {
        id: monthBox

        title: qsTr("Month")
        Layout.fillWidth: parent

        ComboBox {
            id: monthCombo

            anchors.fill: parent
            //Nothing fancy here - just for the concept.
            model: ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"]
        }
    }

    PaymentInfoModel {
        id: paymentDFormInfoModel
    }
}
