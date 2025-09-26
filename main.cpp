#include <iostream>
#include <string>
#include <limits>
#include "FinanceManager.h"

using namespace std;

int choosePeriodDays() {
    cout << "\nChoose period:\n1) Day\n2) Week\n3) Month\n0) All time\nYour choice: ";
    int ch;
    if (!(cin >> ch)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); return 0; }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    switch (ch) {
    case 1: return 1;
    case 2: return 7;
    case 3: return 30;
    default: return 0;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    FinanceManager fm;
    cout << "=== Personal Finance Management System ===\n";

    // sample wallets (you can remove or change)
    fm.addWallet("Cash", WalletType::DEBIT);
    fm.addWallet("VISA_Card", WalletType::DEBIT);
    fm.addWallet("Credit_MC", WalletType::CREDIT, 1000.0);

    while (true) {
        cout << "\n--- MENU ---\n";
        cout << "1. Add wallet/card\n";
        cout << "2. Deposit to wallet/card\n";
        cout << "3. Add expense\n";
        cout << "4. Show all transactions (by period)\n";
        cout << "5. Show TOP-3 expenses (week/month)\n";
        cout << "6. Show TOP-3 categories (week/month)\n";
        cout << "7. Save report to file\n";
        cout << "8. Save TOP-3 expenses to file (week/month)\n";
        cout << "9. Save TOP-3 categories to file (week/month)\n";
        cout << "0. Exit\n";
        cout << "Choice: ";

        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 0) break;

        if (choice == 1) {
            string name;
            cout << "Enter wallet/card name: ";
            getline(cin, name);
            cout << "Type (1=DEBIT, 2=CREDIT): ";
            int t; cin >> t; cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (t == 1) {
                if (fm.addWallet(name, WalletType::DEBIT)) cout << "DEBIT wallet added\n";
                else cout << "Wallet with this name already exists\n";
            }
            else {
                double lim; cout << "Enter credit limit (e.g. 1000): "; cin >> lim; cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (fm.addWallet(name, WalletType::CREDIT, lim)) cout << "CREDIT card added\n";
                else cout << "Wallet with this name already exists\n";
            }
        }
        else if (choice == 2) {
            string name; double amt;
            cout << "Wallet/card name to deposit into: "; getline(cin, name);
            cout << "Deposit amount: "; cin >> amt; cin.ignore(numeric_limits<streamsize>::max(), '\n');
            fm.depositToWallet(name, amt);
        }
        else if (choice == 3) {
            string name, cat; double amt;
            cout << "Wallet/card name for expense: "; getline(cin, name);
            cout << "Expense amount: "; cin >> amt; cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Expense category (e.g. Groceries, Transport): "; getline(cin, cat);
            fm.spendFromWallet(name, amt, cat);
        }
        else if (choice == 4) {
            int days = choosePeriodDays();
            fm.showAllTransactions(days);
        }
        else if (choice == 5) {
            cout << "1) Week  2) Month  0) All time\nYour choice: ";
            int ch; cin >> ch; cin.ignore(numeric_limits<streamsize>::max(), '\n');
            int days = (ch == 1 ? 7 : (ch == 2 ? 30 : 0));
            fm.printTopExpensesConsole(days, 3);
        }
        else if (choice == 6) {
            cout << "1) Week  2) Month  0) All time\nYour choice: ";
            int ch; cin >> ch; cin.ignore(numeric_limits<streamsize>::max(), '\n');
            int days = (ch == 1 ? 7 : (ch == 2 ? 30 : 0));
            fm.printTopCategoriesConsole(days, 3);
        }
        else if (choice == 7) {
            int days = choosePeriodDays();
            string filename;
            cout << "Filename for report (e.g. report.txt): ";
            getline(cin, filename);
            fm.saveReportToFile(filename, days);
        }
        else if (choice == 8) {
            cout << "1) Week  2) Month  0) All time\nYour choice: ";
            int ch; cin >> ch; cin.ignore(numeric_limits<streamsize>::max(), '\n');
            int days = (ch == 1 ? 7 : (ch == 2 ? 30 : 0));
            string filename;
            cout << "Filename for TOP expenses (e.g. top_exp.txt): ";
            getline(cin, filename);
            fm.saveTopExpensesToFile(filename, days, 3);
        }
        else if (choice == 9) {
            cout << "1) Week  2) Month  0) All time\nYour choice: ";
            int ch; cin >> ch; cin.ignore(numeric_limits<streamsize>::max(), '\n');
            int days = (ch == 1 ? 7 : (ch == 2 ? 30 : 0));
            string filename;
            cout << "Filename for TOP categories (e.g. top_cat.txt): ";
            getline(cin, filename);
            fm.saveTopCategoriesToFile(filename, days, 3);
        }
        else {
            cout << "Unknown command\n";
        }
    }

    cout << "Thank you! Goodbye.\n";
    return 0;
}
