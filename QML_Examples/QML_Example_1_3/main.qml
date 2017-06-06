import QtQuick 2.7
import QtQuick.Controls 1.5
import QtQuick.Dialogs 1.2
//import MyText 1.0

Rectangle {
    ApplicationWindow {
        visible: true
        width: 640
        height: 480
        title: qsTr("Hello World")

        Component.onCompleted: {
            testText.textContentChanged.connect(setTextField);
        }

        function setTextField () {
            textField1.text = testText.textContent;
        }

        menuBar: MenuBar {
            Menu {
                title: qsTr("File")
                MenuItem {
                    text: qsTr("&Open")
                    onTriggered: console.log("Open action triggered");
                }
                MenuItem {
                    text: qsTr("Exit")
                    onTriggered: Qt.quit();
                }
            }

            Menu {
                title: qsTr("Test")
                MenuItem {
                    text: qsTr("TestDialog")
                    onTriggered: dialog1.open();
                }
            }
        }

        MainForm {
            anchors.fill: parent
            button1.onClicked: messageDialog.show(qsTr("Button 1 pressed"))
            button2.onClicked: messageDialog.show(qsTr("Button 2 pressed"))
            textField {
                id: textField1
                //text: testText1.textContent
            }
        }

        //TestText {
        //    id: testText1
        //}

        MessageDialog {
            id: messageDialog
            title: qsTr("May I have your attention, please?")

            function show(caption) {
                messageDialog.text = caption;
                messageDialog.open();
            }
        }

        Dialog {
            id: dialog1
            //onAccepted: testText.setTextContent(qsTr("result"));
            //both works
            onAccepted: testText.textContent = "result";
        }
    }
}
