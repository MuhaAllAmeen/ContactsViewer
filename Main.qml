import QtQuick
import QtQuick.Window
import Contacts 1.0
Window {
    width: 640
    height: 480
    visible: true
    id:root
    color:"#C8B6A6"

    Heading{
    id:contactsHeading}

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
                id:contactsModel
                list : contactsList }
            spacing: 5
            delegate: ListViewDelegate{}
            Connections{
                target: contactsModel
                function onModelAboutToBeReset(){
                    console.log("model Reset")
                }
                function onModelReset(){
                    console.log("model Reset")
                }
            }
        }
    }
}
