import QtQuick 2.4
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.1

Component {
        id: sectionHeading
        Rectangle {
            width: container.width
            height: childrenRect.height
            color: "lightsteelblue"

            Text {
                text: section
                font.bold: true
                font.pixelSize: 20
            }
        }
    }
