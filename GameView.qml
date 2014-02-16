import QtQuick 2.2
import QtQuick.Layouts 1.1

import WFLetterCounter 1.0

Item {

   id: gameView

   property QtObject gameObject

   width: 800
   height: 480

   focus: true

   function addLetter(letter) {
      return gameObject.addLetter(letter);
   }
   function backspace() {
      gameObject.backspace();
   }
   function addWord() {
      gameObject.addWord();
   }
   function deleteWord() {
      gameObject.deleteWord();
   }

   Keys.onPressed: {
      event.accepted = true;
      switch(event.key)
      {
      case Qt.Key_PageDown:
      case Qt.Key_PageUp:
         event.accepted = false;
         return;
      case Qt.Key_Backspace:
         backspace();
         return;
      case Qt.Key_Return:
      case Qt.Key_Enter:
         addWord();
         return;
      case Qt.Key_Delete:
         deleteWord();
         return;
      }

      if(addLetter(event.text))
         return;

      event.accepted = false;
   }

   Component {
      id: letterDelegate
      Item {
         id: letterman

         width: GridView.view.cellWidth
         height: GridView.view.cellWidth

         property int rem: remaining
         property color bgColor: capacity === 0 ? "gray" :
                                 remaining > 1 ? "green" :
                                 remaining === 1 ? "orange" : "red"

         Rectangle {
            anchors.fill: parent
            anchors.margins: 2
            color: letterman.bgColor
            border.color: "black"
            border.width: 2
            radius: 5
         }

         Text {
            anchors.centerIn: parent
            text: letter + " " + remaining
            font.pixelSize: 35
         }

         MouseArea {
            anchors.fill: parent
            onClicked: addLetter(letter);
         }
      }
   }

   Rectangle {
      id: stats
      anchors.right: parent.right
      anchors.top: parent.top
      anchors.bottom: keyboard.top
      anchors.margins: 5
      width: 150
      color: "transparent"
      border.color: "black"
      border.width: 2
      radius: 5

      Text {
         anchors.fill: parent
         anchors.margins: 5
         font.pixelSize: 22
         color: "white"
         text: "Total left:<br><b>" + gameObject.remTotal
               + "</b><br>Vowels:<br><b>" + gameObject.remVowels
               + "</b><br>Consonants:<br><b>" + gameObject.remConsonants
               + "</b><br>Wildcards:<br><b>" + gameObject.remWildcards
         textFormat: Text.StyledText
      }
   }

   ListView {

      id: wordlist
      anchors.top: parent.top
      anchors.left: parent.left
      anchors.right: stats.left
      anchors.bottom: keyboard.top
      anchors.margins: 5
      clip: true

      model: gameObject.words

      delegate: Item {
         width: wordlist.width
         height: 30
         id: wordDelegate

         property bool isCurrentItem: ListView.isCurrentItem

         Text {
            id: wordIndex
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.margins: 5
            text: (index+1)
            font.pixelSize: 25
            width: 40
            height: 30
            color: "white"
         }

         Text {
            id: theword
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: wordIndex.right
            anchors.margins: 5
            text: word
            font.pixelSize: 25
            color: "white"
         }

         Rectangle {
            id: cursor
            anchors.verticalCenter: parent.verticalCenter
            height: 30
            anchors.left: theword.right
            color: "white"
            width: 5
            radius: 5

            ColorAnimation on color { from: "white"; to: "black"; duration: 500; loops: Animation.Infinite; running: cursor.visible }
            visible: wordDelegate.isCurrentItem
         }

         Rectangle {
            id: deleteButton
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            width: 25
            height: 25
            color: "red"
            radius: 5
            visible: wordDelegate.isCurrentItem && index > 0
            Text {
               anchors.centerIn: parent
               color: "white"
               text: "X"
            }
            MouseArea {
               anchors.fill: parent
               onClicked: deleteWord();
            }
         }
      }

      onCountChanged: {
         currentIndex = count-1
      }

   }

   GridView {
      id: keyboard
      anchors.bottom: parent.bottom
      anchors.left: parent.left
      anchors.right: parent.right
      anchors.margins: 5
      height: cellHeight*3

      cellWidth: width/11.0001
      cellHeight: cellWidth

      model: gameObject.letters
      delegate: letterDelegate

      interactive: false
   }
}
