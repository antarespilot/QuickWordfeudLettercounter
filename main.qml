import QtQuick 2.2
import QtQuick.Layouts 1.1

import WFLetterCounter 1.0

Rectangle {

   width: 800
   height: 480
   color: "#ff333333"

   ListView {
      id: gameList
      focus: true
      anchors.fill: parent
      snapMode: ListView.SnapOneItem

      highlightFollowsCurrentItem: true
      highlightRangeMode: ListView.StrictlyEnforceRange
      highlightMoveDuration: 500

      preferredHighlightBegin: 0
      preferredHighlightEnd: width

      orientation: ListView.Horizontal
      model: gamemanager
      delegate: GameView {
         gameObject: gamemanager.game(index)
         width: gameList.width
         height: gameList.height
      }

      Keys.onPressed: {
         event.accepted = true;
         switch(event.key)
         {
         case Qt.Key_PageDown:
            incrementCurrentIndex();
            return;
         case Qt.Key_PageUp:
            decrementCurrentIndex();
            return;
         }
         event.accepted = false;
      }
   }
}
