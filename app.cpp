#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Expense
{
    string description;
    double amount;
    string category;
    string date;
};

class BudgetManager
{
private:
    vector<Expense> expenses;

public:
    void addExpense(const string &description, double amount, const string &category, const string &date)
    {
        Expense newExpense{description, amount, category, date};
        expenses.push_back(newExpense);
        cout << "Expense added successfully." << endl;
    }

    void listExpenses() const
    {
        if (expenses.empty())
        {
            cout << "No expenses recorded." << endl;
            return;
        }

        for (const auto &expense : expenses)
        {
            cout << "Description: " << expense.description
                 << ", Amount: $" << expense.amount
                 << ", Category: " << expense.category
                 << ", Date: " << expense.date << endl;
        }
    }
};

int main()
{
    BudgetManager budgetManager;
    int choice;
    string description, category, date;
    double amount;

    while (true)
    {
        cout << "1. Add Expense" << endl;
        cout << "2. List Expenses" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
            cout << "Enter description: ";
            getline(cin, description);
            cout << "Enter amount: ";
            cin >> amount;
            cin.ignore();
            cout << "Enter category: ";
            getline(cin, category);
            cout << "Enter date (YYYY-MM-DD): ";
            getline(cin, date);
            budgetManager.addExpense(description, amount, category, date);
            break;
        case 2:
            budgetManager.listExpenses();
            break;
        case 3:
            cout << "Exiting..." << endl;
            return 0;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}
