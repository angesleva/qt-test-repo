import QtQuick 2.4
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.2

Component {
    id: paymentListHeader

    Rectangle {

        width: container.width
        height: childrenRect.height
        color: "lightsteelblue"

        Text {
            text: Header
            font.bold: true
            font.pixelSize: 20
        }
    }
}
