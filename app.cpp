#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <map>
#include <sstream>
#include <ctime>
#include <fstream>
#include <limits>

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
        for (auto it = budgetLimits.begin(); it != budgetLimits.end(); ++it)
        {
            outFile << it->first << "," << it->second << endl;
        }

        outFile << spent.size() << endl;
        for (auto it = spent.begin(); it != spent.end(); ++it)
        {
            outFile << it->first << "," << it->second << endl;
        }

        outFile << monthlyBudget.size() << endl;
        for (auto it = monthlyBudget.begin(); it != monthlyBudget.end(); ++it)
        {
            outFile << it->first << "," << it->second << endl;
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

        size_t numExpenses;
        if (!(inFile >> numExpenses))
        {
            cerr << "Error: Failed to read number of expenses." << endl;
            return;
        }
        inFile.ignore();
        expenses.clear();
        for (size_t i = 0; i < numExpenses; ++i)
        {
            Expense expense;
            string line;
            getline(inFile, line);
            stringstream ss(line);
            if (!(ss >> expense.amount))
            {
                cerr << "Error: Failed to read expense amount." << endl;
                return;
            }
            getline(ss, expense.category, ',');
            getline(ss, expense.date, ',');
            expenses.push_back(expense);
        }

        size_t numIncomes;
        if (!(inFile >> numIncomes))
        {
            cerr << "Error: Failed to read number of incomes." << endl;
            return;
        }
        inFile.ignore();
        incomes.clear();
        for (size_t i = 0; i < numIncomes; ++i)
        {
            Income income;
            string line;
            getline(inFile, line);
            stringstream ss(line);
            if (!(ss >> income.amount))
            {
                cerr << "Error: Failed to read income amount." << endl;
                return;
            }
            getline(ss, income.source, ',');
            getline(ss, income.date, ',');
            incomes.push_back(income);
        }

        size_t numBudgetLimits;
        if (!(inFile >> numBudgetLimits))
        {
            cerr << "Error: Failed to read number of budget limits." << endl;
            return;
        }
        inFile.ignore();
        budgetLimits.clear();
        for (size_t i = 0; i < numBudgetLimits; ++i)
        {
            string line;
            getline(inFile, line);
            stringstream ss(line);
            string category;
            double limit;
            if (!(getline(ss, category, ',') && (ss >> limit)))
            {
                cerr << "Error: Failed to read budget limit." << endl;
                return;
            }
            budgetLimits[category] = limit;
        }

        size_t numSpent;
        if (!(inFile >> numSpent))
        {
            cerr << "Error: Failed to read number of spent entries." << endl;
            return;
        }
        inFile.ignore();
        spent.clear();
        for (size_t i = 0; i < numSpent; ++i)
        {
            string line;
            getline(inFile, line);
            stringstream ss(line);
            string category;
            double amount;
            if (!(getline(ss, category, ',') && (ss >> amount)))
            {
                cerr << "Error: Failed to read spent amount." << endl;
                return;
            }
            spent[category] = amount;
        }

        size_t numMonthlyBudget;
        if (!(inFile >> numMonthlyBudget))
        {
            cerr << "Error: Failed to read number of monthly budgets." << endl;
            return;
        }
        inFile.ignore();
        monthlyBudget.clear();
        for (size_t i = 0; i < numMonthlyBudget; ++i)
        {
            string line;
            getline(inFile, line);
            stringstream ss(line);
            string month;
            double amount;
            if (!(getline(ss, month, ',') && (ss >> amount)))
            {
                cerr << "Error: Failed to read monthly budget." << endl;
                return;
            }
            monthlyBudget[month] = amount;
        }

        inFile.close();
    }

    bool validateDate(const string &date) const
    {
        if (date.length() != 10 || date[4] != '-' || date[7] != '-')
        {
            return false;
        }
        for (char c : date)
        {
            if (!isdigit(c) && c != '-')
            {
                return false;
            }
        }
        return true;
    }

    bool validateAmount(double amount) const
    {
        return amount > 0;
    }

    static string getMonth(const string &date)
    {
        return date.substr(0, 7); // YYYY-MM
    }

public:
    void setUser(const string &username)
    {
        currentUser = username;
        loadData();
    }

    void addExpense(double amount, const string &category, const string &date)
    {
        if (!validateAmount(amount))
        {
            cerr << "Error: Invalid amount." << endl;
            return;
        }
        if (!validateDate(date))
        {
            cerr << "Error: Invalid date format. Use YYYY-MM-DD." << endl;
            return;
        }
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
            cout << "No expenses found." << endl;
            return;
        }
        cout << left << setw(10) << "Amount" << setw(20) << "Category" << "Date" << endl;
        for (const auto &expense : expenses)
        {
            cout << fixed << setprecision(2) << setw(10) << expense.amount
                 << setw(20) << expense.category << expense.date << endl;
        }
    }

    void addIncome(double amount, const string &source, const string &date)
    {
        if (!validateAmount(amount))
        {
            cerr << "Error: Invalid amount." << endl;
            return;
        }
        if (!validateDate(date))
        {
            cerr << "Error: Invalid date format. Use YYYY-MM-DD." << endl;
            return;
        }
        Income newIncome = {amount, source, date};
        incomes.push_back(newIncome);
        saveData();
        cout << "Income added successfully." << endl;
    }

    void listIncomes() const
    {
        if (incomes.empty())
        {
            cout << "No income found." << endl;
            return;
        }
        cout << left << setw(10) << "Amount" << setw(20) << "Source" << "Date" << endl;
        for (const auto &income : incomes)
        {
            cout << fixed << setprecision(2) << setw(10) << income.amount
                 << setw(20) << income.source << income.date << endl;
        }
    }

    void setBudget(const string &category, double amount)
    {
        if (!validateAmount(amount))
        {
            cerr << "Error: Invalid budget amount." << endl;
            return;
        }
        budgetLimits[category] = amount;
        saveData();
        cout << "Budget set successfully." << endl;
    }

    void trackBudget() const
    {
        cout << left << setw(20) << "Category" << "Budget" << setw(15) << "Spent" << "Remaining" << endl;
        for (auto it = budgetLimits.begin(); it != budgetLimits.end(); ++it)
        {
            const string &category = it->first;
            double budget = it->second;
            double spentAmount = spent.at(category);
            double remaining = budget - spentAmount;
            cout << left << setw(20) << category
                 << fixed << setprecision(2) << setw(15) << budget
                 << setw(15) << spentAmount
                 << setw(15) << remaining << endl;
        }
    }

    void generateSummaryReport() const
    {
        double totalIncome = 0;
        for (const auto &income : incomes)
        {
            totalIncome += income.amount;
        }

        double totalExpenses = 0;
        for (const auto &expense : expenses)
        {
            totalExpenses += expense.amount;
        }

        cout << fixed << setprecision(2);
        cout << "Total Income: " << totalIncome << endl;
        cout << "Total Expenses: " << totalExpenses << endl;
        cout << "Remaining Budget: " << totalIncome - totalExpenses << endl;
    }

    void viewExpenseByCategory(const string &category) const
    {
        bool found = false;
        for (const auto &expense : expenses)
        {
            if (expense.category == category)
            {
                cout << fixed << setprecision(2) << "Amount: " << expense.amount
                     << ", Date: " << expense.date << endl;
                found = true;
            }
        }
        if (!found)
        {
            cout << "No expenses found for category '" << category << "'." << endl;
        }
    }

    void viewIncomeBySource(const string &source) const
    {
        bool found = false;
        for (const auto &income : incomes)
        {
            if (income.source == source)
            {
                cout << fixed << setprecision(2) << "Amount: " << income.amount
                     << ", Date: " << income.date << endl;
                found = true;
            }
        }
        if (!found)
        {
            cout << "No income found from source '" << source << "'." << endl;
        }
    }

    void trackMonthlyBudget() const
    {
        cout << left << setw(10) << "Month" << "Budget" << setw(20) << "Expenses" << "Remaining Budget" << endl;
        for (auto it = monthlyBudget.begin(); it != monthlyBudget.end(); ++it)
        {
            const string &month = it->first;
            double budget = it->second;
            double totalExpenses = 0;
            for (const auto &expense : expenses)
            {
                if (getMonth(expense.date) == month)
                {
                    totalExpenses += expense.amount;
                }
            }
            double remainingBudget = budget - totalExpenses;
            cout << left << setw(10) << month
                 << fixed << setprecision(2) << setw(20) << budget
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
            cerr << "Error: Authentication failed for user: " << username << endl;
        }
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
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            switch (choice)
            {
            case 1:
            {
                double amount;
                string category, date;
                cout << "Enter amount, category, and date (YYYY-MM-DD): ";
                cin >> amount;
                cin.ignore();
                getline(cin, category);
                getline(cin, date);
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
                cin >> amount;
                cin.ignore();
                getline(cin, source);
                getline(cin, date);
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
                cin.ignore();
                getline(cin, category);
                cin >> amount;
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
                cin.ignore();
                getline(cin, category);
                viewExpenseByCategory(category);
                break;
            }
            case 9:
            {
                string source;
                cout << "Enter source to view income: ";
                cin.ignore();
                getline(cin, source);
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
                cerr << "Error: Invalid choice. Please try again." << endl;
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
