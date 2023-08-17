import QtQuick

Rectangle{
            id: contactRect
            /*width: root.width; height:80;*/ color:"#F1DEC9"
            state:  "shrinked"
            radius: 5; width: root.width-10
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
                ExpandedState{},
                ShrinkedState{}
                    ]
            transitions: [
                Transition {
                    from: "shrinked"; to: "expanded"
                    PropertyAnimation{
                        target: contactRect; properties: "height"
                        duration: 500; easing.type: Easing.InCirc
                    }

                    SequentialAnimation{
                        AnchorAnimation{
                            targets: [nameLabel,/*numberLabel,*/nameIcon]
                            duration: 800; easing.type: Easing.InCubic
                        }
                        PropertyAnimation{
                           targets: [numberLabel,nameLabel]; properties: "font.pixelSize,anchors.bottomMargin"
                           duration: 1000; easing.type: Easing.OutBack
                        }
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
                        target: contactRect; properties: "height"
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