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

// todo: Ace can be 1 or 11 (automate choice to some extent) 
// (currently works but you have to enter value each time getDealerCards 
// is called which is tedious, maybe make global so it does it automatically otherwise);
// player can choose balance;
// blackjack message repeated;
// repeats hit or stand if you enter wrong thing

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
    if (bet > 0 && bet < playerBalance){
        playerBalance -= bet;
    }
}

void clearHands(){
    dealerHand.erase(dealerHand.begin(), dealerHand.end());
    playerHand.erase(playerHand.begin(), playerHand.end());
}

int playerAces(){
    int aceCheck;
    while (aceCheck != 1 && aceCheck != 11){
        cout << "\nAce equals 1 or 11:" << endl;
        cin >> aceCheck;
        if (aceCheck == 1){
            return 1;
        }
        else if (aceCheck == 11){
            return 11;
        }
    }
    return 1;
}

int getDealerCards(){
    int dealerHandValue = 0;
    for (int i = 0; i < dealerHand.size(); i++){
        dealerHandValue += cards[dealerHand.at(i)];
    }
    return dealerHandValue;
}

int getPlayerCards(){
    int playerHandValue = 0;
    for (int i = 0; i < playerHand.size(); i++){
        if (cards[playerHand.at(i)] == 1){
            playerHandValue += playerAces();
        }
        else {
            playerHandValue += cards[playerHand.at(i)];
        }
    }
    return playerHandValue;
}

void printInfo(){
    cout << "\n";
    cout << "Dealer: " << endl;
    for (int i = 0; i < dealerHand.size() - 1; i++){
        cout << dealerHand.at(i) << " ";
    }
    cout << "***";
    cout << "\n";
    cout << "Player: " << endl;
    for (int i = 0; i < playerHand.size(); i++){
        cout << playerHand.at(i) << " ";
    }
    cout << "= ";
    cout << getPlayerCards();
    cout << "\n";
}

void printInfoFlip(){
    cout << "\n";
    cout << "Dealer: " << endl;
    for (int i = 0; i < dealerHand.size(); i++){
        cout << dealerHand.at(i) << " ";
    }
    cout << "= ";
    cout << getDealerCards();
    cout << "\n";
    cout << "Player: " << endl;
    for (int i = 0; i < playerHand.size(); i++){
        cout << playerHand.at(i) << " ";
    }
    cout << "= ";
    cout << getPlayerCards();
    cout << "\n";
}

void dealerAI(){
    if (getDealerCards() > 17){
        printInfoFlip();
    }
    while (getDealerCards() < 17){
        dealerHit();
        printInfoFlip();
    }
}

int checkBlackjack(){
    if (getDealerCards() == 21 && getPlayerCards() == 21){
        cout << "Both Blackjack!";
        return 2;
    }
    else if (getDealerCards() == 21){
        cout << "Dealer Blackjack!";
        return 1;
    }
    else if (getPlayerCards() == 21){
        cout << "Player Blackjack!";
        return 0;
    }
    else{
        return -1;
    }
}

void checkWin(){
    if ((getDealerCards() < getPlayerCards() && getPlayerCards() <= 21) || getDealerCards() > 21){
        cout << "\nPlayer wins!";
        playerBalance += 2 * bet; 
    }
    else if ((getDealerCards() > getPlayerCards() && getDealerCards() <= 21) || getPlayerCards() > 21){
        cout << "\nDealer wins!";
    }
    else if (getDealerCards() == getPlayerCards()){
        cout << "\nTie game!";
        playerBalance += bet;
    }
    else {
        cout << "\nNo winner! Maybe an error?";
    }
}

bool checkPlayerBust(){
    if (getPlayerCards() > 21){
        return true;
    }
    else{
        return false;
    }
}

// mainline
int main(){
    while (playCheck.compare("N")!=0){
        cout << "Welcome to Blackjack! Bets are doubled on win! Good luck!" << endl;
        if (playCheck.compare("")==0){
            shuffle();
        }
        playerBet();
        deal();
        printInfo();
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
                printInfo();
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

        if (checkPlayerBust() == false){
            checkBlackjack();
            sleep_for(1s);
            printInfoFlip();
            sleep_for(1s);
            dealerAI();
            checkBlackjack();
        }
        else{
            printInfoFlip();
        }
        checkWin();
        cout << "\nPlayer balance: ";
        cout << playerBalance;
        cout << "\nPlay again? (Y/N):" << endl;
        cin >> playCheck;
        clearHands();
        check = "";
        sleep_for(1s);
    }
    endLoop:
        cout << "\n";
    return 0;
}
