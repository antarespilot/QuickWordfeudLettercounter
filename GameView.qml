import QtQuick 2.2
import QtQuick.Layouts 1.1

import WFLetterCounter 1.0

FocusScope {

   id: gameView

   property QtObject gameObject

   property int listWidth: 800
   property int listHeight: 480
   property int minHeight: 50
   width: listWidth

   property bool isCurrentItem: ListView.isCurrentItem
   onIsCurrentItemChanged: {
      if(!isCurrentItem) {
         state = "Menu";
      }
   }

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
      console.log("gameview key " + event.key);

      event.accepted = true;
      switch(event.key)
      {
      case Qt.Key_Escape:
         gameView.state = "Menu";
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

   state: "Menu"
   onStateChanged: {
      console.log("State now " + state);
   }

   states: [
      State {
         name: "Menu"
         PropertyChanges {
            target: gameView;
            height: minHeight
            width: listWidth
         }
         PropertyChanges { target: keyboard; opacity: 0; }
         PropertyChanges { target: wordlist; opacity: 0; }
         PropertyChanges { target: gameView.ListView.view; interactive: true }
         PropertyChanges { target: nameInput; readOnly: true;  activeFocusOnPress: false; }
      },
      State {
         name: "Game"
         PropertyChanges {
            target: gameView;
            height: listHeight
            width: listWidth
         }
         PropertyChanges { target: keyboard; opacity: 1; }
         PropertyChanges { target: wordlist; opacity: 1; }
         PropertyChanges { target: gameView.ListView.view; interactive: false }
         PropertyChanges { target: nameInput; readOnly: false; activeFocusOnPress: true;  }
      }
   ]

   transitions: Transition {
      NumberAnimation { properties: "height,opacity"; easing.type: Easing.InOutQuad }
   }

   MouseArea {
      anchors.fill: gameView
      onDoubleClicked: {
         if(gameView.state === "Menu")
            gameView.state = "Game";
         else
            gameView.state = "Menu";

         gameView.ListView.view.currentIndex = index
         nameInput.focus = false;
      }
      onClicked: {
         nameInput.focus = false;
         gameView.ListView.view.currentIndex = index
      }
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
      anchors.left: parent.left
      anchors.right: parent.right
      anchors.top: parent.top
      height: minHeight - 10
      anchors.margins: 5

      color: "#111111"
      radius: 5


      RowLayout {
         id: statLayout
         anchors.fill: parent
         anchors.margins: 5

         property int largeFont: 22
         property int smallFont: 10

         TextInput {
            id: nameInput
            Layout.fillWidth: true
            text: gameObject.name
            font.pixelSize: statLayout.largeFont
            color: "white"
            onTextChanged: gameObject.name = text;
            maximumLength: 32
         }

         Text {
            text: "Total"
            font.pixelSize: statLayout.smallFont
            color: "white"
         }

         Text {
            text: gameObject.remTotal
            font.pixelSize: statLayout.largeFont
            color: "white"
         }

         Text {
            text: "Vowels"
            font.pixelSize: statLayout.smallFont
            color: "white"
         }

         Text {
            text: gameObject.remVowels
            font.pixelSize: statLayout.largeFont
            color: "white"
         }

         Text {
            text: "Consonants"
            font.pixelSize: statLayout.smallFont
            color: "white"
         }

         Text {
            text: gameObject.remConsonants
            font.pixelSize: statLayout.largeFont
            color: "white"
         }

         Text {
            text: "Wildcards"
            font.pixelSize: statLayout.smallFont
            color: "white"
         }

         Text {
            text: gameObject.remWildcards
            font.pixelSize: statLayout.largeFont
            color: "white"
         }
      }
   }

   ListView {

      id: wordlist
      anchors.top: stats.bottom
      anchors.left: parent.left
      anchors.right: stats.left
      anchors.bottom: keyboard.top
      anchors.margins: 5

      model: gameObject ? gameObject.words : null

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

      model: gameObject ? gameObject.letters : null
      delegate: letterDelegate

      interactive: false
      visible: opacity > 0
   }
}
