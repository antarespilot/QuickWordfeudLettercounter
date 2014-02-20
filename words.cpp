#include "words.h"
#include <QJsonArray>

Words::Words(QObject *parent) :
    QAbstractListModel(parent)
{
    initialize();
}

void Words::initialize()
{
    beginResetModel();
    words_.clear();
    words_.append(Word());
    endResetModel();
}

void Words::read(const QJsonObject &json)
{
    beginResetModel();
    words_.clear();
    QJsonArray array = json["words"].toArray();
    for(int i=0; i<array.size(); i++) {
        QJsonObject wobj = array[i].toObject();
        Word w;
        w.word = wobj["word"].toString();
        words_.append(w);
    }
    endResetModel();
}

void Words::write(QJsonObject &json) const
{
    QJsonArray array;
    for(int i=0; i<words_.size(); i++) {
        QJsonObject wobj;
        wobj["word"] = words_.at(i).word;
        array.append(wobj);
    }
    json["words"] = array;
}

int Words::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return words_.size();
}

QVariant Words::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= words_.size())
       return QVariant();

    const Word& w = words_.at(index.row());

    if (role == WordRole)
       return w.word;
    else if (role == ValidityRole)
       return w.validity;

    return QVariant();
}

QHash<int, QByteArray> Words::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[WordRole]    = "word";
    roles[ValidityRole] = "validity";
    return roles;
}

void Words::addWord()
{
    beginInsertRows(QModelIndex(), words_.size(), words_.size());
    words_.append(Word());
    endInsertRows();
}

bool Words::addLetter(const QString &letter)
{
    if(words_.isEmpty())
        return false;

    Word& w = words_.last();
    w.word += letter;
    emit dataChanged(index(words_.size()-1),index(words_.size()-1));
    return true;
}

bool Words::backspace()
{
    if(words_.isEmpty())
        return false;

    Word& w = words_.last();
    w.word.truncate(w.word.size()-1);

    emit dataChanged(index(words_.size()-1),index(words_.size()-1));
    return true;
}

bool Words::deleteWord()
{
    if(words_.size() <= 1) // not allowed to delete first word
        return false;

    beginRemoveRows(QModelIndex(), words_.size()-1, words_.size()-1);
    words_.removeLast();
    endRemoveRows();

    return true;
}
