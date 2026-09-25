import QtQuick
import QtQuick.Controls

ApplicationWindow {
    id : dashboard
    visible: true
    width: 800
    height: 480
    background: Rectangle {
        color:"black"
    }

    Loader{
        id:settingsLoader
        anchors.fill:parent
        active:false
        z : 100

        onLoaded:item.backRequested.connect(function(){
            settingsLoader.active = false
        })
    }

    Row{
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.top: parent.top
        anchors.topMargin: 30
        spacing:50

        Item {
            width: 300
            height: 300

            Rectangle{
                anchors.fill: parent
                radius: width / 2
                color: "#202020"
                border.width: 4
                border.color: "white"
            }

            // Speed value in center
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 30

                text: vehicle.speed + " km/h"

                font.pixelSize: 30
                color: "white"
            }

            Rectangle{
                id:needle

                width:6
                height:110

                color: "red"

                anchors.bottom: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter

                transformOrigin: Item.Bottom

                rotation: ((240 * vehicle.speed ) / 200) - 120

                Behavior on rotation {
                    NumberAnimation {
                        duration: 300
                        easing.type: Easing.Linear
                    }
                }
            }

            // Center pivot
            Rectangle {
                width: 18
                height: 18
                radius: 9

                anchors.centerIn: parent

                color: "white"
            }
        }
    


        Column {
            id : info
            spacing: 20

            Row {
                spacing:5

                Text {
                        text: "Battery: "
                        font.pixelSize: 32
                        color: "white"
                    }

                Rectangle{
                    width:150
                    height:30
                    color:"gray"

                    Rectangle{
                        id:batteryFill

                        height:parent.height
                        width: (parent.width * vehicle.batteryLevel) / 100
                        color:"green"

                        Behavior on width {
                            NumberAnimation {
                                duration: 300
                                easing.type: Easing.Linear
                            }
                        }
                    }
                }

                Text {
                    id: batterytext
                    text: vehicle.batteryLevel+"%"
                    font.pixelSize: 32
                    color: "white"
                }
            }


            Text {
                text: "Temperature: " + vehicle.temperature + " °C"
                font.pixelSize: 32
                color: "white"
            }

            Row{
                spacing:30
                Button {
                    text : "[ + ]"

                    onClicked : {
                        vehicle.increaseSpeed()
                    }
                }

                Button {
                    text : "[ - ]"

                    onClicked : {
                        vehicle.decreaseSpeed()
                    }
                }

                Button {
                    text : "[ Settings ]"

                    onClicked : {
                        settingsLoader.source = "Settings.qml"
                        settingsLoader.active = true
                    }
                }
            }
            Item{
                id : warningArea
                width:250
                height:40

                 Text {
                    id: warningText
                    anchors.fill: parent
                    text: "LOW BATTERY"
                    font.pixelSize: 30
                    visible : true
                    color : "red"
                    opacity : 0
                }

                 StateGroup {
                    id: warningStateGroup
                    states : [
                        State {
                            name : "lowBattery"
                            when : vehicle.batteryLevel < 30

                            PropertyChanges {
                                target : warningText
                                opacity : 1.0
                            }
                             PropertyChanges {
                                target : batteryFill
                                color : "red"
                            }
                             PropertyChanges {
                                target : batterytext
                                color : "red"
                            }
                        }
                    ]
                transitions : [
                    Transition {
                        from : ""
                        to : "lowBattery"

                        NumberAnimation{
                            target:warningText
                            property : "opacity"
                            duration : 3000
                        }
                    },
                    Transition {
                        from : "lowBattery"
                        to : ""

                        NumberAnimation{
                            target : warningText
                            property : "opacity"
                            duration : 3000
                        }
                    }
                ]
                }
            }
        }

   
    }

}