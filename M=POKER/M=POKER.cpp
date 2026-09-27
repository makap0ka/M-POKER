#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>

using namespace std;


class Card
{
private:
    string value;
    string suit;

public:

    Card(string value, string suit)
        : value(value), suit(suit)
    {
    }

    string GetValue() const
    {
        return value;
    }

    string GetSuit() const
    {
        return suit;
    }

    void Print() const
    {
        cout << value << suit;
    }
};


class Deck
{
private:
    vector<Card> cards;

public:

    void Create()
    {
        vector<string> values =
        {
            "2", "3", "4", "5", "6", "7", "8",
            "9", "10", "J", "Q", "K", "A"
        };

        vector<string> suits =
        {
            "H", "D", "C", "S"
        };

        for (const string& suit : suits)
        {
            for (const string& value : values)
            {
                cards.push_back(Card(value, suit));
            }
        }
    }

    void Shuffle()
    {
        random_device rd;
        mt19937 generator(rd());

        shuffle(cards.begin(), cards.end(), generator);
    }

    Card DealCard()
    {
        Card card = cards.back();

        cards.pop_back();

        return card;
    }

    int GetSize() const
    {
        return cards.size();
    }
};


class Player
{
private:
    string name;
    vector<Card> hand;

public:

    Player(string name)
        : name(name)
    {
    }

    void AddCard(Card card)
    {
        hand.push_back(card);
    }

    void ShowCards() const
    {
        cout << name << ": ";

        for (const Card& card : hand)
        {
            card.Print();
            cout << " ";
        }

        cout << endl;
    }

    string GetName() const
    {
        return name;
    }
};


int main()
{
    int botCount;

    cout << "Enter number of bots: ";
    cin >> botCount;

    if (botCount < 1)
    {
        cout << "Number of bots must be at least 1" << endl;
        return 0;
    }

    int playerCount = botCount + 1;

    if (playerCount * 2 > 52)
    {
        cout << "Too many players!" << endl;
        cout << "Maximum number of bots: 25" << endl;

        return 0;
    }

    Deck deck;

    deck.Create();
    deck.Shuffle();

    vector<Player> players;

    players.push_back(Player("You"));

    for (int i = 1; i <= botCount; i++)
    {
        players.push_back(Player("Bot " + to_string(i)));
    }

    for (int card = 0; card < 2; card++)
    {
        for (Player& player : players)
        {
            player.AddCard(deck.DealCard());
        }
    }


    cout << endl;
    cout << "============================" << endl;
    cout << "       POKER TABLE" << endl;
    cout << "============================" << endl;

    cout << endl;
    cout << "Cards:" << endl;

    for (const Player& player : players)
    {
        player.ShowCards();
    }

    cout << endl;
    cout << "Cards left in deck: "
        << deck.GetSize() << endl;
}