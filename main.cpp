#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>
#include <QtQml>

#include "game.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Game game;

    qmlRegisterUncreatableType<Game>("WFLetterCounter",1,0,"Game","Computer says NO");
    qmlRegisterUncreatableType<Letters>("WFLetterCounter",1,0,"Letters","Computer says NO");
    qmlRegisterUncreatableType<Words>("WFLetterCounter",1,0,"Words","Computer says NO");

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);

    QQmlContext* c = view.rootContext();
    c->setContextProperty("game", &game);

    view.setSource(QUrl("qrc:///main.qml"));
    view.show();

    return app.exec();
}
