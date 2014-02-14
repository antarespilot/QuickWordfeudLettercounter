#ifndef WORDS_H
#define WORDS_H

#include <QAbstractListModel>

class Words : public QAbstractListModel
{
    Q_OBJECT
    Q_ENUMS(Validity)

public:

    enum Validity
    {
        InvalidWord,
        EmptyWord,
        ValidWord,
        ValidWordWithWildcard
    };

    enum Roles
    {
        WordRole = Qt::UserRole,
        ValidityRole
    };

    struct Word
    {
        Word()
            : validity(EmptyWord)
        {
        }

        QString word;
        Validity validity;
    };

    explicit Words(QObject *parent = 0);

    void initialize();

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    const QList<Word>& getWords() const { return words_;    }

    const QString& currWord() const { return words_.last().word;    }

    void addWord();
    bool addLetter(const QString& letter);
    bool backspace();
    bool deleteWord();

protected:

    QHash<int, QByteArray> roleNames() const;

private:

    QList<Word> words_;

};

#endif // WORDS_H
