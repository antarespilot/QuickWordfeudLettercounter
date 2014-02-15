#include "gamemanager.h"

GameManager::GameManager(QObject *parent) :
    QAbstractListModel(parent)
{
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


