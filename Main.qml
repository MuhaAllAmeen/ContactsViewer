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

    Flickable{
            anchors{top: contactsHeading.bottom; topMargin: 20; bottom: parent.bottom; horizontalCenter: parent.horizontalCenter}
            width: parent.width-10; height: parent.height-contactsHeading.height
            contentHeight: listView.contentHeight
            clip: true
            interactive: true
            ListView{
                anchors.fill: parent
                id: listView
                clip: true
                interactive:false
                model: ContactsModel {
                    id:contactsModel }
                spacing: 5
                delegate: ListViewDelegate{}

            }
        }
}
