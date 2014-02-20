import QtQuick 2.2
import QtQuick.Layouts 1.1

import WFLetterCounter 1.0

Rectangle {

   id: gameScreen

   width: 800
   height: 480
   color: "#ff333333"

   state: "Menu"

   states: [
      State {
         name: "Menu"
//         PropertyChanges { target: root; color: "red" }
      },
      State {
         name: "Game"
//         PropertyChanges { target: root; color: "blue" }
      }
   ]

   ListView {
      id: gameList
      focus: true
      anchors.fill: parent
      snapMode: ListView.SnapOneItem
//      highlightRangeMode: ListView.StrictlyEnforceRange

      highlightFollowsCurrentItem: true
      highlightMoveDuration: 200

//      preferredHighlightBegin: 0
//      preferredHighlightEnd: 0

      orientation: ListView.Vertical
      model: gamemanager
      delegate: GameView {
         id: gameViewDelegate
         gameObject: game
         width: gameList.width
      }

//      highlight: Rectangle {
//         color: "blue"
//      }

      onCountChanged: {
         console.log("count changed, it's now at " + count + " curridx " + currentIndex);
         currentIndex = count-1;
      }
      onCurrentIndexChanged: {
         console.log("current index is " + currentIndex);
      }

      Keys.onPressed: {
         console.log("main listview key " + event.key);
         event.accepted = true;
         switch(event.key)
         {
         case Qt.Key_PageDown:
            incrementCurrentIndex();
            return;
         case Qt.Key_PageUp:
            decrementCurrentIndex();
            return;
         case Qt.Key_Insert:
            gamemanager.addNewGame();
            return;
         case Qt.Key_Escape:
            if(gameList.currentItem)
               gamemanager.deleteGame(gameList.currentIndex);
            return;
         case Qt.Key_F1:
            console.log("F1");
            gameScreen.state = "Menu";
            return;
         case Qt.Key_F2:
            console.log("F2");
            gameScreen.state = "Game";
            return;
         }
         event.accepted = false;
      }
   }
   onStateChanged: {
      console.log("main state now " + state);
   }
}
