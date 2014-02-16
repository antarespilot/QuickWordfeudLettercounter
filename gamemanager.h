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

    Q_INVOKABLE int numGames() const    { return games_.size(); }
    Q_INVOKABLE Game* game(int idx)     { return games_[idx];   }

    Q_INVOKABLE void addNewGame();
    Q_INVOKABLE void deleteGame(int game_idx);

protected:

    QHash<int, QByteArray> roleNames() const;

private:


    QList<Game*> games_;

private slots:

    void saveGames();
    void loadGames();

};

#endif // GAMEMANAGER_H
