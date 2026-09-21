import QtQuick
import QtQuick.Controls

Rectangle {
    signal backrequested()
    width: 800
    height: 480
    color: "darkblue"



        Text {
            anchors.centerIn: parent

            text: "SETTINGS"
            color: "white"
            font.pixelSize: 40
        }

        Button {
            text: "Back"

            anchors.centerIn: parent

            onClicked: {
                backRequested()
            }
        }

}