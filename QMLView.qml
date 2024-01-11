import QtQuick 2.4
import com.shiftystudio.tree 1.0
import QtQuick.Controls 1.6
import QtQuick.Layouts 1.3

import "QMLView.js" as ViewFormScript


Item {
    id: viewForm

    Component.onCompleted: ViewFormScript.onCompleted()

    ColumnLayout {
        anchors.fill: parent

        anchors.rightMargin: 5
        anchors.leftMargin: 5
        anchors.bottomMargin: 5
        anchors.topMargin: 5

        spacing: 0

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Column {
                Repeater {
                    model: treeModel

                    Row {
                        Text {
                            text: index + ": " + title
                        }
                        Text {
                            text: "   " + axis
                        }
                        Text {
                            text: "   " + layout
                        }
                        Text {
                            text: "   " + bounds
                        }
                        Text {
                            text: "   " + hwnd
                        }
                    }
                }
            }
        }

        TestForm {
            repeaterModel: treeModel
            gridLayoutFlow: GridLayout.TopToBottom
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
