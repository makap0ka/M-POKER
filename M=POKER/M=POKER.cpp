#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <map>

using namespace std;

//Короткий опис до моєї роботи:
//Я створив гру покер, де користувач може грати проти ботів.
//75-80% коду написав сам, а решту дивився в інтернеті,тому що зі школою майже нічого не успіваю
//Старався підписувати те що дивився, а так мені сподобався С++ але джава подобається всерівно більше
    


class Card
{
private:
    int value;
    string symbol;

public:

    Card(int value, string symbol)
        : value(value), symbol(symbol)
    {
    }

    int GetValue() const
    {
        return value;
    }

    string GetSymbol() const
    {
        return symbol;
    }

    void Print() const
    {
        if (value == 11)
            cout << "J" << symbol;
        else if (value == 12)
            cout << "Q" << symbol;
        else if (value == 13)
            cout << "K" << symbol;
        else if (value == 14)
            cout << "A" << symbol;
        else
            cout << value << symbol;
    }
};


class Deck
{
private:
    vector<Card> cards;

public:

    void Create()
    {
        vector<string> suits =
        {
            "♥", "♦", "♣", "♠"
        };

        for (const string& suit : suits)
        {
            for (int value = 2; value <= 14; value++)
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
    bool bot;
    int balance;
    int currentBet;

public:

    Player(string name, bool bot, int balance = 1000)
        : name(name), bot(bot), balance(balance), currentBet(0)
    {
    }

    void AddCard(Card card)
    {
        hand.push_back(card);
    }

    const vector<Card>& GetHand() const
    {
        return hand;
    }

    void ShowCards() const
    {
        cout << name << ": ";

        if (bot)
        {
            cout << "?? ??";
        }
        else
        {
            for (const Card& card : hand)
            {
                card.Print();
                cout << " ";
            }
        }

        cout << endl;
    }

    void ShowCardsOpen() const
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

    bool IsBot() const
    {
        return bot;
    }

    int GetBalance() const
    {
        return balance;
    }

    int GetCurrentBet() const
    {
        return currentBet;
    }

    bool MakeBet(int amount)
    {
        if (amount <= 0)
        {
            return false;
        }

        if (amount > balance)
        {
            return false;
        }

        balance -= amount;
        currentBet += amount;

        return true;
    }

    void ResetBet()
    {
        currentBet = 0;
    }

    void AddMoney(int amount)
    {
        balance += amount;
    }
};


class PokerTable
{
private:
    vector<Card> cards;

public:

    void AddCard(Card card)
    {
        cards.push_back(card);
    }

    const vector<Card>& GetCards() const
    {
        return cards;
    }

    void ShowTable() const
    {
        cout << endl;

        cout << "\t\t\t";

        for (const Card& card : cards)
        {
            card.Print();
            cout << "\t";
        }

        cout << endl;

        cout << endl << "========================================================================================" << endl;
    }

    int GetSize() const
    {
        return cards.size();
    }
};

class PokerGame
{
private:
    int pot;

public:

    PokerGame()
        : pot(0)
    {
    }

    int GetPot() const
    {
        return pot;
    }

    bool PlayerBet(Player& player, int amount)
    {
        if (player.MakeBet(amount))
        {
            pot += amount;
            return true;
        }

        return false;
    }

    void ShowPot() const
    {
        cout << "\t\t\tPot: $" << pot << endl;
    }

    void GivePot(Player& player, int multiplier)
    {
        int prize = pot * multiplier;

        player.AddMoney(prize);
        pot = 0;
    }

    void SplitPot(vector<Player>& players, const vector<int>& winners)
    {
        if (winners.empty())
        {
            return;
        }

        int money = pot / winners.size();

        for (int index : winners)
        {
            players[index].AddMoney(money);
        }

        pot = 0;
    }
};

void PlayerBetting(Player& player, PokerGame& game, int playerBet)
{
    cout << player.GetName() << ": ";

    if (player.IsBot())
    {
        cout << "?? ??";
    }
    else
    {
        for (const Card& card : player.GetHand())
        {
            card.Print();
            cout << " ";
        }
    }

    cout << "\t\tBalance: $" << player.GetBalance();

    int amount;

    if (player.IsBot())
    {
        amount = playerBet;

        if (amount > player.GetBalance())
        {
            amount = player.GetBalance();
        }

        cout << "\t\t\t" << player.GetName()
            << " bets: $" << amount << endl;

        game.PlayerBet(player, amount);
    }
    else
    {
        cout << "\t\t\tEnter your bet: $";
        cin >> amount;

        while (amount <= 0 || amount > player.GetBalance())
        {
            cout << "\t\t\tInvalid bet" << endl;
            cout << "\t\t\tYour balance: $" << player.GetBalance() << endl;
            cout << "\t\t\tEnter your bet: $";

            cin >> amount;
        }

        game.PlayerBet(player, amount);

        cout << endl;
        cout << "\t\t\tYou bet: $" << amount << endl;
        cout << "\t\t\tBalance left: $" << player.GetBalance() << endl;
    }

    game.ShowPot();
}

void AllPlayersBetting(vector<Player>& players, PokerGame& game)
{
    int playerBet = 0;

    for (Player& player : players)
    {
        if (!player.IsBot())
        {
            cout << player.GetName() << ": ";

            for (const Card& card : player.GetHand())
            {
                card.Print();
                cout << " ";
            }

            cout << "\t\tBalance: $" << player.GetBalance();

            cout << "\t\t\tEnter your bet: $";
            cin >> playerBet;

            while (playerBet <= 0 || playerBet > player.GetBalance())
            {
                cout << "\t\t\tInvalid bet" << endl;
                cout << "\t\t\tYour balance: $" << player.GetBalance() << endl;
                cout << "\t\t\tEnter your bet: $";

                cin >> playerBet;
            }

            game.PlayerBet(player, playerBet);

            cout << endl;
            cout << "\t\t\tYou bet: $" << playerBet << endl;
            cout << "\t\t\tBalance left: $" << player.GetBalance() << endl;
        }
        else
        {
            cout << player.GetName() << ": ?? ??";
            cout << "\t\tBalance: $" << player.GetBalance();

            int botBet = playerBet;

            if (botBet > player.GetBalance())
            {
                botBet = player.GetBalance();
            }

            game.PlayerBet(player, botBet);

            cout << "\t\t\t" << player.GetName()
                << " bets: $" << botBet << endl;
        }
    }

    game.ShowPot();
}

enum class HandRank  //подивився в інтеренеті що так можна
{
    HighCard = 1,
    Pair,
    TwoPair,
    ThreeOfKind,
    Straight,
    Flush,
    FullHouse,
    FourOfKind,
    StraightFlush,
    RoyalFlush
};


struct HandValue
{
    HandRank rank = HandRank::HighCard;
    vector<int> tiebreakers;
};

class HandEvaluator
{
public:
    static HandValue Evaluate(const vector<Card>& playerCards, const vector<Card>& tableCards)
    {
        vector<Card> allCards = playerCards;

        for (const Card& card : tableCards)
        {
            allCards.push_back(card);
        }

        HandValue bestHand;
        bestHand.rank = HandRank::HighCard;
        bestHand.tiebreakers = { 0 };

        for (int a = 0; a < 7; a++)
        {
            for (int b = a + 1; b < 7; b++)
            {
                for (int c = b + 1; c < 7; c++)
                {
                    for (int d = c + 1; d < 7; d++)
                    {
                        for (int e = d + 1; e < 7; e++)
                        {
                            vector<Card> fiveCards =
                            {
                                allCards[a],
                                allCards[b],
                                allCards[c],
                                allCards[d],
                                allCards[e]
                            };

                            HandValue currentHand = EvaluateFiveCards(fiveCards);

                            if (Compare(currentHand, bestHand) > 0)
                            {
                                bestHand = currentHand;
                            }
                        }
                    }
                }
            }
        }

        return bestHand;
    }

    static string GetRankName(HandRank rank)
    {
        switch (rank)
        {
        case HandRank::HighCard:
            return "High Card";

        case HandRank::Pair:
            return "Pair";

        case HandRank::TwoPair:
            return "Two Pair";

        case HandRank::ThreeOfKind:
            return "Three of a Kind";

        case HandRank::Straight:
            return "Straight";

        case HandRank::Flush:
            return "Flush";

        case HandRank::FullHouse:
            return "Full House";

        case HandRank::FourOfKind:
            return "Four of a Kind";

        case HandRank::StraightFlush:
            return "Straight Flush";

        case HandRank::RoyalFlush:
            return "Royal Flush";
        }

        return "Unknown";
    }

    static int Compare(const HandValue& first, const HandValue& second)
    {
        if (first.rank != second.rank)
        {
            return static_cast<int>(first.rank) > static_cast<int>(second.rank) ? 1 : -1;
        }

        int size = min(first.tiebreakers.size(), second.tiebreakers.size());

        for (int i = 0; i < size; i++)
        {
            if (first.tiebreakers[i] > second.tiebreakers[i])
            {
                return 1;
            }

            if (first.tiebreakers[i] < second.tiebreakers[i])
            {
                return -1;
            }
        }

        return 0;
    }

private:

    static HandValue EvaluateFiveCards(const vector<Card>& cards)
    {
        vector<int> values;

        for (const Card& card : cards)
        {
            values.push_back(card.GetValue());
        }

        sort(values.begin(), values.end(), greater<int>());

        map<int, int> counts;

        for (int value : values)
        {
            counts[value]++;
        }

        bool flush = true;

        for (int i = 1; i < cards.size(); i++)
        {
            if (cards[i].GetSymbol() != cards[0].GetSymbol())
            {
                flush = false;
                break;
            }
        }

        int straightHigh = GetStraightHigh(values);

        bool straight = straightHigh != 0;


        if (flush &&
            straightHigh == 14 &&
            HasValues(values, { 14, 13, 12, 11, 10 }))
        {
            return { HandRank::RoyalFlush, { 14 } };
        }

        if (flush && straight)
        {
            return { HandRank::StraightFlush, { straightHigh } };
        }

        for (auto pair : counts)
        {
            if (pair.second == 4)
            {
                int four = pair.first;
                int kicker = 0;

                for (int value : values)
                {
                    if (value != four)
                    {
                        kicker = value;
                        break;
                    }
                }

                return { HandRank::FourOfKind, { four, kicker } };
            }
        }

        int three = 0;
        int pair = 0;

        for (auto item : counts)
        {
            if (item.second == 3)
            {
                if (item.first > three)
                {
                    three = item.first;
                }
            }
        }

        for (auto item : counts)
        {
            if (item.second >= 2 && item.first != three)
            {
                if (item.first > pair)
                {
                    pair = item.first;
                }
            }
        }

        if (three != 0 && pair != 0)
        {
            return { HandRank::FullHouse, { three, pair } };
        }

        if (flush)
        {
            return { HandRank::Flush, values };
        }

        if (straight)
        {
            return { HandRank::Straight, { straightHigh } };
        }

        if (three != 0)
        {
            vector<int> kickers;

            for (int value : values)
            {
                if (value != three)
                {
                    kickers.push_back(value);
                }
            }

            return
            {
                HandRank::ThreeOfKind,
                { three, kickers[0], kickers[1] }
            };
        }

        vector<int> pairs;

        for (auto item : counts)
        {
            if (item.second == 2)
            {
                pairs.push_back(item.first);
            }
        }

        if (pairs.size() >= 2)
        {
            sort(pairs.begin(), pairs.end(), greater<int>());

            int kicker = 0;

            for (int value : values)
            {
                if (value != pairs[0] && value != pairs[1])
                {
                    kicker = value;
                    break;
                }
            }

            return
            {
                HandRank::TwoPair,
                { pairs[0], pairs[1], kicker }
            };
        }

        if (pairs.size() == 1)
        {
            int pairValue = pairs[0];

            vector<int> kickers;

            for (int value : values)
            {
                if (value != pairValue)
                {
                    kickers.push_back(value);
                }
            }

            return
            {
                HandRank::Pair,
                {
                    pairValue,
                    kickers[0],
                    kickers[1],
                    kickers[2]
                }
            };
        }

        return { HandRank::HighCard, values };
    }

    static int GetStraightHigh(vector<int> values)
    {
        sort(values.begin(), values.end());
        values.erase(unique(values.begin(), values.end()), values.end());

        if (values.size() != 5)
        {
            return 0;
        }

        if (values[0] == 2 &&
            values[1] == 3 &&
            values[2] == 4 &&
            values[3] == 5 &&
            values[4] == 14)
        {
            return 5;
        }

        for (int i = 1; i < values.size(); i++)
        {
            if (values[i] != values[i - 1] + 1)
            {
                return 0;
            }
        }

        return values[4];
    }

    static bool HasValues(const vector<int>& values, const vector<int>& required)
    {
        for (int value : required)
        {
            if (find(values.begin(), values.end(), value) == values.end())
            {
                return false;
            }
        }

        return true;
    }
};

void Showdown(vector<Player>& players, PokerTable& table, PokerGame& game)
{
    cout << endl;
    cout << "===================================== SHOWDOWN =========================================" << endl;

    cout << endl;
    cout << "\t\t\tPlayers cards:" << endl;

    for (Player& player : players)
    {
        player.ShowCardsOpen();
    }

    cout << endl;

    vector<HandValue> hands;

    for (Player& player : players)
    {
        HandValue hand = HandEvaluator::Evaluate(
            player.GetHand(),
            table.GetCards()
        );

        hands.push_back(hand);

        cout << player.GetName() << ": "
            << HandEvaluator::GetRankName(hand.rank)
            << endl;
    }

    int winnerIndex = 0;

    for (int i = 1; i < players.size(); i++)
    {
        if (HandEvaluator::Compare(hands[i], hands[winnerIndex]) > 0)
        {
            winnerIndex = i;
        }
    }

    vector<int> winners;

    for (int i = 0; i < players.size(); i++)
    {
        if (HandEvaluator::Compare(hands[i], hands[winnerIndex]) == 0)
        {
            winners.push_back(i);
        }
    }

    cout << endl;

    if (winners.size() == 1)
    {
        int prize = game.GetPot();

        cout << "\t\t\tWinner: "
            << players[winnerIndex].GetName()
            << endl;

        cout << "\t\t\tCombination: "
            << HandEvaluator::GetRankName(hands[winnerIndex].rank)
            << endl;
    
        cout << "\t\t\tPrize: $" << prize << endl;

        game.GivePot(players[winnerIndex], 1);

        cout << "\t\t\tWinner balance: $"
            << players[winnerIndex].GetBalance()
            << endl;
    }
    else
    {
        int prize = game.GetPot() / winners.size();

        cout << "\t\t\tTie between: ";

        for (int i = 0; i < winners.size(); i++)
        {
            cout << players[winners[i]].GetName();

            if (i + 1 < winners.size())
            {
                cout << ", ";
            }
        }

        cout << endl;

        cout << "\t\t\tCombination: "
            << HandEvaluator::GetRankName(hands[winnerIndex].rank)
            << endl;

        cout << "\t\t\tPrize for each player: $"
            << prize
            << endl;

        game.SplitPot(players, winners);

        cout << endl;

        for (int index : winners)
        {
            cout << players[index].GetName()
                << "\t\t\tbalance: $"
                << players[index].GetBalance()
                << endl;
        }
    }

    cout << endl << "========================================================================================" << endl;
}


void PreFlop(vector<Player>& players, PokerGame& game)
{
    cout << endl;
    cout << "========================================================================================" << endl;
    cout << "\t\t\t\tPRE-FLOP TABLE" << endl;

    AllPlayersBetting(players, game);

    cout << endl;
    cout << "Press ENTER to continue to FLOP...";

    cin.ignore();
    cin.get();
}


void Flop(
    Deck& deck,
    PokerTable& table,
    vector<Player>& players,
    PokerGame& game)
{
    cout << endl;
    cout << "========================================================================================" << endl;
    cout << "\t\t\t\tFLOP TABLE" << endl;

    table.AddCard(deck.DealCard());
    table.AddCard(deck.DealCard());
    table.AddCard(deck.DealCard());

    table.ShowTable();

    AllPlayersBetting(players, game);

    cout << endl;
    cout << "Press ENTER to continue to TURN...";

    cin.get();
}


void Turn(
    Deck& deck,
    PokerTable& table,
    vector<Player>& players,
    PokerGame& game)
{
    cout << endl;
    cout << "========================================================================================" << endl;
    cout << "\t\t\t\tTURN TABLE" << endl;

    table.AddCard(deck.DealCard());

    table.ShowTable();

    AllPlayersBetting(players, game);

    cout << endl;
    cout << "Press ENTER to continue to RIVER...";

    cin.get();
}


void River(
    Deck& deck,
    PokerTable& table,
    vector<Player>& players,
    PokerGame& game)
{
    cout << endl;
    cout << "========================================================================================" << endl;
    cout << "\t\t\t\tRIVER TABLE" << endl;

    table.AddCard(deck.DealCard());

    table.ShowTable();

    AllPlayersBetting(players, game);

    cout << endl;
    cout << "\t\tAll five community cards are on the table" << endl;
}


int main()
{
    system("chcp 65001 > nul"); //це для знаків не трогать

    int startBalance;

    cout << "\t\t\tEnter your starting balance: $";
    cin >> startBalance;

    while (startBalance <= 0)
    {
        cout << "\t\tBalance must be greater than 0" << endl;
        cout << "\t\tEnter your starting balance: $";
        cin >> startBalance;
    }

    int botCount;

    cout << "\t\t\tEnter number of bots: ";
    cin >> botCount;

    while (botCount < 1)
    {
        cout << "\t\t\tNumber of bots must be at least 1" << endl;
        cout << "\t\t\tEnter number of bots: ";
        cin >> botCount;
    }

    if ((botCount + 1) * 2 > 52)
    {
        cout << "\t\t\tToo many players" << endl;
        cout << "\t\t\tMaximum number of bots: 25" << endl;
        return 0;
    }

    int balance = startBalance;
    int choice;

    do
    {
        Deck deck;

        deck.Create();
        deck.Shuffle();

        vector<Player> players;

        players.push_back(Player("You", false, balance));

        for (int i = 1; i <= botCount; i++)
        {
            players.push_back(
                Player("Bot " + to_string(i), true, startBalance)
            );
        }

        for (int card = 0; card < 2; card++)
        {
            for (Player& player : players)
            {
                player.AddCard(deck.DealCard());
            }
        }

        PokerTable table;

        PokerGame game;

        PreFlop(players, game);

        Flop(deck, table, players, game);

        Turn(deck, table, players, game);

        River(deck, table, players, game);

        Showdown(players, table, game);

        balance = players[0].GetBalance();

        cout << "\t\t\t\tGAME FINISHED" << endl <<endl;

        cout << "\t\t\tYour balance: $"
            << balance
            << endl;

        if (balance <= 0)
        {
            cout << endl;
            cout << "\t\t\tYou have no money left" << endl;
            cout << "\t\t\tGame over." << endl;
            break;
        }

        cout << endl;
        cout << "\t\t\t1 - Play again" << endl;
        cout << "\t\t\t2 - Exit" << endl;

        cout << "\t\t\tYour choice: ";
        cin >> choice;

        while (choice != 1 && choice != 2)
        {
            cout << "\t\t\tInvalid choice" << endl;
            cout << "\t\t\tYour choice: ";
            cin >> choice;
        }

    } while (choice == 1);

    cout << endl;
    cout << "\t\t\tThanks for your pensiya" << endl;

}