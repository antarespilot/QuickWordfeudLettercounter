#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>
#include <QtQml>

#include "gamemanager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    app.setApplicationName("LetterCounter");
    app.setApplicationVersion("1.0");
    app.setApplicationDisplayName("LetterCounter");
    app.setOrganizationName("Silent Wings AS");
    app.setOrganizationDomain("www.silentwings.no");

    GameManager game;

    qmlRegisterUncreatableType<GameManager>("WFLetterCouter",1,0,"GameManager","Computer says NO");
    qmlRegisterUncreatableType<Game>("WFLetterCounter",1,0,"Game","Computer says NO");
    qmlRegisterUncreatableType<Letters>("WFLetterCounter",1,0,"Letters","Computer says NO");
    qmlRegisterUncreatableType<Words>("WFLetterCounter",1,0,"Words","Computer says NO");

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);

    QQmlContext* c = view.rootContext();
    c->setContextProperty("gamemanager", &game);

    view.setSource(QUrl("qrc:///main.qml"));
    view.show();

    return app.exec();
}
