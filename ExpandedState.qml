import QtQuick

State{
    name:"expanded"
//                    when: contactRect.ListView.isCurrentItem
    PropertyChanges {
        target: contactRect
        height: 200;
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
}
