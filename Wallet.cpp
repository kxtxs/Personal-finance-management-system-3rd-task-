#include "Wallet.h"

Wallet::Wallet(const std::string& n, WalletType t, double creditLim)
    : name(n), type(t), balance(0.0), creditLimit(creditLim) {
}

const std::string& Wallet::getName() const { return name; }
WalletType Wallet::getType() const { return type; }
double Wallet::getBalance() const { return balance; }
double Wallet::getCreditLimit() const { return creditLimit; }
const std::vector<Transaction>& Wallet::getTransactions() const { return transactions; }

void Wallet::deposit(double amt) {
    if (amt <= 0) return;
    balance += amt;
    transactions.emplace_back(name, "Deposit", amt, false);
}

bool Wallet::spend(double amt, const std::string& category) {
    if (amt <= 0) return false;
    if (type == WalletType::DEBIT) {
        if (amt > balance) return false;
        balance -= amt;
        transactions.emplace_back(name, category, amt, true);
        return true;
    }
    else { // CREDIT
        if (balance - amt < -creditLimit) return false;
        balance -= amt;
        transactions.emplace_back(name, category, amt, true);
        return true;
    }
}
