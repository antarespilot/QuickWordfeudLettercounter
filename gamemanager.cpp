#include "gamemanager.h"
#include <QCoreApplication>
#include <QStandardPaths>
#include <QDebug>

GameManager::GameManager(QObject *parent) :
    QAbstractListModel(parent)
{
    connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit,
            this, &GameManager::saveGames);

    loadGames();
    addNewGame();
}

int GameManager::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return games_.size();
}

QVariant GameManager::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= games_.size())
       return QVariant();

    if (role == GameRole)
       return QVariant::fromValue(games_[index.row()]);
//    else if(role == NameRole)
//        return

    return QVariant();

}

QHash<int, QByteArray> GameManager::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[GameRole]    = "game";
    return roles;
}

void GameManager::addNewGame()
{
    Game* g = new Game(this);
    g->setName(QString("NewGame%1").arg(games_.size()));
    beginInsertRows(QModelIndex(), games_.size(), games_.size());
    games_.append(g);
    endInsertRows();
}

void GameManager::deleteGame(int game_idx)
{
    if(game_idx < 0 || game_idx >= games_.size())
        return;

    beginRemoveRows(QModelIndex(), game_idx, game_idx);
    Game* gameless = games_.takeAt(game_idx);
    endRemoveRows();

    gameless->deleteLater();
}

void GameManager::saveGames()
{
    qDebug() << "Save games";

    QString location = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    qDebug() << "It'll go into " << location;

}

void GameManager::loadGames()
{
    qDebug() << "Load games";

    QString location = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    qDebug() << "It'll go into " << location;

}
