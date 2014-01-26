#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>
#include <QtQml>

#include "game.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Game game;

    qmlRegisterType<Game>("WFLetterCounter",1,0,"Game");

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);

    QQmlContext* c = view.rootContext();
    c->setContextProperty("game", &game);

    view.setSource(QUrl("qrc:///main.qml"));
    view.show();

    return app.exec();
}
