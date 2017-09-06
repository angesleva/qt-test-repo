import QtQuick 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.1

Dialog {
    id:messageDialog

    property alias text1:text1

    signal msgBoxFinished();

    x:parent.x; y:parent.y
    title: "Default title"
    visible: true

    standardButtons: StandardButton.Ok

    function setText(text)
    {
        text1.text = text;
    }

    contentItem: Label{
        id: text1
        color: "black"
        Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
        wrapMode: "Wrap"
    }

    onAccepted: msgBoxFinished();
}

