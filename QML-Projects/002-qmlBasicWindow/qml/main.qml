import QtQuick 2.7
        import QtQuick.Controls 2.0
        import QtQuick.Layouts 1.0
        import Hakan.Kaya.BackEndClass 1.0
        ApplicationWindow
        {
        width: 640
        height: 480
        visible: true

        BackEndClass{
        id: myBackEndClass
        }

        ColumnLayout{
        spacing: 2
        anchors.fill: parent
        Rectangle {
        color: "green"
        Layout.fillWidth: true
        height: 40
        Text {
        text: "QML/C++ Integration Example"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        width:parent.width
        height:parent.height
        font.family: "Helvetica"
        font.pointSize: 12
        color: "white"
        }
        }
        // Mesaj satirir
        RowLayout{
        Layout.fillWidth: true
        Rectangle {
        color: "red"
        width: 200
        height: 40
        Text {
        text: "Message: "
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        width:parent.width
        height:parent.height
        font.family: "Helvetica"
        font.pointSize: 12
        color: "white"
        }
        }
        Rectangle {
        color: "brown"
        Layout.fillWidth: true
        height: 40
        Text {
        text: myBackEndClass.message
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        width:parent.width
        height:parent.height
        font.family: "Helvetica"
        font.pointSize: 12
        color: "white"
        }
        }
        }
        // Sayac satiri
        RowLayout{
        Layout.fillWidth: true
        Rectangle {
        color: "red"
        width: 200
        height: 40
        Text {
        text: "Counter: "
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        width:parent.width
        height:parent.height
        font.family: "Helvetica"
        font.pointSize: 12
        color: "white"
        }
        }
        Rectangle {
        color: "brown"
        Layout.fillWidth: true
        height: 40
        Text {
        id:counterText
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        width:parent.width
        height:parent.height
        font.family: "Helvetica"
        font.pointSize: 12
        color: "white"
        }
        }
        Button{
        text:"Update Counter"
        onClicked:{
        counterText.text = String(myBackEndClass.incrementCounter())
        }
        }
        Button{
        text:"Reset Counter"
        onClicked:{
        myBackEndClass.resetCounter()
        counterText.text = String(myBackEndClass.incrementCounter())
        }
        }
        }
        Rectangle{
        color:"blue"
        Layout.fillHeight: true
        Layout.fillWidth: true
        }
        }
        }