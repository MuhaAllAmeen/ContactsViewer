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
        anchors{top: contactsHeading.bottom; topMargin: 10; bottom: parent.bottom; bottomMargin: 10; horizontalCenter: parent.horizontalCenter}
        width: parent.width-10; height: parent.height-contactsHeading.height
        clip: true
        model: ContactsModel {
            id:contactsModel
            list : contactsList }
        spacing: 5
        delegate: ListViewDelegate{}
        }
}
