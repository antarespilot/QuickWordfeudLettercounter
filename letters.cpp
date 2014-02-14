#include "letters.h"
#include "words.h"

#include <QDebug>

Letters::Letters(QObject *parent) :
    QAbstractListModel(parent)
{
}

int Letters::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return letters_.size();
}

QVariant Letters::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= letters_.size())
       return QVariant();

    const Letter& l = letters_.at(index.row());

    if (role == LetterRole)
       return l.letter;
    else if (role == CapacityRole)
        return l.capacity;
    else if (role == RemainingRole)
       return l.remaining;
    else if (role == TypeRole)
       return l.type;

    return QVariant();
}

QHash<int, QByteArray> Letters::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[LetterRole]    = "letter";
    roles[CapacityRole]  = "capacity";
    roles[RemainingRole] = "remaining";
    roles[TypeRole]      = "type";

    return roles;
}

void Letters::initialize()
{
    beginResetModel();

    letters_.clear();

    letters_ << Letter("Q",0,Consonant)
             << Letter("W",1,Consonant)
             << Letter("E",9,Vowel)
             << Letter("R",7,Consonant)
             << Letter("T",7,Consonant)
             << Letter("Y",1,Vowel)
             << Letter("U",3,Vowel)
             << Letter("I",6,Vowel)
             << Letter("O",4,Vowel)
             << Letter("P",2,Consonant)
             << Letter("Å",2,Vowel)

             << Letter("A",7,Vowel)
             << Letter("S",7,Consonant)
             << Letter("D",5,Consonant)
             << Letter("F",4,Consonant)
             << Letter("G",4,Consonant)
             << Letter("H",3,Consonant)
             << Letter("J",2,Consonant)
             << Letter("K",4,Consonant)
             << Letter("L",5,Consonant)
             << Letter("Ø",2,Vowel)
             << Letter("Æ",1,Vowel)

             << Letter("Z",0,Consonant)
             << Letter("X",0,Consonant)
             << Letter("C",1,Consonant)
             << Letter("V",3,Consonant)
             << Letter("B",3,Consonant)
             << Letter("N",6,Consonant)
             << Letter("M",3,Consonant)
             << Letter(".",2,Wildcard);


    letter_index_.clear();
    for(int i=0; i<letters_.size(); i++)
        letter_index_[letters_[i].letter] = i;

    endResetModel();
}

bool Letters::letterAvailable(const QString &l) const
{
    if(!letter_index_.contains(l))
        return false;

    int idx = letter_index_.value(l);
    const Letter& letter = letters_.at(idx);

    return (letter.remaining > 0);
}

void Letters::update(const Words *words)
{
    // Update, BRUTE FORCE STYLE

    for(int i=0; i<letters_.size(); i++) {
        Letter& l = letters_[i];
        l.remaining = l.capacity;
    }

    int idx_change_start = letters_.size()-1;
    int idx_change_end = 0;
    int idx;

    for(int i=0; i<words->getWords().size(); i++) {
        const Words::Word& w = words->getWords().at(i);
        for(int j=0; j<w.word.size(); j++) {
            const QString& letter = w.word.mid(j,1);

            if(!letter_index_.contains(letter)) {
                qCritical() << "Can't find letter " << letter;
                qFatal("WE DO NOT HAVE THAT HERE");
            }

            idx = letter_index_.value(letter);

            if(idx < idx_change_start)
                idx_change_start = idx;
            if(idx > idx_change_end)
                idx_change_end = idx;

            Letter& l = letters_[idx];
            l.remaining--;

            if(l.remaining < 0) {
                qCritical() << "Letter " << l.letter << " has subzero left " << l.remaining;
                qFatal("WE SHOULD NOT HAVE SPENT THAT MANY");
            }
        }
    }

    // this didn't quite work
//    if(idx_change_start <= idx_change_end) {
//        qDebug() << "Changed words from index " << idx_change_start << " to " << idx_change_end;
//        emit dataChanged(index(idx_change_start),index(idx_change_end));
//    }
    emit dataChanged(index(0),index(letters_.size()-1));
}
