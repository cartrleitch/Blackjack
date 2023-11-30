#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <stack>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
using namespace std;
using namespace std::this_thread;

// todo: repeats hit or stand if you enter wrong thing;

// data and functions
int playerBalance = 100;
int bet;
string check = "";
string playCheck = "";
stack<string> shuffledDeck;
vector<string> dealerHand;
vector<string> playerHand;
string deck[52] = {"A-S", "2-S", "3-S", "4-S", "5-S", "6-S", "7-S", "8-S", "9-S", 
"10-S", "J-S", "Q-S", "K-S", "A-C", "2-C", "3-C", "4-C", "5-C", "6-C", "7-C", 
"8-C", "9-C", "10-C", "J-C", "Q-C","K-C", "A-H", "2-H", "3-H", "4-H", "5-H", "6-H", 
"7-H", "8-H", "9-H", "10-H", "J-H", "Q-H", "K-H","A-D", "2-D", "3-D", "4-D", "5-D", 
"6-D", "7-D", "8-D", "9-D", "10-D", "J-D", "Q-D", "K-D"};

map<string, int> cards = {
    {"A-S", 1}, {"2-S", 2}, {"3-S", 3}, {"4-S", 4}, {"5-S", 5}, {"6-S", 6}, {"7-S", 7}, 
    {"8-S", 8}, {"9-S", 9}, {"10-S", 10}, {"J-S", 10}, {"Q-S", 10}, {"K-S", 10},
    {"A-C", 1}, {"2-C", 2}, {"3-C", 3}, {"4-C", 4}, {"5-C", 5}, {"6-C", 6}, {"7-C", 7}, 
    {"8-C", 8}, {"9-C", 9}, {"10-C", 10}, {"J-C", 10}, {"Q-C", 10}, {"K-C", 10},
    {"A-H", 1}, {"2-H", 2}, {"3-H", 3}, {"4-H", 4}, {"5-H", 5}, {"6-H", 6}, {"7-H", 7}, 
    {"8-H", 8}, {"9-H", 9}, {"10-H", 10}, {"J-H", 10}, {"Q-H", 10}, {"K-H", 10},
    {"A-D", 1}, {"2-D", 2}, {"3-D", 3}, {"4-D", 4}, {"5-D", 5}, {"6-D", 6}, {"7-D", 7}, 
    {"8-D", 8}, {"9-D", 9}, {"10-D", 10}, {"J-D", 10}, {"Q-D", 10}, {"K-D", 10}
};

int random (int i){
    srand((unsigned int)time(NULL));
    return rand()%i;
}

void shuffle(){
    random_shuffle(&deck[0], &deck[52], random);

    for (int i = 0; i < 52; i++){
        shuffledDeck.push(deck[i]);
    }
}

void deal(){
    playerHand.push_back(shuffledDeck.top());
    shuffledDeck.pop();
    dealerHand.push_back(shuffledDeck.top());
    shuffledDeck.pop();
    playerHand.push_back(shuffledDeck.top());
    shuffledDeck.pop();
    dealerHand.push_back(shuffledDeck.top());
    shuffledDeck.pop();
}

void playerHit(){
    playerHand.push_back(shuffledDeck.top());
    shuffledDeck.pop();
}

void dealerHit(){
    dealerHand.push_back(shuffledDeck.top());
    shuffledDeck.pop();
}

void playerBet(){
    cout << "\nEnter bet: " << endl;
    cin >> bet;
    if (bet > 0 && bet <= playerBalance){
        playerBalance -= bet;
    }
}

void clearHands(){
    dealerHand.erase(dealerHand.begin(), dealerHand.end());
    playerHand.erase(playerHand.begin(), playerHand.end());
}

int getCards(vector<string> hand){
    int handValue = 0;
    int numberOfAces = 0;
    for (string card: hand){
        handValue += cards[card];
        if (cards[card] == 1){
            numberOfAces++;
        }
    }

    if (numberOfAces > 0 && handValue + 10 <= 21){
        handValue += 10;
    }
    return handValue;
}

void printInfo(bool flip = true){
    int playerAcesValue = 0;
    cout << "\n";
    cout << "Dealer: " << endl;
    if (flip == false){
        for (int i = 0; i < dealerHand.size() - 1; i++){
            cout << dealerHand.at(i) << " ";
        }
        cout << "***";
    }
    else {
        for (string card: dealerHand){
            cout << card << " ";
        }
        cout << "= ";
        cout << getCards(dealerHand);
    }
    cout << "\n";
    cout << "Player: " << endl;
    for (string card: playerHand){
        cout << card << " ";
    }
    for (string card: playerHand){
        playerAcesValue += cards[card];
    }
    cout << "= ";
    if (getCards(playerHand) != playerAcesValue){
    cout << playerAcesValue;
    cout << " or ";
    }
    cout << getCards(playerHand);
    cout << "\n";
}

void dealerAI(){
    if (getCards(dealerHand) > 17){
        printInfo();
    }
    while (getCards(dealerHand) < 17){
        dealerHit();
        printInfo();
        sleep_for(1s);
    }
}

int checkBlackjack(){
    if (getCards(dealerHand) == 21 && getCards(playerHand) == 21){
        return 2;
    }
    else if (getCards(dealerHand) == 21){
        return 1;
    }
    else if (getCards(playerHand) == 21){
        return 0;
    }
    else{
        return -1;
    }
}

void checkWin(){
    if ((getCards(dealerHand) < getCards(playerHand) && getCards(playerHand) <= 21) || getCards(dealerHand) > 21){
        cout << "\nPlayer wins!";
        if (getCards(playerHand) == 21){
            cout << " Player has Blackjack!";
        }
        playerBalance += 2 * bet; 
    }
    else if ((getCards(dealerHand) > getCards(playerHand) && getCards(dealerHand) <= 21) || getCards(playerHand) > 21){
        cout << "\nDealer wins!";
        if (getCards(dealerHand) == 21){
            cout << " Dealer has Blackjack!";
        }
    }
    else if (getCards(dealerHand) == getCards(playerHand)){
        cout << "\nTie game!";
        playerBalance += bet;
    }
    else {
        cout << "\nNo winner! Maybe an error?";
    }
}

bool checkPlayerBust(){
    if (getCards(playerHand) > 21){
        return true;
    }
    else{
        return false;
    }
}

// mainline
int main(){
    while (playCheck.compare("N")!=0 && playCheck.compare("n")!=0){
        cout << "Welcome to Blackjack! Bets are doubled on win! Good luck!" << endl;
        if (playCheck.compare("")==0){
            shuffle();
        }
        playerBet();
        deal();
        printInfo(false);
        if (checkBlackjack() == 1 || checkBlackjack() == 0){
            check = "end";
        }

        while (check.compare("end")!=0){
            cout << "Hit (hit) or Stand (stand) or End (end):" << endl;
            cin >> check;
            if (check.compare("end")==0){
                goto endLoop;
            }
            else if (check.compare("hit")==0){
                playerHit();
                sleep_for(1s);
                printInfo(false);
                if (checkPlayerBust() == true || checkBlackjack() == 0){
                    check = "end";
                    break;
                }
            }
            else if(check.compare("stand")==0){
                break;
            }
            if (check.compare("end")!=0){
                check = "";
            }
        }

        if (checkPlayerBust() == false || checkBlackjack() == 0 || 
            checkBlackjack() == 1 || checkBlackjack() == 2){
            checkBlackjack();
            sleep_for(1s);
            printInfo();
            sleep_for(1s);
            dealerAI();
            checkBlackjack();
        }
        else{
            printInfo();
        }
        checkWin();
        cout << "\nPlayer balance: $";
        cout << playerBalance;
        if (playerBalance > 0){
            cout << "\nPlay again? (Y/N):" << endl;
            cin >> playCheck;
            clearHands();
            check = "";
            sleep_for(1s);
        }
        else{
            cout << "You're broke! Game over!";
            playCheck = "N";
        }
    }
    endLoop:
        cout << "\n";
    return 0;
}
