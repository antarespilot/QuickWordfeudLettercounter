import QtQuick 2.2
import QtQuick.Layouts 1.1

import WFLetterCounter 1.0

Rectangle {
   width: 640
   height: 480

   Component {
      id: letterDelegate
      Item {
         id: letterman

         width: GridView.view.cellWidth
         height: GridView.view.cellWidth

         property int rem: remaining
         property color bgColor: remaining > 0 ? "green" : "gray"

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
      }
   }

   ColumnLayout {
      anchors.fill: parent

      Text {
         Layout.fillWidth: true

         font.pixelSize: 25
         text: "Total: " + game.remTotal
               + "\nVowels: " + game.remVowels
               + "\nConsonants: " + game.remConsonants
               + "\nWildcards: " + game.remWildcards
      }

      GridView {
         Layout.fillWidth: true
         Layout.fillHeight: true

         cellWidth: 64
         cellHeight: 64

         model: game
         delegate: letterDelegate
      }

      Text {
         Layout.fillWidth: true
         Layout.minimumHeight: 30

         text: "Enter word below. Press return/enter to apply."
      }

      Item {
         Layout.fillWidth: true
         Layout.minimumHeight: 30

         Rectangle {
            anchors.fill: parent
            color: wordInput.wordStatus === Game.EmptyWord ? "gray" :
                   wordInput.wordStatus === Game.InvalidWord ? "red" :
                   wordInput.wordStatus === Game.ValidWord ? "green" :
                                                             "orange"
         }

         TextInput {
            anchors.fill: parent

            id: wordInput

            font.pixelSize: 25

            property int wordStatus: Game.EmptyWord

            onTextChanged: {
               wordStatus = game.wordValidity(text);
            }
            onAccepted: {
               game.applyWord(text);
               text = "";
            }
         }
      }

      Text {
         Layout.fillWidth: true
         Layout.minimumHeight: 30

         text: (wordInput.wordStatus === Game.EmptyWord ? "" :
               wordInput.wordStatus === Game.InvalidWord ? "Not enough letters left for that" :
               wordInput.wordStatus === Game.ValidWord ? "Good to go" :
                                                         "Will use wildcards for that")
      }

   }
}
