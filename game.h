#ifndef GAME_H
#define GAME_H

#include <QAbstractListModel>
#include <QJsonObject>
#include "letters.h"
#include "words.h"

class Game : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int remTotal READ remTotal NOTIFY remTotalChanged)
    Q_PROPERTY(int remVowels READ remVowels NOTIFY remVowelsChanged)
    Q_PROPERTY(int remConsonants READ remConsonants NOTIFY remConsonantsChanged)
    Q_PROPERTY(int remWildcards READ remWildcards NOTIFY remWildcardsChanged)

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

    Q_PROPERTY(Letters* letters READ letters CONSTANT)
    Q_PROPERTY(Words* words READ words CONSTANT)

public:

    explicit Game(QObject *parent = 0);

    void read(const QJsonObject& json);
    void write(QJsonObject& json) const;

    Letters* letters() { return letters_;   }
    Words* words() { return words_;         }

    Q_INVOKABLE bool addLetter(const QString& letter);
    Q_INVOKABLE void backspace();
    Q_INVOKABLE void addWord();
    Q_INVOKABLE void deleteWord();

    int remTotal() const        { return rem_total_;        }
    int remVowels() const       { return rem_vowels_;       }
    int remConsonants() const   { return rem_consonants_;   }
    int remWildcards() const    { return rem_wildcards_;    }

    const QString& name() const { return name_; }
    void setName(const QString& name)   { if(name==name_) return; name_ = name; emit nameChanged(); }

private:

    Letters* letters_;
    Words* words_;

    void updateLetters();
    void updateStats();

    QString name_;

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

    void nameChanged();

};

#endif // GAME_H
