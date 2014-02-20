import QtQuick 2.2
import QtQuick.Layouts 1.1

import WFLetterCounter 1.0

Rectangle {

   id: gameScreen

   width: 800
   height: 480
   color: "#ff333333"

   ListView {
      id: gameList
      focus: true
      anchors.fill: parent
//      snapMode: ListView.SnapOneItem
//      highlightRangeMode: ListView.StrictlyEnforceRange

//      highlightFollowsCurrentItem: true
      highlightMoveDuration: 100
      highlightResizeDuration: 100

//      preferredHighlightBegin: 0
//      preferredHighlightEnd: 0

      orientation: ListView.Vertical
      model: gamemanager
      delegate: GameView {
         id: gameViewDelegate
         gameObject: game
         listWidth: gameList.width
         listHeight: gameList.height
      }

      highlight: Rectangle {
         color: "#33ffffff"
      }

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
         case Qt.Key_Plus:
            gamemanager.addNewGame();
            return;
         case Qt.Key_Escape:
            if(gameList.currentItem)
               gamemanager.deleteGame(gameList.currentIndex);
            return;
         }
         event.accepted = false;
      }
   }
   onStateChanged: {
      console.log("main state now " + state);
   }
}
