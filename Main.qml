import QtQuick
import QtQuick.Window
import Contacts 1.0
Window {
    width: 640
    height: 480
    visible: true
    id:root
    color:"#C8B6A6"

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
        id: listView
        anchors{top: contactsHeading.bottom; topMargin: 20; bottom: parent.bottom}
        width: parent.width; height: parent.height-contactsHeading.height
        clip: true
        model: ContactsModel {
            id:contactsModel
            list : contactsList }
        spacing: 5      
        delegate: Rectangle{
            id: contactRect
            /*width: root.width; height:80;*/ color:"#F1DEC9"
            state:  "shrinked"
            Rectangle{
                id: nameIcon
//                anchors{left:parent.left; leftMargin: 10; verticalCenter: parent.verticalCenter}
                anchors.leftMargin: 5
                width: 50; height: 50; radius: 30; color: "#C8B6A6"
                Text{
                    id: nameFirstLetter
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
//                    when: contactRect.ListView.isCurrentItem
                    PropertyChanges {
                        target: contactRect
                        height: 200; width: root.width
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
                    PropertyChanges {
                        target: nameFirstLetter
                        font.pixelSize: 40
                    }
                    PropertyChanges{
                        target:nameIcon
                        height:90; width:90; radius:90
                        anchors.topMargin: 10
                    }
                    AnchorChanges{
                        target: nameLabel
                        anchors{bottom: numberLabel.top; horizontalCenter: contactRect.horizontalCenter }

                    }
                    AnchorChanges{
                        target: numberLabel
                        anchors{bottom: contactRect.bottom; horizontalCenter: contactRect.horizontalCenter}
                    }
                    AnchorChanges{
                        target:nameIcon
                        anchors{top:contactRect.top; horizontalCenter: contactRect.horizontalCenter}
                    }
                },
                State{
                    name:"shrinked"
                    PropertyChanges {
                        target: contactRect
                        height: 80; width: root.width
                    }
                    AnchorChanges{
                        target: nameIcon
                        anchors{left:contactRect.left; verticalCenter: contactRect.verticalCenter}
                    }
                    AnchorChanges{
                        target: nameLabel
                        anchors{left:nameIcon.right; verticalCenter: contactRect.verticalCenter;}
                    }
                    AnchorChanges{
                        target:numberLabel
                        anchors{right: contactRect.right; verticalCenter: contactRect.verticalCenter;}
                    }
                }

            ]
            transitions: [
                Transition {
                    from: "shrinked"; to: "expanded"
                    PropertyAnimation{
                        target: contactRect; properties: "height,width"
                        duration: 500; easing.type: Easing.InCirc
                    }
                    PropertyAnimation{
                       targets: [numberLabel,nameLabel]; properties: "font.pixelSize,anchors.bottomMargin"
                       duration: 1000; easing.type: Easing.OutBack
                    }
                    AnchorAnimation{
                        targets: [nameLabel,numberLabel,nameIcon]
                        duration: 800; easing.type: Easing.InCubic
                    }
                    PropertyAnimation{
                        target: nameIcon; properties: "height,width,radius,anchors.topMargin"
                        duration: 1000; easing.type: Easing.InBack
                    }
                    PropertyAnimation{
                        target: nameFirstLetter; properties: "font.pixelSize"
                        duration: 1000; easing.type: Easing.InBack
                    }

                },
                Transition {
                    from: "expanded"; to: "shrinked"
                    PropertyAnimation{
                        target: contactRect; properties: "height,width"
                        duration: 500; easing.type: Easing.OutCirc
                    }

                    AnchorAnimation{
                        targets: [nameLabel,numberLabel,nameIcon]
                        duration: 400; easing.type: Easing.OutCubic
                    }
                }
            ]
            MouseArea{
                id:mymouse
                anchors.fill: parent;
                onClicked: {
                    contactRect.ListView.view.currentIndex = index
//                    contactRect.ListView.view.current
                    console.log(/*listView.currentIndex,*/ contactRect.ListView.view.currentIndex)
                    contactRect.state === "shrinked" ? contactRect.state="expanded":contactRect.state="shrinked"
               }
            }

        }
    }
}
