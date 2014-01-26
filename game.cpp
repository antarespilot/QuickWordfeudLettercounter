#include "game.h"
#include <QDebug>

Game::Game(QObject *parent) :
    QAbstractListModel(parent)
  , rem_total_(0)
  , rem_vowels_(0)
  , rem_consonants_(0)
  , rem_wildcards_(0)
{
    initializeLetters();
}

int Game::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return letters_.size();
}

QVariant Game::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= letters_.size())
       return QVariant();

    const Letter& l = letters_.at(index.row());

    if (role == LetterRole)
       return l.letter;
    else if (role == RemainingRole)
       return l.remaining;
    else if (role == TypeRole)
       return l.type;

    return QVariant();
}

QHash<int, QByteArray> Game::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[LetterRole]    = "letter";
    roles[RemainingRole] = "remaining";
    roles[TypeRole]      = "type";

    return roles;
}

void Game::initializeLetters()
{
    beginResetModel();

    letters_.clear();
    letters_ << Letter(".",2,Wildcard)
             << Letter("A",7,Vowel)
             << Letter("B",3,Consonant)
             << Letter("C",1,Consonant)
             << Letter("D",5,Consonant)
             << Letter("E",9,Vowel)
             << Letter("F",4,Consonant)
             << Letter("G",4,Consonant)
             << Letter("H",3,Consonant)
             << Letter("I",6,Vowel)
             << Letter("J",2,Consonant)
             << Letter("K",4,Consonant)
             << Letter("L",5,Consonant)
             << Letter("M",3,Consonant)
             << Letter("N",6,Consonant)
             << Letter("O",4,Vowel)
             << Letter("P",2,Consonant)
             << Letter("Q",0,Consonant)
             << Letter("R",7,Consonant)
             << Letter("S",7,Consonant)
             << Letter("T",7,Consonant)
             << Letter("U",3,Vowel)
             << Letter("V",3,Consonant)
             << Letter("W",1,Consonant)
             << Letter("X",0,Consonant)
             << Letter("Y",1,Vowel)
             << Letter("Z",0,Consonant)
             << Letter("Æ",1,Vowel)
             << Letter("Ø",2,Vowel)
             << Letter("Å",2,Vowel);

    letter_index_.clear();
    for(int i=0; i<letters_.size(); i++)
        letter_index_[letters_[i].letter] = i;

    updateStats();

    endResetModel();
}

void Game::updateStats()
{
    int total = 0;
    int vowels = 0;
    int consonants = 0;
    int wildcards = 0;

    for(int i=0; i<letters_.size(); i++)
    {
        const Letter& l = letters_.at(i);
        total += l.remaining;

        switch(l.type)
        {
        case Vowel:
            vowels += l.remaining;
            break;
        case Consonant:
            consonants += l.remaining;
            break;
        case Wildcard:
            wildcards += l.remaining;
            break;
        }
    }

    setRemTotal(total);
    setRemVowels(vowels);
    setRemConsonants(consonants);
    setRemWildcards(wildcards);
}

void Game::resetRemainings()
{
    for(int i=0; i<letters_.size(); i++)
        letters_[i].test_remaining = letters_[i].remaining;
}

Game::Validity Game::wordValidity(const QString &word)
{
    if(word.isEmpty())
        return EmptyWord;

    QString uword = word.toUpper();
    int wc = remWildcards();

    resetRemainings();

    // check if we have what it takes2
    for(int i=0; i<uword.size(); i++)
    {
        const QString& l = uword.mid(i,1);
        if(!letter_index_.contains(l))
            return InvalidWord;

        Letter& ltr = letters_[letter_index_.value(l)];
        if(ltr.test_remaining <= 0) {
            wc--;
            if(wc < 0)
                return InvalidWord;
        } else {
            ltr.test_remaining--;
        }
    }

    if(wc < remWildcards()) {
        return ValidWordWithWildcard;
    }

    return ValidWord;
}

void Game::applyWord(const QString& word)
{
    QString uword = word.toUpper();

    if(wordValidity(word) == InvalidWord)
        return;

    for(int i=0; i<uword.size(); i++)
    {
        int l_idx = letter_index_.value(uword.at(i));
        Letter& l = letters_[l_idx];
        if(l.remaining > 0) {
            l.remaining--;
            emit dataChanged(index(l_idx),index(l_idx));
        } else {
            // need to spend a wildcard here
            int wc_idx = letter_index_.value(".");
            Letter& wcl = letters_[wc_idx];
            wcl.remaining--;
            emit dataChanged(index(wc_idx),index(wc_idx));
        }
    }

    updateStats();
}
