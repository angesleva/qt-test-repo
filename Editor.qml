import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.0

TextField {
    id: editor

    Layout.fillWidth: true
    Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
    placeholderText: "<enter>"

    MouseArea {
        anchors.fill: parent
        onClicked: {
            editor.forceActiveFocus();
            //editor.openSoftwareInputPanel();
        }
    }
}
