/*
 *  Copyright 2011 Ruediger Gad
 *
 *  This file is part of VUMeter.
 *
 *  VUMeter is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  VUMeter is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with VUMeter.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 1.1
import com.meego 1.0
import vumeter 1.0

Dialog {
    id: aboutDialog

    anchors.fill: parent

    content:Item {
      anchors.fill: parent

      Text {id: homepage;
          text: "VUMeter - Version 0.2.1<br /><img src=\"vumeter_100x100.png\" />";
          textFormat: Text.RichText;
          font.pixelSize: 25; horizontalAlignment: Text.AlignHCenter; color: "white";
          anchors.horizontalCenter: parent.horizontalCenter; anchors.bottom: description.top; anchors.bottomMargin: 8 }

      Text {id: description; text: "A Simple Volume Meter"; font.pixelSize: 25; font.bold: true; anchors.horizontalCenter: parent.horizontalCenter; anchors.bottom: author.top; anchors.bottomMargin: 12; color: "white"}

      Text {id: author;
          text: "Author: Ruediger Gad - <a href=\"mailto:r.c.g@gmx.de\" style=\"text-decoration:none; color:#78bfff\" >r.c.g@gmx.de</a>";
          textFormat: Text.RichText;
          onLinkActivated: { Qt.openUrlExternally(link); }
          font.pixelSize: 20; anchors.centerIn: parent; color: "lightgray"; horizontalAlignment: Text.AlignHCenter}

      Text {id: license;
          text: "VUMeter is free software: you can redistribute it and/or modify "
            + "it under the terms of the GNU General Public License as published by "
            + "the Free Software Foundation, either version 3 of the License, or "
            + "(at your option) any later version.<br />"
            + "VUMeter is distributed in the hope that it will be useful, "
            + "but WITHOUT ANY WARRANTY; without even the implied warranty of "
            + "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
            + "GNU General Public License for more details.<br />"
            + "You should have received a copy of the GNU General Public License "
            + "along with VUMeter.  If not, see <a href=\"http://www.gnu.org/licenses\" style=\"text-decoration:none; color:#78bfff\" >http://www.gnu.org/licenses</a>.";
          textFormat: Text.RichText;
          onLinkActivated: { Qt.openUrlExternally(link); }
          font.pixelSize: 18; anchors.horizontalCenter: parent.horizontalCenter; anchors.top: author.bottom; anchors.topMargin: 12; width: parent.width; color: "lightgray"; horizontalAlignment: Text.AlignHCenter; wrapMode: Text.Wrap}
    }
}
