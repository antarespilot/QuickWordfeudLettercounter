#include "gamemanager.h"
#include <QCoreApplication>
#include <QStandardPaths>
#include <QDebug>

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDir>

GameManager::GameManager(QObject *parent) :
    QAbstractListModel(parent)
{
    connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit,
            this, &GameManager::saveGames);

    loadGames();
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
    g->setName(QString("Game%1").arg(games_.size()));
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
    QString location = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    QDir dir(location);
    if(!dir.exists()) {
        qDebug() << "Directory " << location << " does not yet exist. creating it...";
        if(!dir.mkpath(location)) {
            qWarning() << "Was not able to create path " << location;
            return;
        }
    }

    QFile savefile(location + "/games.json");
    if(!savefile.open(QIODevice::WriteOnly)) {
        qWarning() << "Unable to open save file " << savefile.fileName();
        return;
    }

    QJsonArray array;
    for(int i=0; i<games_.size(); i++) {
        QJsonObject obj;
        games_.at(i)->write(obj);
        array.append(obj);
    }
    QJsonObject savegame;
    savegame["games"] = array;

    QJsonDocument savedoc(savegame);
    savefile.write(savedoc.toJson());
}

void GameManager::loadGames()
{
    QString location = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    QFile loadfile(location + "/games.json");
    if(!loadfile.open(QIODevice::ReadOnly)) {
        qWarning() << "Unable to open load file " << loadfile.fileName();
        return;
    }

    QByteArray gamedata = loadfile.readAll();

    beginResetModel();
    for(int i=0; i<games_.size(); i++) {
        games_[i]->deleteLater();
    }
    games_.clear();

    QJsonDocument loaddoc(QJsonDocument::fromJson(gamedata));
    QJsonArray array = loaddoc.object()["games"].toArray();
    for(int i=0; i<array.size(); i++) {
        QJsonObject obj = array[i].toObject();
        Game* g = new Game(this);
        g->read(obj);
        games_.append(g);
    }

    endResetModel();
}
