import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    id: window
    width: 960
    height: 540
    visible: true
    title: "QML Terminal"
    color: "#0b0e12"

    property var history: []
    property int historyIndex: -1

    function appendLine(text) {
        outputArea.text += text + "\n"
        outputArea.cursorPosition = outputArea.length
    }

    function resetHistoryIndex() {
        historyIndex = history.length
    }

    function handleOutput(chunk) {
        var normalized = chunk.replace(/\r\n/g, "\n").replace(/\r/g, "\n")
        outputArea.text += normalized
        outputArea.cursorPosition = outputArea.length
    }

    Connections {
        target: terminalBackend
        function onOutputReceived(text) {
            handleOutput(text)
        }
    }

    Rectangle {
        id: frame
        anchors.fill: parent
        radius: 12
        color: "#0f141b"
        border.color: "#1e2833"
        border.width: 1
        anchors.margins: 20

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 18
            spacing: 12

            Rectangle {
                Layout.fillWidth: true
                height: 36
                radius: 8
                color: "#151d26"
                border.color: "#243242"

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 8
                    spacing: 10

                    Rectangle { width: 12; height: 12; radius: 6; color: "#ff5f56" }
                    Rectangle { width: 12; height: 12; radius: 6; color: "#ffbd2e" }
                    Rectangle { width: 12; height: 12; radius: 6; color: "#27c93f" }

                    Label {
                        text: "QML Terminal"
                        color: "#d6dee6"
                        font.pixelSize: 14
                        font.weight: Font.DemiBold
                        Layout.leftMargin: 12
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                radius: 10
                color: "#0b1016"
                border.color: "#1c2632"

                ScrollView {
                    anchors.fill: parent
                    anchors.margins: 12

                    TextArea {
                        id: outputArea
                        text: ""
                        readOnly: true
                        selectByMouse: true
                        persistentSelection: true
                        color: "#c7d4e0"
                        font.family: "Consolas"
                        font.pixelSize: 14
                        wrapMode: TextEdit.WrapAnywhere
                        background: Rectangle { color: "transparent" }
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                height: 48
                radius: 8
                color: "#0f141b"
                border.color: "#1c2632"

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 8
                    spacing: 8

                    Label {
                        text: ">"
                        color: "#7cd1ff"
                        font.pixelSize: 16
                        font.family: "Consolas"
                    }

                    TextField {
                        id: inputField
                        Layout.fillWidth: true
                        placeholderText: "Type a command and press Enter"
                        color: "#e7eef5"
                        font.pixelSize: 14
                        font.family: "Consolas"
                        background: Rectangle {
                            radius: 6
                            color: "#121923"
                            border.color: "#223043"
                        }

                        Keys.onPressed: function(event) {
                            if (event.key === Qt.Key_Up) {
                                if (historyIndex > 0) {
                                    historyIndex -= 1
                                    text = history[historyIndex]
                                    cursorPosition = text.length
                                }
                                event.accepted = true
                            } else if (event.key === Qt.Key_Down) {
                                if (historyIndex < history.length - 1) {
                                    historyIndex += 1
                                    text = history[historyIndex]
                                } else {
                                    historyIndex = history.length
                                    text = ""
                                }
                                cursorPosition = text.length
                                event.accepted = true
                            } else if (event.key === Qt.Key_V && (event.modifiers & Qt.ControlModifier)) {
                                text += Qt.application.clipboard.text
                                cursorPosition = text.length
                                event.accepted = true
                            }
                        }

                        onAccepted: {
                            var trimmed = text.trim()
                            if (trimmed.length > 0) {
                                history.push(trimmed)
                                resetHistoryIndex()
                                terminalBackend.sendInput(trimmed + "\r\n")
                            }
                            text = ""
                        }
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        appendLine("Starting shell...")
        terminalBackend.start()
    }
}
