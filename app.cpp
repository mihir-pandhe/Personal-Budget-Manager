#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <map>

using namespace std;

struct Expense
{
    double amount;
    string category;
    string date;
};

struct Income
{
    double amount;
    string source;
    string date;
};

class BudgetManager
{
private:
    vector<Expense> expenses;
    vector<Income> incomes;
    map<string, double> budgetLimits;
    map<string, double> spent;

public:
    void addExpense(double amount, const string &category, const string &date)
    {
        Expense newExpense = {amount, category, date};
        expenses.push_back(newExpense);
        spent[category] += amount;
        cout << "Expense added successfully." << endl;
    }

    void listExpenses() const
    {
        if (expenses.empty())
        {
            cout << "No expenses to display." << endl;
            return;
        }
        cout << left << setw(15) << "Amount" << setw(20) << "Category" << "Date" << endl;
        for (const auto &expense : expenses)
        {
            cout << fixed << setprecision(2) << setw(15) << expense.amount
                 << setw(20) << expense.category << expense.date << endl;
        }
    }

    void addIncome(double amount, const string &source, const string &date)
    {
        Income newIncome = {amount, source, date};
        incomes.push_back(newIncome);
        cout << "Income added successfully." << endl;
    }

    void listIncomes() const
    {
        if (incomes.empty())
        {
            cout << "No incomes to display." << endl;
            return;
        }
        cout << left << setw(15) << "Amount" << setw(20) << "Source" << "Date" << endl;
        for (const auto &income : incomes)
        {
            cout << fixed << setprecision(2) << setw(15) << income.amount
                 << setw(20) << income.source << income.date << endl;
        }
    }

    void setBudget(const string &category, double amount)
    {
        budgetLimits[category] = amount;
        if (spent.find(category) == spent.end())
        {
            spent[category] = 0;
        }
        cout << "Budget set for category '" << category << "' with amount " << amount << endl;
    }

    void trackBudget() const
    {
        cout << left << setw(20) << "Category" << setw(15) << "Budget" << setw(15) << "Spent" << "Remaining" << endl;
        for (const auto &[category, budget] : budgetLimits)
        {
            double spentAmount = spent.at(category);
            double remaining = budget - spentAmount;
            cout << left << setw(20) << category
                 << setw(15) << fixed << setprecision(2) << budget
                 << setw(15) << spentAmount
                 << remaining << endl;
        }
    }

    void generateSummaryReport() const
    {
        double totalIncome = 0;
        double totalExpenses = 0;
        for (const auto &income : incomes)
        {
            totalIncome += income.amount;
        }
        for (const auto &expense : expenses)
        {
            totalExpenses += expense.amount;
        }

        double totalBudget = 0;
        double totalSpent = 0;
        for (const auto &[category, budget] : budgetLimits)
        {
            totalBudget += budget;
            totalSpent += spent.at(category);
        }

        cout << "Summary Report:" << endl;
        cout << "Total Income: " << fixed << setprecision(2) << totalIncome << endl;
        cout << "Total Expenses: " << fixed << setprecision(2) << totalExpenses << endl;
        cout << "Remaining Budget: " << fixed << setprecision(2) << (totalBudget - totalSpent) << endl;
    }
};

int main()
{
    BudgetManager manager;
    int choice;
    double amount;
    string category, source, date;

    while (true)
    {
        cout << "1. Add Expense" << endl;
        cout << "2. List Expenses" << endl;
        cout << "3. Add Income" << endl;
        cout << "4. List Income" << endl;
        cout << "5. Set Budget" << endl;
        cout << "6. Track Budget" << endl;
        cout << "7. Generate Summary Report" << endl;
        cout << "8. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
            cout << "Enter amount: ";
            cin >> amount;
            cin.ignore();
            cout << "Enter category: ";
            getline(cin, category);
            cout << "Enter date (YYYY-MM-DD): ";
            getline(cin, date);
            manager.addExpense(amount, category, date);
            break;
        case 2:
            manager.listExpenses();
            break;
        case 3:
            cout << "Enter amount: ";
            cin >> amount;
            cin.ignore();
            cout << "Enter source: ";
            getline(cin, source);
            cout << "Enter date (YYYY-MM-DD): ";
            getline(cin, date);
            manager.addIncome(amount, source, date);
            break;
        case 4:
            manager.listIncomes();
            break;
        case 5:
            cout << "Enter category: ";
            getline(cin, category);
            cout << "Enter budget amount: ";
            cin >> amount;
            manager.setBudget(category, amount);
            break;
        case 6:
            manager.trackBudget();
            break;
        case 7:
            manager.generateSummaryReport();
            break;
        case 8:
            cout << "Exiting..." << endl;
            return 0;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}
