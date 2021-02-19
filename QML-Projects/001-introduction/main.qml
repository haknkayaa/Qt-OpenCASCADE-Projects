import QtQuick 2.0
        import QtQuick.Controls 2.1

        Button {
        id: buttonTest
        objectName: "buttonTest"

        signal clickedButton(string str)
        onClicked: {
        buttonTest.clickedButton("clicked !")
        }
        }