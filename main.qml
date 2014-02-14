import QtQuick 2.2
import QtQuick.Layouts 1.1

import WFLetterCounter 1.0

Rectangle {
   width: 640
   height: 480

   focus: true

   function addLetter(letter) {
      return game.addLetter(letter);
   }
   function backspace() {
      game.backspace();
   }
   function addWord() {
      game.addWord();
   }
   function deleteWord() {
      game.deleteWord();
   }

   Keys.onPressed: {
      event.accepted = true;
      switch(event.key)
      {
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
            anchors.margins: 5
            color: letterman.bgColor
            radius: 5
         }

         Text {
            anchors.centerIn: parent
            text: letter + " " + remaining
            font.pixelSize: 25
         }

         MouseArea {
            anchors.fill: parent
            onClicked: addLetter(letter);
         }
      }
   }

   Text {
      anchors.centerIn: parent
      font.pixelSize: 15
      text: "Total: " + game.remTotal
            + " Vowels: " + game.remVowels
            + " Consonants: " + game.remConsonants
            + " Wildcards: " + game.remWildcards
   }

   ListView {

      anchors.top: parent.top
      anchors.left: parent.left
      anchors.right: parent.right
      anchors.bottom: keyboard.top

      model: game.words
      delegate: Text {
         text: (index+1) + ": " + word
         font.pixelSize: 25
      }
      highlightFollowsCurrentItem: true
      highlight: Rectangle {
         color: "green"
         width: ListView.width
         height: 30
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
      height: cellHeight*3

      cellWidth: width/11.5
      cellHeight: cellWidth

      model: game.letters
      delegate: letterDelegate
   }
}
