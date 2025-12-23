import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    width: 640
    height: 360
    visible: true
    title: "Hello QML"
    color: "#0f1218"

    Label {
        anchors.centerIn: parent
        text: "Hello, QML!"
        color: "#e7eef5"
        font.pixelSize: 28
        font.weight: Font.DemiBold
    }
}