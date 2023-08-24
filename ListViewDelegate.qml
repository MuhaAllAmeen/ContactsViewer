import QtQuick

Rectangle{
            id: contactRect
            color:"#F1DEC9"
            state:  "shrinked"
            radius: 5; width: root.width-10; height: state=="shrinked" ? 80 : 200
            Rectangle{
                id: nameIcon
//                anchors{left:parent.left; leftMargin: 10; verticalCenter: parent.verticalCenter}
                anchors.leftMargin: 5
                width: 50; height:50; radius: 30; color: "#C8B6A6"
                Text{
                    id: nameFirstLetter
                    anchors.centerIn: parent; text: name[0]; color: "white"
                    font.pixelSize: contactRect.state=="shrinked"?20:40
                }
            }

            Text{
                id:nameLabel
                anchors.leftMargin: 5; anchors.bottomMargin: contactRect.state=="shrinked"? 0: 5
                text: name; font.pixelSize:contactRect.state=="shrinked"?17:25
                color:"#8D7B68"; font.bold: true
            }
            Text{
                id:numberLabel
                anchors.rightMargin: 5; anchors.bottomMargin: contactRect.state=="shrinked"? 0: 5
                color:"#8D7B68"
                text: number; font.pixelSize:contactRect.state=="shrinked"?17:25
            }
            states:[
                State{
                    name:"expanded"
                    PropertyChanges{
                           target:nameIcon
                           height:90; width:90; radius:90; anchors.topMargin: 10
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
                },
                State{
                    name:"shrinked"
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
                    ]
            transitions: [
                Transition {
                    from: "shrinked"; to: "expanded"
                    PropertyAnimation{
                        target: contactRect; properties: "height"
                        duration: 500; easing.type: Easing.InCirc
                    }

                    ParallelAnimation{
                        AnchorAnimation{
                            targets: [nameLabel,numberLabel,nameIcon]
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
                anchors.fill: parent;
                onClicked: {
                    contactRect.ListView.view.currentIndex = index
                    console.log(contactRect.ListView.view.currentIndex)
                    contactRect.state === "shrinked" ? contactRect.state="expanded":contactRect.state="shrinked"
               }
            }

        }
