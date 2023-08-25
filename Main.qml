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
        anchors{top: contactsHeading.bottom; topMargin: 20; bottom: parent.bottom; horizontalCenter: parent.horizontalCenter}
        width: parent.width-10; height: parent.height-contactsHeading.height
        clip: true
        model: ContactsModel {
            id:contactsModel
        }
        spacing: 5
        delegate:

            ListViewDelegate{
            id: contactRectView
            DragHandler{
                target: contactRectView
                yAxis.enabled: false
                acceptedDevices: PointerDevice.AllDevices
                xAxis{
                    maximum: 0
                    minimum: -100
                }
            }
            Rectangle{
                id:deleteBtn
                height:contactRectView.state=="shrinked" ? 80 : 200; width:100; color:"red";
                radius: 5
                anchors{left: contactRectView.right;}
                Text{
                    anchors.centerIn: parent; text: "Delete"
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        contactsModel.deleteFromQml(index);
                    }
                }
            }
        }

        remove: Transition {
            NumberAnimation {
                property: "x"; to:-listView.width+100
                duration: 800;  easing.type: Easing.OutCirc
            }
        }
        removeDisplaced:Transition{
            NumberAnimation{
                property:"y"; duration:1000; easing.type: Easing.InExpo
            }
        }
    }

}
