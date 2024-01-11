import QtQuick 2.4
import com.shiftystudio.tree 1.0
import QtQuick.Controls 1.6

Item {
    width: 400
    height: 400

    property TreeModel viewModel

    Column {
        Repeater {
            model: viewModel

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
