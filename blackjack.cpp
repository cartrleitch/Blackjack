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
#include <bits/stdc++.h>
using namespace std;
using namespace std::this_thread;

//todo: allow split and double bets and insurance;
// add optional cheat mode to show probabilities to suggest moves,
// this needs to be based on probabilities for the entire deck
// minus known cards, so it is not counting cards and consideringdf
// real values left in the deck;
// maybe make player objects for multiplayer;
// add visual components;

// data and functions

double playerBalance = 100.000;
double bet = 0.00;
int minBet = 5;
int numRounds = 0;
int numBlackjacks = 0;
float highestBalance = playerBalance;
float wins = 0;
float losses = 0;
float winPercentage = 0;
float biggestWin = 0;
float biggestLoss = 0;
vector <float> highestBalanceList;
vector <float> wlAmounts;
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
    float number;
    while (bet > playerBalance || bet < minBet){
        printf("\nPlayer balance: $%.2f\n", playerBalance);
        cout << "\nEnter bet: " << endl;
        // ensures numeric input
        while (!(cin >> number)){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nEnter bet (must be a number): " << endl;
        }
        bet = number;

        if (bet <= playerBalance && bet >= minBet){
            playerBalance -= bet;
            break;
        }
    }
    sleep_for(1s);
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
    cout << "\nDealer: " << endl;

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
        cout << "= " << getCards(dealerHand);
    }

    cout << "\nPlayer: " << endl;

    for (string card: playerHand){
        cout << card << " ";
    }

    for (string card: playerHand){
        playerAcesValue += cards[card];
    }

    cout << "= ";

    if (getCards(playerHand) != playerAcesValue){
        cout << playerAcesValue << " or ";
    }
    cout << getCards(playerHand) << endl;
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
            playerBalance += 2.5 * bet; 
            wlAmounts.push_back(2.5 * bet);
            numBlackjacks++;
        }
        else{
            playerBalance += 2 * bet; 
            wlAmounts.push_back(2*bet);
        }

        wins++;
    }
    else if ((getCards(dealerHand) > getCards(playerHand) && getCards(dealerHand) <= 21) || getCards(playerHand) > 21){
        cout << "\nDealer wins!";
        
        if (getCards(dealerHand) == 21){
            cout << " Dealer has Blackjack!";
        }

        wlAmounts.push_back(-bet);
        losses++;
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

void playerStats(){
    if (wins + losses > 0){
        winPercentage = 100*(wins/(wins + losses));
    }
    if (!highestBalanceList.empty()){
        highestBalance = *max_element(highestBalanceList.begin(), highestBalanceList.end());
    }

    if (!wlAmounts.empty()){
        biggestWin = *max_element(wlAmounts.begin(), wlAmounts.end());
        biggestLoss = *min_element(wlAmounts.begin(), wlAmounts.end());
    }
    if (biggestWin < 0){
        biggestWin = 0;
    }
    if (biggestLoss > 0){
        biggestLoss = 0;
    }
    cout << "\nPlayer stats: " << endl;
    cout << "Rounds played: " << numRounds << endl;
    cout << "Blackjacks: " << numBlackjacks << endl;
    printf("Highest balance: $%.2f\n", highestBalance);
    cout << "Wins: " << wins << endl;
    cout << "Losses: " << losses << endl;
    cout << "Win percentage: " << winPercentage << "%" << endl;
    printf("Biggest win: $%.2f\n",biggestWin);
    printf("Biggest loss: $%.2f\n",abs(biggestLoss));
}

// mainline
int main(){
    highestBalanceList.push_back(playerBalance);

    while (playCheck.compare("N")!=0 && playCheck.compare("n")!=0){
        cout << "Welcome to Blackjack! Bets are doubled on win!\nBlackjack 3:2 payout! $5 minimum bet! Good luck!" << endl;
        numRounds++;

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
            transform(check.begin(), check.end(), check.begin(), ::tolower);
            if (check.compare("end")==0){
                goto endLoop;
            }
            else if (check.compare("hit")==0 || check.compare("h")==0){
                playerHit();
                sleep_for(1s);
                printInfo(false);

                if (checkPlayerBust() == true || checkBlackjack() == 0){
                    check = "end";
                    break;
                }
            }
            else if(check.compare("stand")==0 || check.compare("s")==0){
                break;
            }

            if (check.compare("end")!=0){
                check = "";
            }
        }

        if (checkPlayerBust() == false && checkBlackjack() != 0 && 
            checkBlackjack() != 1 && checkBlackjack() != 2){
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
        printf("\nPlayer balance: $%.2f", playerBalance);
        highestBalanceList.push_back(playerBalance);

        if (playerBalance > 0){
            cout << "\nPlay again? (Y/N):" << endl;
            cin >> playCheck;
            clearHands();
            check = "";
            bet = 0;
            sleep_for(1s);
        }
        else{
            cout << "\nYou're broke! Game over!";
            playCheck = "N";
        }
    }
    endLoop:
        cout << "\n";
    playerStats();
    system("pause");
    return 0;
}
