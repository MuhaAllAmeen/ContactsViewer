import QtQuick

State{
    name:"expanded"
//                    when: contactRect.ListView.isCurrentItem


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
