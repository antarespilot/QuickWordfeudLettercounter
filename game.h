#ifndef GAME_H
#define GAME_H

#include <QAbstractListModel>

class Game : public QAbstractListModel
{
    Q_OBJECT

    Q_ENUMS(LetterType Validity Roles)

    Q_PROPERTY(int remTotal READ remTotal NOTIFY remTotalChanged)
    Q_PROPERTY(int remVowels READ remVowels NOTIFY remVowelsChanged)
    Q_PROPERTY(int remConsonants READ remConsonants NOTIFY remConsonantsChanged)
    Q_PROPERTY(int remWildcards READ remWildcards NOTIFY remWildcardsChanged)

public:

    enum LetterType
    {
        Vowel,
        Consonant,
        Wildcard
    };

    enum Validity
    {
        InvalidWord,
        EmptyWord,
        ValidWord,
        ValidWordWithWildcard
    };

    enum Roles
    {
        LetterRole = Qt::UserRole,
        RemainingRole,
        TypeRole
    };

    struct Letter
    {
        Letter(const QString& _letter, int _remaining, LetterType _type)
            : letter(_letter)
            , remaining(_remaining)
            , test_remaining(_remaining)
            , type(_type)
        {
        }

        QString letter;
        int remaining;
        int test_remaining;
        LetterType type;
    };

public:

    explicit Game(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    Q_INVOKABLE void applyWord(const QString& word);
    Q_INVOKABLE Validity wordValidity(const QString& word);
    Q_INVOKABLE void resetGame() { initializeLetters(); }

    int remTotal() const        { return rem_total_;        }
    int remVowels() const       { return rem_vowels_;       }
    int remConsonants() const   { return rem_consonants_;   }
    int remWildcards() const    { return rem_wildcards_;    }

protected:

    QHash<int, QByteArray> roleNames() const;

private:

    void initializeLetters();
    void updateStats();
    void resetRemainings();

    QList<Letter> letters_;
    QHash<QString,int> letter_index_;

    int rem_total_;
    int rem_vowels_;
    int rem_consonants_;
    int rem_wildcards_;

    void setRemTotal(int s) { if(s==rem_total_) return; rem_total_ = s; emit remTotalChanged(); }
    void setRemVowels(int s) { if(s==rem_vowels_) return; rem_vowels_ = s; emit remVowelsChanged(); }
    void setRemConsonants(int s) { if(s==rem_consonants_) return; rem_consonants_ = s; emit remConsonantsChanged(); }
    void setRemWildcards(int s) { if(s==rem_wildcards_) return; rem_wildcards_ = s; emit remWildcardsChanged(); }

signals:

    void remTotalChanged();
    void remVowelsChanged();
    void remConsonantsChanged();
    void remWildcardsChanged();

};

#endif // GAME_H
