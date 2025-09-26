#pragma once
#include "Transaction.h"
#include <string>
#include <vector>

class Wallet {
private:
    std::string name;
    WalletType type;
    double balance;
    double creditLimit;
    std::vector<Transaction> transactions;

public:
    Wallet(const std::string& n, WalletType t, double creditLim = 0.0);

    const std::string& getName() const;
    WalletType getType() const;
    double getBalance() const;
    double getCreditLimit() const;
    const std::vector<Transaction>& getTransactions() const;

    void deposit(double amt);
    bool spend(double amt, const std::string& category);
};
