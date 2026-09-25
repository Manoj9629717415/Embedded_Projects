import QtQuick
import QtQuick.Controls

Rectangle {
    signal backRequested()
    width: 800
    height: 480
    color: "darkblue"

    Column{
        anchors.horizontalCenter : parent.horizontalCenter
        anchors.verticalCenter : parent.verticalCenter
        spacing : 50
        Text {
            text: "SETTINGS"
            color: "white"
            font.pixelSize: 40
        }

        Button {
            text: "Back"

            onClicked: {
                backRequested()
            }
        }
    }

}