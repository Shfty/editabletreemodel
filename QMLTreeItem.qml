import QtQuick 2.0
import QtQuick.Layouts 1.3

Component {
    id: recursiveDelegate

    Rectangle {
        Layout.fillWidth: true
        Layout.fillHeight: true
        color: "#122aed"

        ColumnLayout {
            anchors.fill: parent
            Text {
                text: title
                color: "white"
                Layout.fillWidth: true
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 12
                fontSizeMode: Text.Fit
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "pink"

                Repeater {
                    model: 0
                    delegate: recursiveDelegate
                }
            }
        }
    }
}