import QtQuick
import QtQuick.Window
import Contacts 1.0
Window {
    width: 640
    height: 480
    visible: true
    id:root
    color:"#C8B6A6"
    ListModel{
        id:contactsModel
        ListElement{
            name:"john"; number:"0556768888"
        }
    }
    Rectangle{
        id: contactsHeading
        anchors.top: parent.top
        width: parent.width; height: 90; color:"#8D7B68"
        Text{
            anchors.centerIn: parent; text: "CONTACTS";
            font{pixelSize: 30; bold:true}
        }
    }

    ListView{
        anchors{top: contactsHeading.bottom; topMargin: 20; bottom: parent.bottom}
        width: parent.width; height: parent.height-contactsHeading.height
        clip: true
//        anchors.fill: parent
        id: listView
        model: ContactsModel{
        list : contactsList}
        spacing: 5
        delegate: Rectangle{
            id: contactRect
            width: root.width; height: 80; color:"#F1DEC9"
            state: "shrinked"
            PropertyAnimation{
                id:contactRectHeight
                running: false
                target: contactRect
                property: "height"
                to: 200
                duration: 1000
            }

            Rectangle{
                id: nameIcon
//                anchors{left:parent.left; leftMargin: 10; verticalCenter: parent.verticalCenter}
                anchors.leftMargin: 5
                width: 50; height: 50; radius: 30; color: "#C8B6A6"
                Text{
                    anchors.centerIn: parent; text: name[0]; color: "white"
                    font.pixelSize: 20
                }
            }

            Text{
                id:nameLabel
//                anchors{left:nameIcon.right; verticalCenter: parent.verticalCenter; leftMargin: 5}
                anchors.leftMargin: 5
                text: name; font.pixelSize:17; color:"#8D7B68"; font.bold: true
            }
            Text{
                id:numberLabel
                anchors.rightMargin: 5
//                anchors{right: parent.right; verticalCenter: parent.verticalCenter; rightMargin: 5}
                text: number; font.pixelSize:17; color:"#8D7B68"
            }
            states:[
                State{
                    name:"expanded"
                    PropertyChanges {
                        target: contactRect
                        height: 200
                        width: root.width
                    }
                    PropertyChanges{
                        target: nameLabel
                        font.pixelSize: 25
                        anchors.bottomMargin: 5
                    }
                    PropertyChanges{
                        target: numberLabel
                        font.pixelSize: 25
                        anchors.bottomMargin: 5
                    }
                    PropertyChanges{
                        target:nameIcon
                        height:90
                        width:90
                        radius:90
                        anchors.topMargin: 10
                    }
                    AnchorChanges{
                        target: nameLabel
                        anchors{/*verticalCenter: contactRect.verticalCenter;*/ bottom: numberLabel.top; /*bottomMargin:10;*/ horizontalCenter: contactRect.horizontalCenter /*leftMargin:10*/ }

                    }
                    AnchorChanges{
                        target: numberLabel
                        anchors{bottom: contactRect.bottom; /*bottomMargin:10;*/ horizontalCenter: contactRect.horizontalCenter /*rightMargin:10*/}
                    }
                    AnchorChanges{
                        target:nameIcon
                        anchors{top:contactRect.top; /*topMargin:20;*/ horizontalCenter: contactRect.horizontalCenter}
                    }
                },
                State{
                    name:"shrinked"
                    PropertyChanges {
                        target: contactRect
                        height: 80
                        width: root.width
                    }
//                    PropertyChanges {
//                        target: numberLabel
//                        anchors.right: contactRect.right

//                    }
                    AnchorChanges{
                        target: nameIcon
                        anchors{left:contactRect.left; /*leftMargin: 10;*/ verticalCenter: contactRect.verticalCenter}
                    }
                    AnchorChanges{
                        target: nameLabel
                        anchors{left:nameIcon.right; verticalCenter: contactRect.verticalCenter; /*leftMargin: 5*/}
                    }
                    AnchorChanges{
                        target:numberLabel
                        anchors{right: contactRect.right; verticalCenter: contactRect.verticalCenter; /*rightMargin: 5*/}
                    }
                }

            ]
            transitions: [
                Transition {
                    from: "shrinked"
                    to: "expanded"
                    PropertyAnimation{
                        target: contactRect
//                        property: "height"
                        properties: "height,width"
                        duration: 500
                        easing.type: Easing.InCirc
                    }
                    PropertyAnimation{
                       targets: [numberLabel,nameLabel]
                       properties: "font.pixelSize,anchors.bottomMargin"
                       duration: 1000
                       easing.type: Easing.OutBack
                    }
                    AnchorAnimation{
                        targets: [nameLabel,numberLabel,nameIcon]
                        duration: 800
                        easing.type: Easing.InCubic
                    }

                    PropertyAnimation{
                        target: nameIcon
                        properties: "height,width,radius,anchors.topMargin"
                        duration: 1000
                        easing.type: Easing.InBack
                    }
//                    AnchorAnimation{
//                        targets: nameIcon
//                        duration: 500
//                    }
                },
                Transition {
                    from: "expanded"
                    to: "shrinked"
                    PropertyAnimation{
                        target: contactRect
//                        property: "height"
                        properties: "height,width"
                        duration: 500
                        easing.type: Easing.OutCirc
                    }
//                    PropertyAnimation{
//                        target: numberLabel
//                        properties: "anchors.right"
//                        duration: 500
//                    }

                    AnchorAnimation{
                        targets: [nameLabel,numberLabel,nameIcon]
                        duration: 400
                        easing.type: Easing.OutCubic
                    }
                }
            ]
            MouseArea{
                anchors.fill: parent;
                onClicked: {/*contactRect.height==80 ? contactRect.height=200 : contactRect.height=80*/
                    contactRect.state == "shrinked" ? contactRect.state="expanded":contactRect.state="shrinked"
               /* contactRectHeight.running=true*/}
            }

        }
    }
}
