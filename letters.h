#ifndef LETTERS_H
#define LETTERS_H

#include <QAbstractListModel>

class Words;
class Letters : public QAbstractListModel
{
    Q_OBJECT

    Q_ENUMS(LetterType Roles)

public:

    enum LetterType
    {
        Vowel,
        Consonant,
        Wildcard
    };

    enum Roles
    {
        LetterRole = Qt::UserRole,
        CapacityRole,
        RemainingRole,
        TypeRole
    };

    struct Letter
    {
        Letter(const QString& _letter, int _remaining, LetterType _type)
            : letter(_letter)
            , capacity(_remaining)
            , remaining(_remaining)
            , type(_type)
        {
        }

        QString letter;
        int capacity;
        int remaining;
        LetterType type;
    };

    explicit Letters(QObject *parent = 0);

    void initialize();

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    const QList<Letter>& getLetters() const { return letters_; }

    bool letterAvailable(const QString& l) const;

    void update(const Words* words);

protected:

    QHash<int, QByteArray> roleNames() const;

private:

    QList<Letter> letters_;
    QHash<QString,int> letter_index_;


};

#endif // LETTERS_H
