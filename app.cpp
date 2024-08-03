#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <map>
#include <sstream>
#include <ctime>
#include <fstream>

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
    map<string, double> monthlyBudget;
    string currentUser;

    void saveData() const
    {
        ofstream outFile(currentUser + ".dat");
        if (!outFile)
        {
            cerr << "Error: Unable to open file for saving data." << endl;
            return;
        }
        outFile << expenses.size() << endl;
        for (const auto &expense : expenses)
        {
            outFile << expense.amount << "," << expense.category << "," << expense.date << endl;
        }
        outFile << incomes.size() << endl;
        for (const auto &income : incomes)
        {
            outFile << income.amount << "," << income.source << "," << income.date << endl;
        }
        outFile << budgetLimits.size() << endl;
        for (const auto &[category, limit] : budgetLimits)
        {
            outFile << category << "," << limit << endl;
        }
        outFile << spent.size() << endl;
        for (const auto &[category, amount] : spent)
        {
            outFile << category << "," << amount << endl;
        }
        outFile << monthlyBudget.size() << endl;
        for (const auto &[month, amount] : monthlyBudget)
        {
            outFile << month << "," << amount << endl;
        }
        outFile.close();
    }

    void loadData()
    {
        ifstream inFile(currentUser + ".dat");
        if (!inFile)
        {
            cerr << "Error: Unable to open file for loading data." << endl;
            return;
        }
        // Load expenses
        size_t numExpenses;
        inFile >> numExpenses;
        inFile.ignore();
        expenses.clear();
        for (size_t i = 0; i < numExpenses; ++i)
        {
            Expense expense;
            string line;
            getline(inFile, line);
            stringstream ss(line);
            getline(ss, line, ',');
            expense.amount = stod(line);
            getline(ss, expense.category, ',');
            getline(ss, expense.date, ',');
            expenses.push_back(expense);
        }
        // Load incomes
        size_t numIncomes;
        inFile >> numIncomes;
        inFile.ignore();
        incomes.clear();
        for (size_t i = 0; i < numIncomes; ++i)
        {
            Income income;
            string line;
            getline(inFile, line);
            stringstream ss(line);
            getline(ss, line, ',');
            income.amount = stod(line);
            getline(ss, income.source, ',');
            getline(ss, income.date, ',');
            incomes.push_back(income);
        }
        size_t numBudgetLimits;
        inFile >> numBudgetLimits;
        inFile.ignore();
        budgetLimits.clear();
        for (size_t i = 0; i < numBudgetLimits; ++i)
        {
            string line;
            getline(inFile, line);
            stringstream ss(line);
            string category;
            double limit;
            getline(ss, category, ',');
            ss >> limit;
            budgetLimits[category] = limit;
        }
        size_t numSpent;
        inFile >> numSpent;
        inFile.ignore();
        spent.clear();
        for (size_t i = 0; i < numSpent; ++i)
        {
            string line;
            getline(inFile, line);
            stringstream ss(line);
            string category;
            double amount;
            getline(ss, category, ',');
            ss >> amount;
            spent[category] = amount;
        }
        size_t numMonthlyBudget;
        inFile >> numMonthlyBudget;
        inFile.ignore();
        monthlyBudget.clear();
        for (size_t i = 0; i < numMonthlyBudget; ++i)
        {
            string line;
            getline(inFile, line);
            stringstream ss(line);
            string month;
            double amount;
            getline(ss, month, ',');
            ss >> amount;
            monthlyBudget[month] = amount;
        }
        inFile.close();
    }

public:
    void setUser(const string &username)
    {
        currentUser = username;
        loadData();
    }

    void addExpense(double amount, const string &category, const string &date)
    {
        Expense newExpense = {amount, category, date};
        expenses.push_back(newExpense);
        spent[category] += amount;
        string month = getMonth(date);
        monthlyBudget[month] += amount;
        saveData();
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
        string month = getMonth(date);
        monthlyBudget[month] += amount;
        saveData();
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
        saveData();
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

    void viewExpenseByCategory(const string &category) const
    {
        bool found = false;
        for (const auto &expense : expenses)
        {
            if (expense.category == category)
            {
                cout << "Amount: " << fixed << setprecision(2) << expense.amount
                     << ", Date: " << expense.date << endl;
                found = true;
            }
        }
        if (!found)
        {
            cout << "No expenses found for category: " << category << endl;
        }
    }

    void viewIncomeBySource(const string &source) const
    {
        bool found = false;
        for (const auto &income : incomes)
        {
            if (income.source == source)
            {
                cout << "Amount: " << fixed << setprecision(2) << income.amount
                     << ", Date: " << income.date << endl;
                found = true;
            }
        }
        if (!found)
        {
            cout << "No income found from source: " << source << endl;
        }
    }

    void trackMonthlyBudget() const
    {
        cout << left << setw(10) << "Month" << "Income" << setw(20) << "Expenses" << "Remaining Budget" << endl;
        for (const auto &[month, amount] : monthlyBudget)
        {
            double totalExpenses = 0;
            for (const auto &expense : expenses)
            {
                if (getMonth(expense.date) == month)
                {
                    totalExpenses += expense.amount;
                }
            }
            double remainingBudget = amount - totalExpenses;
            cout << left << setw(10) << month
                 << fixed << setprecision(2) << setw(20) << amount
                 << totalExpenses << setw(15) << remainingBudget << endl;
        }
    }

    void addUserProfile(const string &username)
    {
        currentUser = username;
        saveData();
        cout << "User profile added: " << username << endl;
    }

    bool authenticateUser(const string &username) const
    {
        return username == currentUser;
    }

    void switchUserProfile(const string &username)
    {
        if (authenticateUser(username))
        {
            currentUser = username;
            loadData();
            cout << "Switched to user profile: " << username << endl;
        }
        else
        {
            cout << "Authentication failed for user: " << username << endl;
        }
    }

    static string getMonth(const string &date)
    {
        return date.substr(0, 7);
    }

    void run()
    {
        while (true)
        {
            cout << "Personal Budget Manager" << endl;
            cout << "1. Add Expense" << endl;
            cout << "2. List Expenses" << endl;
            cout << "3. Add Income" << endl;
            cout << "4. List Income" << endl;
            cout << "5. Set Budget" << endl;
            cout << "6. Track Budget" << endl;
            cout << "7. Generate Summary Report" << endl;
            cout << "8. View Expense by Category" << endl;
            cout << "9. View Income by Source" << endl;
            cout << "10. Track Monthly Budget" << endl;
            cout << "11. Add User Profile" << endl;
            cout << "12. Switch User Profile" << endl;
            cout << "0. Exit" << endl;
            int choice;
            cin >> choice;
            switch (choice)
            {
            case 1:
            {
                double amount;
                string category, date;
                cout << "Enter amount, category, and date (YYYY-MM-DD): ";
                cin >> amount >> category >> date;
                addExpense(amount, category, date);
                break;
            }
            case 2:
                listExpenses();
                break;
            case 3:
            {
                double amount;
                string source, date;
                cout << "Enter amount, source, and date (YYYY-MM-DD): ";
                cin >> amount >> source >> date;
                addIncome(amount, source, date);
                break;
            }
            case 4:
                listIncomes();
                break;
            case 5:
            {
                string category;
                double amount;
                cout << "Enter category and budget amount: ";
                cin >> category >> amount;
                setBudget(category, amount);
                break;
            }
            case 6:
                trackBudget();
                break;
            case 7:
                generateSummaryReport();
                break;
            case 8:
            {
                string category;
                cout << "Enter category to view expenses: ";
                cin >> category;
                viewExpenseByCategory(category);
                break;
            }
            case 9:
            {
                string source;
                cout << "Enter source to view income: ";
                cin >> source;
                viewIncomeBySource(source);
                break;
            }
            case 10:
                trackMonthlyBudget();
                break;
            case 11:
            {
                string username;
                cout << "Enter new username: ";
                cin >> username;
                addUserProfile(username);
                break;
            }
            case 12:
            {
                string username;
                cout << "Enter username to switch to: ";
                cin >> username;
                switchUserProfile(username);
                break;
            }
            case 0:
                return;
            default:
                cout << "Invalid choice. Please try again." << endl;
            }
        }
    }
};

int main()
{
    BudgetManager bm;
    string username;
    cout << "Enter username to start: ";
    cin >> username;
    bm.setUser(username);
    bm.run();
    return 0;
}
