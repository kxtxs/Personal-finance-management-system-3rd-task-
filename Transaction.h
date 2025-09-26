#pragma once
#include <string>
#include <ctime>

enum class WalletType { DEBIT, CREDIT };

struct Transaction {
    std::string category;
    double amount;
    time_t date;
    bool isExpense;
    std::string walletName;

    Transaction() = default;
    Transaction(const std::string& wname, const std::string& cat, double amt, bool expense)
        : category(cat), amount(amt), date(time(nullptr)), isExpense(expense), walletName(wname) {
    }
};
