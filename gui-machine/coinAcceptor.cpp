#include "mainwindow.h"
#include "coinAcceptor.h"

CoinAcceptor::CoinAcceptor(void) {
    balance = 0;
    change = 0;
    price = 0;
}

CoinAcceptor::CoinAcceptor(int initBalance, int initChange, int initPrice) {
    balance = initBalance;
    change = initChange;
    price = initPrice;
}
int CoinAcceptor::getBalance() {
    return balance;
}
void CoinAcceptor::setBalance(int newBalance) {
    balance = newBalance;
}
void CoinAcceptor::addBalance(int addAmount) {
    balance += addAmount;
}

int CoinAcceptor::getChange() {
    return change;
}

void CoinAcceptor::setChange(int newChange) {
    change = newChange;
}
void CoinAcceptor::addChange(int addAmount) {
    change += addAmount;
}
void CoinAcceptor::subChange(int subAmount) {
    change -= subAmount;
}

int CoinAcceptor::getPrice() {
    return price;
}
void CoinAcceptor::setPrice(int newPrice) {
    price = newPrice;
}

bool CoinAcceptor::checkBalance() {
    return balance >= price;
}

void CoinAcceptor::processMoney(int creditValue) {
    //  processing of input value and change handled here
}