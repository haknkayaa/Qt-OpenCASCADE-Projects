import QtQuick 2.2
        import QtQuick.Window 2.2
        import QtQuick.Controls 2.2
        import opencascade 7.4

        ApplicationWindow {
        visible: true
        width: 640
        height: 480
        title: "OpenCascade Qt Viewer Demo"

        Occt {

        }

        Row {

        Button {
        id: button0
        text: "print text into console"
        onClicked: {
        occ.printtext();
        }
        }

        Button {
        id: button1
        text: "demo scene"
        onClicked: {
        occ.createdemoscene();
        }
        }

        Button {
        id: button2
        text: "robot scene"
        onClicked: {
        occ.createrobotscene();
        }
        }

        Button {
        id: button3
        text: "clear"
        onClicked: {
        occ.clear();
        }
        }
        }
        }
