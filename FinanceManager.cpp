#include "FinanceManager.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <map>

using namespace std;

time_t FinanceManager::nowTime() {
    return time(nullptr);
}

time_t FinanceManager::periodStartDays(int days) {
    if (days <= 0) return 0;
    time_t now = nowTime();
    return now - static_cast<time_t>(days) * 24 * 60 * 60;
}

string FinanceManager::formatTime(time_t t) {
    if (t == 0) return string("-");
    tm local_tm;
#if defined(_MSC_VER)
    localtime_s(&local_tm, &t);
#else
    localtime_r(&t, &local_tm);
#endif
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", &local_tm);
    return string(buf);
}

bool FinanceManager::addWallet(const string& name, WalletType type, double creditLimit) {
    if (getWallet(name) != nullptr) return false;
    wallets.emplace_back(name, type, creditLimit);
    return true;
}

Wallet* FinanceManager::getWallet(const string& name) {
    for (auto& w : wallets)
        if (w.getName() == name) return &w;
    return nullptr;
}

const vector<Wallet>& FinanceManager::getAllWallets() const { return wallets; }

void FinanceManager::depositToWallet(const string& name, double amt) {
    Wallet* w = getWallet(name);
    if (!w) { cout << "Wallet not found.\n"; return; }
    w->deposit(amt);
    cout << "Deposit successful. Balance: " << fixed << setprecision(2) << w->getBalance() << "\n";
}

void FinanceManager::spendFromWallet(const string& name, double amt, const string& category) {
    Wallet* w = getWallet(name);
    if (!w) { cout << "Wallet not found.\n"; return; }
    if (w->spend(amt, category)) {
        cout << "Expense added. Balance: " << fixed << setprecision(2) << w->getBalance() << "\n";
    }
    else {
        cout << "Insufficient funds or credit limit.\n";
    }
}

void FinanceManager::showAllTransactions(int days) const {
    time_t start = (days > 0) ? periodStartDays(days) : 0;
    bool any = false;
    for (const auto& w : wallets) {
        cout << "\nWallet: " << w.getName()
            << " | Type: " << (w.getType() == WalletType::DEBIT ? "DEBIT" : "CREDIT")
            << " | Balance: " << fixed << setprecision(2) << w.getBalance() << "\n";
        const auto& tr = w.getTransactions();
        bool empty = true;
        for (const auto& t : tr) {
            if (start == 0 || t.date >= start) {
                cout << (t.isExpense ? "Expense" : "Deposit")
                    << " | Wallet: " << t.walletName
                    << " | Category: " << t.category
                    << " | Amount: " << fixed << setprecision(2) << t.amount
                    << " | Date: " << formatTime(t.date) << "\n";
                empty = false;
                any = true;
            }
        }
        if (empty) cout << "  No transactions for the selected period.\n";
    }
    if (!any) cout << "\nNo transactions found for the selected period.\n";
}

void FinanceManager::saveReportToFile(const string& filename, int days) const {
    ofstream fout(filename);
    if (!fout) {
        cout << "Failed to open file for writing\n";
        return;
    }
    time_t start = (days > 0) ? periodStartDays(days) : 0;
    fout << "REPORT (period days: " << days << ")\n";
    for (const auto& w : wallets) {
        fout << "\nWallet: " << w.getName() << " | Type: "
            << (w.getType() == WalletType::DEBIT ? "DEBIT" : "CREDIT")
            << " | Balance: " << fixed << setprecision(2) << w.getBalance() << "\n";
        const auto& tr = w.getTransactions();
        bool empty = true;
        for (const auto& t : tr) {
            if (start == 0 || t.date >= start) {
                fout << (t.isExpense ? "Expense" : "Deposit")
                    << " | Wallet: " << t.walletName
                    << " | Category: " << t.category
                    << " | Amount: " << fixed << setprecision(2) << t.amount
                    << " | Date: " << formatTime(t.date) << "\n";
                empty = false;
            }
        }
        if (empty) fout << "  No transactions for the selected period.\n";
    }
    fout.close();
    cout << "Report saved to file: " << filename << "\n";
}

vector<Transaction> FinanceManager::collectTransactions(int days) const {
    time_t start = (days > 0) ? periodStartDays(days) : 0;
    vector<Transaction> all;
    for (const auto& w : wallets) {
        for (const auto& t : w.getTransactions()) {
            if (start == 0 || t.date >= start) all.push_back(t);
        }
    }
    return all;
}

vector<Transaction> FinanceManager::topExpenses(int days, int topN) const {
    auto all = collectTransactions(days);
    vector<Transaction> expenses;
    for (const auto& t : all) if (t.isExpense) expenses.push_back(t);
    sort(expenses.begin(), expenses.end(), [](const Transaction& a, const Transaction& b) {
        return a.amount > b.amount;
        });
    if ((int)expenses.size() > topN) expenses.resize(topN);
    return expenses;
}

vector<pair<string, double>> FinanceManager::topCategories(int days, int topN) const {
    auto all = collectTransactions(days);
    map<string, double> sums;
    for (const auto& t : all) if (t.isExpense) sums[t.category] += t.amount;
    vector<pair<string, double>> vec(sums.begin(), sums.end());
    sort(vec.begin(), vec.end(), [](const pair<string, double>& a, const pair<string, double>& b) {
        return a.second > b.second;
        });
    if ((int)vec.size() > topN) vec.resize(topN);
    return vec;
}

void FinanceManager::saveTopExpensesToFile(const string& filename, int days, int topN) const {
    ofstream fout(filename);
    if (!fout) { cout << "Failed to open file for writing\n"; return; }
    fout << "TOP-" << topN << " EXPENSES (last " << days << " days)\n";
    auto top = topExpenses(days, topN);
    if (top.empty()) fout << "No expenses for the period.\n";
    for (size_t i = 0; i < top.size(); ++i) {
        fout << i + 1 << ". Wallet: " << top[i].walletName
            << " | Category: " << top[i].category
            << " | Amount: " << fixed << setprecision(2) << top[i].amount
            << " | Date: " << formatTime(top[i].date) << "\n";
    }
    fout.close();
    cout << "Top expenses saved to file: " << filename << "\n";
}

void FinanceManager::saveTopCategoriesToFile(const string& filename, int days, int topN) const {
    ofstream fout(filename);
    if (!fout) { cout << "Failed to open file for writing\n"; return; }
    fout << "TOP-" << topN << " CATEGORIES (last " << days << " days)\n";
    auto top = topCategories(days, topN);
    if (top.empty()) fout << "No expenses for the period.\n";
    for (size_t i = 0; i < top.size(); ++i) {
        fout << i + 1 << ". Category: " << top[i].first
            << " | Sum: " << fixed << setprecision(2) << top[i].second << "\n";
    }
    fout.close();
    cout << "Top categories saved to file: " << filename << "\n";
}

void FinanceManager::printTopExpensesConsole(int days, int topN) const {
    auto top = topExpenses(days, topN);
    cout << "\n== TOP-" << topN << " expenses (last " << (days == 0 ? "all time" : to_string(days) + " days") << ") ==\n";
    if (top.empty()) { cout << "No expenses for the period.\n"; return; }
    for (size_t i = 0; i < top.size(); ++i) {
        cout << i + 1 << ". Wallet: " << top[i].walletName
            << " | Category: " << top[i].category
            << " | Amount: " << fixed << setprecision(2) << top[i].amount
            << " | Date: " << formatTime(top[i].date) << "\n";
    }
}

void FinanceManager::printTopCategoriesConsole(int days, int topN) const {
    auto top = topCategories(days, topN);
    cout << "\n== TOP-" << topN << " categories (last " << (days == 0 ? "all time" : to_string(days) + " days") << ") ==\n";
    if (top.empty()) { cout << "No expenses for the period.\n"; return; }
    for (size_t i = 0; i < top.size(); ++i) {
        cout << i + 1 << ". Category: " << top[i].first
            << " | Sum: " << fixed << setprecision(2) << top[i].second << "\n";
    }
}
