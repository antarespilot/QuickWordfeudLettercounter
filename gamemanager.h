#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QAbstractListModel>
#include "game.h"

class GameManager : public QAbstractListModel
{
    Q_OBJECT

public:

    enum Roles
    {
        GameRole = Qt::UserRole,
        NameRole
    };

    explicit GameManager(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

protected:

    QHash<int, QByteArray> roleNames() const;

private:

    QList<Game*> games_;

};

#endif // GAMEMANAGER_H
