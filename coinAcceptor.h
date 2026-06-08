#ifndef COINACCEPTOR_H
#define COINACCEPTOR_H

class CoinAcceptor {
public:
    CoinAcceptor();
    CoinAcceptor(int initBalance, int initChange, int initPrice);
    int getBalance();
    void setBalance(int newBalance);
    void addBalance(int addAmount);

    int getChange();
    void setChange(int newChange);
    void addChange(int addAmount);
    void subChange(int subAmount);

    int getPrice();
    void setPrice(int newPrice);

    bool checkBalance();

    void processMoney(int creditValue);


private:
    int balance;
    int change;
    int price;
};

#endif