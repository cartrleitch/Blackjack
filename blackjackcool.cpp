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

//todo: allow split bets and insurance;
// add optional cheat mode to show probabilities to suggest moves,
// this needs to be based on probabilities for the entire deck
// minus known cards, so it is not counting cards and considering;
// real values left in the deck;
// maybe make player objects for multiplayer;
// add visual components;

// Split:
// split info make it so you can select to show split as well as another parameter (delete split info function);
// make all stat tracking work when split;
// make sure it checks wins for both hands on split and blackjacks on both;
// minimize code duplication;

// data and functions

double playerBalance = 100.000;
double bet = 0.00;
int minBet = 5;
int numRounds = 0;
int numBlackjacks = 0;
double highestBalance = playerBalance;
double wins = 0;
double losses = 0;
double winPercentage = 0;
double biggestWin = 0;
double biggestLoss = 0;
vector <double> highestBalanceList;
vector <double> wlAmounts;
string check = "";
string splitCheck = "";
string playCheck = "";
bool hasSplit = false;
stack<string> shuffledDeck;
vector<string> dealerHand;
vector<string> playerHand;
vector<string> splitHand;
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

void splitHit(){
    splitHand.push_back(shuffledDeck.top());
    shuffledDeck.pop();
}

void dealerHit(){
    dealerHand.push_back(shuffledDeck.top());
    shuffledDeck.pop();
}

void playerSplit(){
    splitHand.push_back(playerHand.at(1));
    playerHand.pop_back();
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
void printSplitInfo(){
    int playerAcesValue = 0;
    int playerSplitAcesValue = 0;

    cout << "\nDealer: " << endl;

    for (int i = 0; i < dealerHand.size() - 1; i++){
        cout << dealerHand.at(i) << " ";
    }
    cout << "***";

    cout << "\nPlayer Hand 1: " << endl;

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

    cout << "\nPlayer Hand 2: " << endl;

    for (string card: splitHand){
        cout << card << " ";
    }

    playerAcesValue = 0;

    for (string card: splitHand){
        playerAcesValue += cards[card];
    }

    cout << "= ";

    if (getCards(splitHand) != playerSplitAcesValue){
        cout << playerSplitAcesValue << " or ";
    }
    cout << getCards(splitHand) << endl;
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
        sleep_for(1s);
    }
    while (getCards(dealerHand) < 17){
        dealerHit();
        printInfo();
        sleep_for(1s);
    }
}

int checkBlackjack(vector<string> phand){
    if (getCards(dealerHand) == 21 && getCards(phand) == 21){
        return 2;
    }
    else if (getCards(dealerHand) == 21){
        return 1;
    }
    else if (getCards(phand) == 21){
        return 0;
    }
    else{
        return -1;
    }
}

void checkWin(vector<string> phand){
    if ((getCards(dealerHand) < getCards(phand) && getCards(phand) <= 21) || getCards(dealerHand) > 21){
        cout << "\nPlayer wins!";

        if (getCards(phand) == 21){
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
    else if ((getCards(dealerHand) > getCards(phand) && getCards(dealerHand) <= 21) || getCards(phand) > 21){
        cout << "\nDealer wins!";
        
        if (getCards(dealerHand) == 21){
            cout << " Dealer has Blackjack!";
        }

        wlAmounts.push_back(-bet);
        losses++;
    }
    else if (getCards(dealerHand) == getCards(phand)){
        cout << "\nTie game!";
        playerBalance += bet;
    }
    else {
        cout << "\nNo winner! Maybe an error?";
    }
}

bool checkPlayerBust(vector<string> hand){
    if (getCards(hand) > 21){
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
void hitCheck(vector<string> hand){
    playerHit();
    sleep_for(1s);
    printInfo(false);

    if (checkPlayerBust(hand) == true || checkBlackjack(hand) == 0){
        break;
    }
}

// mainline
int main(){
    system("color 20");
    highestBalanceList.push_back(playerBalance);

    while (playCheck.compare("N")!=0 && playCheck.compare("n")!=0){
        cout << "Welcome to Blackjack! Bets are doubled on win!\nBlackjack 3:2 payout! $5 minimum bet! Good luck!" << endl;
        numRounds++;

        if (playCheck.compare("")==0 || shuffledDeck.size() <= 10){
            shuffle();
        }

        playerBet();
        deal();
        printInfo(false);

        if (checkBlackjack(playerHand) == 1 || checkBlackjack(playerHand) == 0){
            check = "end";
        }

        while (check.compare("end")!=0){
            if (playerHand.size() == 2){
                cout << "Hit or Stand or Double or End:" << endl;
            }
            else if (playerHand.size() == 2 && cards[playerHand.at(0)] == cards[playerHand.at(1)]){
                cout << "Hit or Stand or Double or Split or End:" << endl;
            }
            else{
                cout << "Hit or Stand or End:" << endl;
            }

            cin >> check;
            transform(check.begin(), check.end(), check.begin(), ::tolower);

            if (check.compare("end")==0){
                goto endLoop;
            }
            else if (check.compare("hit")==0 || check.compare("h")==0){
                hitCheck(playerHand);
            }
            else if(playerHandsize() == 2 && cards[playerHand.at(0)] == cards[playerHand.at(1)]
                    && check.compare("split")==0 || check.compare("sp")==0){
                
                playerSplit();
                hasSplit = true;
                sleep_for(1s);
                printf("\nSplitting!\nPlayer balance: $%.2f", playerBalance);
                bet *= 2;
                playerBalance -= bet;
                printSplitInfo();
                sleep_for(1s);

                while (splitCheck.compare("end")!=0){
                    cout << "\nHand 1: Hit or Stand or End:" << endl;
                    cin >> splitCheck;
                    transform(splitCheck.begin(), splitCheck.end(), splitCheck.begin(), ::tolower);

                    if (splitCheck.compare("end")==0){
                        goto endLoop;
                    }
                    else if (splitCheck.compare("hit")==0 || splitCheck.compare("h")==0){
                        hitCheck(splitHand);
                    }
                    else if(splitCheck.compare("stand")==0 || splitCheck.compare("s")==0){
                        break;
                    }
                }
                splitCheck = "";
                while (splitCheck.compare("end")!=0){
                    cout << "\nHand 2: Hit or Stand or End:" << endl;
                    cin >> splitCheck;
                    transform(splitCheck.begin(), splitCheck.end(), splitCheck.begin(), ::tolower);

                    if (splitCheck.compare("end")==0){
                        goto endLoop;
                    }
                    else if (splitCheck.compare("hit")==0 || splitCheck.compare("h")==0){
                        hitCheck(playerHand);
                    }
                    else if(splitCheck.compare("stand")==0 || splitCheck.compare("s")==0){
                        break;
                    }
                }
                splitCheck = "";




                break;
            }
            else if(check.compare("stand")==0 || check.compare("s")==0){
                break;
            }
            else if(playerHand.size() == 2 && check.compare("double")==0 || check.compare("d")==0){
                if (playerBalance - bet >= 0){
                    playerBalance -= bet;
                    sleep_for(1s);
                    printf("\nDoubling your bet!\nPlayer balance: $%.2f", playerBalance);
                    bet *= 2;
                    playerHit();
                    sleep_for(1s);
                    printInfo(false);
                    break;
                }
                else{
                    cout << "\nCannot double bet! Balance is too low!" << endl;
                }
            }
            else{
                cout << "\nInvalid input!" << endl;
            }
            if (check.compare("end")!=0){
                check = "";
            }
        }

        if (hasSplit == false && checkPlayerBust(playerHand) == false && checkBlackjack(playerHand) != 0 && 
            checkBlackjack(playerHand) != 1 && checkBlackjack(playerHand) != 2){
            checkBlackjack(playerHand);
            sleep_for(1s);
            printInfo();
            sleep_for(1s);
            dealerAI();
            checkBlackjack(playerHand);
            checkWin(playerHand);
        }
        else if (hasSplit == true){
            checkBlackjack(playerHand);
            sleep_for(1s);
            printInfo();
            sleep_for(1s);
            dealerAI();
            checkBlackjack(playerHand);
            cout << "\nHand 1: " << endl;
            checkWin(playerHand);
            cout << "\nHand 2: " << endl;
            checkWin(splitHand);
        }
        else{
            printInfo();
        }
        printf("\nPlayer balance: $%.2f", playerBalance);
        highestBalanceList.push_back(playerBalance);

        if (playerBalance > 0){
            cout << "\nPlay again? (Y/N):" << endl;
            cin >> playCheck;
            clearHands();
            check = "";
            hasSplit = false;
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
