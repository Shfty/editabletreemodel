import QtQuick 2.4
import QtQuick.Controls 1.6
import QtQuick.Layouts 1.3

Rectangle {
    property var repeaterModel
    property var gridLayoutFlow

    GridLayout {
        id: gridLayout
        flow: gridLayoutFlow
        rows: 1
        columns: 1

        anchors.fill: parent

        Repeater {
            model: repeaterModel
            QMLTreeItem {
            }
        }
    }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
