import QtQuick

Rectangle{
    id: contactsHeading
    anchors.top: parent.top
    width: parent.width; height: 90; color:"#8D7B68"
    Text{
        anchors.centerIn: parent; text: "CONTACTS";
        font{pixelSize: 30; bold:true}
    }
}
