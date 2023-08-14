import QtQuick
import QtQuick.Window

Window {
    width: 640
    height: 480
    visible: true

    ListModel{
        id:contactsModel
        ListElement{
            name:"john"; number:"0556768888"
        }
    }

    ListView{
        anchors.fill: parent
        id: listView
        model: contactsModel
        spacing: 5
        delegate: Rectangle{
            width: parent.width; height: 50; color:"beige"
            Text{
                anchors.centerIn: parent; text: number
            }
        }
    }
}
