#include "game.h"
#include <QDebug>

Game::Game(QObject *parent) :
    QObject(parent)
  , rem_total_(0)
  , rem_vowels_(0)
  , rem_consonants_(0)
  , rem_wildcards_(0)
{
    qDebug() << "New game here";

    letters_ = new Letters(this);
    words_ = new Words(this);

    letters_->initialize();
    words_->initialize();
    updateStats();
}

void Game::addWord()
{
    if(!words_->currWord().isEmpty())
        words_->addWord();
}

void Game::deleteWord()
{
    if(words_->deleteWord())
        updateLetters();
}

bool Game::addLetter(const QString &letter)
{
    if(!letters_->letterAvailable(letter.toUpper()))
        return false;

    if(words_->addLetter(letter.toUpper())) {
        updateLetters();
        return true;
    }

    return false;
}

void Game::backspace()
{
    if(words_->backspace())
        updateLetters();
}

void Game::updateLetters()
{
    letters_->update(words_);
    updateStats();
}

void Game::updateStats()
{
    int total = 0;
    int vowels = 0;
    int consonants = 0;
    int wildcards = 0;

    for(int i=0; i<letters_->getLetters().size(); i++)
    {
        const Letters::Letter& l = letters_->getLetters().at(i);
        total += l.remaining;

        switch(l.type)
        {
        case Letters::Vowel:
            vowels += l.remaining;
            break;
        case Letters::Consonant:
            consonants += l.remaining;
            break;
        case Letters::Wildcard:
            wildcards += l.remaining;
            break;
        }
    }

    setRemTotal(total);
    setRemVowels(vowels);
    setRemConsonants(consonants);
    setRemWildcards(wildcards);
}

