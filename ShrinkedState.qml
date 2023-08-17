import QtQuick

State{
    name:"shrinked"
    PropertyChanges {
        target: contactRect
        height: 80;
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
