#pragma once
#include "Wallet.h"
#include "Transaction.h"
#include <vector>
#include <string>
#include <utility>

class FinanceManager {
private:
    std::vector<Wallet> wallets;

    static time_t nowTime();
    static time_t periodStartDays(int days);
    static std::string formatTime(time_t t);

public:
    bool addWallet(const std::string& name, WalletType type, double creditLimit = 0.0);
    Wallet* getWallet(const std::string& name);
    const std::vector<Wallet>& getAllWallets() const;

    void depositToWallet(const std::string& name, double amt);
    void spendFromWallet(const std::string& name, double amt, const std::string& category);

    void showAllTransactions(int days = 0) const;
    void saveReportToFile(const std::string& filename, int days = 0) const;

    std::vector<Transaction> collectTransactions(int days = 0) const;
    std::vector<Transaction> topExpenses(int days = 0, int topN = 3) const;
    std::vector<std::pair<std::string, double>> topCategories(int days = 0, int topN = 3) const;

    void saveTopExpensesToFile(const std::string& filename, int days = 0, int topN = 3) const;
    void saveTopCategoriesToFile(const std::string& filename, int days = 0, int topN = 3) const;

    void printTopExpensesConsole(int days = 0, int topN = 3) const;
    void printTopCategoriesConsole(int days = 0, int topN = 3) const;
};
