/*
 *  Copyright 2010, 2011 Ruediger Gad
 *
 *  This file is part of vumeter.
 *
 *  vumeter is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  vumeter is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with vumeter.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 1.1
import com.meego 1.0
import vumeter 1.0

PageStackWindow {
    id: appWindow

    property VolumeMeter vuMeter;
    property int vuMeterStyle: 0

    initialPage: mainPage

    PulseAudio {
        id: audio

        onAudioDataReceived: {
            var vol = 0;
            if(dbButton.checked){
                vol = 100 + audio.linearVolToDb(data);
            }else{
                vol = 100 * data;
            }
            vol = parseInt(vol);

            if(vuMeter !== null){
                vuMeter.updateValue(vol);
            }

            curVol.text = vol;
            if(maxVol.text < vol){
                maxVol.text = vol;
            }
        }
    }

    SelectionDialog {
        id: sourceSelectionDialog
        titleText: "Select Source"
        selectedIndex: 0

        model: ListModel {
            ListElement { name: "Microphone" }
            ListElement { name: "Bluetooth" }
            ListElement { name: "Monitor" }
        }

        onAccepted: {
            mainPage.connectAudioInput();
        }
    }

    ToolBarLayout {
        id: commonTools
        visible: true
        ButtonRow {
            id: unitButtonRow

            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.right: sourceSelectionButton.left
            anchors.rightMargin: 0

            Button {
                id: dbButton
                text: "dB"

                onClicked: {
                    vuMeter.setDbUnit(true);
                    maxVol.text = "0";
                }
            }
            Button {
                id: linerButton
                text: "Linear"

                onClicked: {
                    vuMeter.setDbUnit(false);
                    maxVol.text = "0";
                }
            }
        }
        Button{
            id: sourceSelectionButton
            text: sourceSelectionDialog.model.get(sourceSelectionDialog.selectedIndex).name

            anchors.right: menuButton.left
            anchors.rightMargin: 0

            width: 200

            onClicked: sourceSelectionDialog.open()
        }
        ToolIcon { id: menuButton; platformIconId: "toolbar-view-menu";
             anchors.right: parent===undefined ? undefined : parent.right
             onClicked: (myMenu.status == DialogStatus.Closed) ? myMenu.open() : myMenu.close()
        }
    }

    AboutDialog{
        id: aboutDialog
    }

    Menu {
        id: myMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem { text: "About"; onClicked: aboutDialog.open() }
        }
    }

    Page {
        id: mainPage
        tools: commonTools

        Component.onCompleted: {
            setVuMeter(vuMeterStyle);
            audio.connectAudioStream("source.hw0");
            vuMeter.setDbUnit(true);
        }

        onWidthChanged: {
            if(vuMeter !== null){
                vuMeter.resize(parent.width, parent.height);
            }
        }


        function connectAudioInput() {
            switch(sourceSelectionDialog.selectedIndex){
            case 0:
                audio.connectAudioStream("source.hw0");
                resetValues();
                break;
            case 1:
                audio.connectAudioStream("source.hw1");
                resetValues();
                break;
            case 2:
                audio.connectAudioStream("sink.hw0.monitor");
                resetValues();
                break;
            }
        }

        function resetValues(){
            vuMeter.updateValue(0);
            curVol.text = "0";
            maxVol.text = "0";
        }

        function setVuMeter(style) {
            if(vuMeter !== null){
                vuMeter.destroy();
                vuMeter = null;
            }

            switch(style){
            case 0:
                vuMeter = Qt.createQmlObject('import vumeter 1.0; AnalogMeter{}', vuMeterItem, "AnalogMeter");
                break;
            case 1:
                vuMeter = Qt.createQmlObject('import vumeter 1.0; HorizontalVolumeBar{}', vuMeterItem, "HorizontalVolumeBar");
                vuMeter.setUseColoredScale(false);
                break;
            case 2:
                vuMeter = Qt.createQmlObject('import vumeter 1.0; VolumeLineChart{}', vuMeterItem, "VolumeLineChart");
                vuMeter.setUseColoredScale(false);
                break;
            default:
                console.log("Unknown VU Meter Style: " + style);
                break;
            }

            vuMeter.setDbUnit(dbButton.checked);
            vuMeter.resize(parent.width, parent.height);
        }

        Grid {
            id: grid
            anchors.fill: parent
            spacing: 2
            columns: 1
            rows: 2

            Item {
                id: vuMeterItem
                anchors.top: parent.top
                anchors.bottom: row2.top
                anchors.bottomMargin: 8
                width: parent.width

                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        vuMeterStyle = ++vuMeterStyle % 3
                        mainPage.setVuMeter(vuMeterStyle)
                    }
                }
            }

            Row {
                id: row2

                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 8
                height: curVolLabel.height

                Text {
                    id: curVolLabel
                    text: "Cur. Volume: "

                    font.pixelSize: 30

                    anchors.left: parent.left
                    anchors.leftMargin: 0
                }

                Text {
                    id: curVol
                    text: "0"

                    font.pixelSize: 30

                    anchors.left: curVolLabel.right
                    anchors.leftMargin: 0
                }

                Text {
                    id: maxVolLabel
                    text: "<a href=\"http://foo\">Max. Volume:</a> "
    
                    anchors.right: maxVol.left
                    anchors.rightMargin: 0

                    textFormat: Text.RichText;
                    font.pixelSize: 30

                    onLinkActivated: {
                        console.log("Resetting maximum volume.")
                        maxVol.text = "0";
                    }
                }

                Text {
                    id: maxVol
                    text: "0"

                    font.pixelSize: 30

                    anchors.right: parent.right
                    anchors.rightMargin: 0
                }
            }
        }
    }
}
