#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <map>
#include <sstream>
#include <ctime>
#include <fstream>
#include <limits>
#include <algorithm>

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
        return date.substr(0, 7);
    }

    void printExpenses(const vector<Expense> &expensesToPrint) const
    {
        if (expensesToPrint.empty())
        {
            cout << "No expenses found." << endl;
            return;
        }
        cout << left << setw(10) << "Amount" << setw(20) << "Category" << "Date" << endl;
        for (const auto &expense : expensesToPrint)
        {
            cout << fixed << setprecision(2) << setw(10) << expense.amount
                 << setw(20) << expense.category << expense.date << endl;
        }
    }

    void printIncomes(const vector<Income> &incomesToPrint) const
    {
        if (incomesToPrint.empty())
        {
            cout << "No income found." << endl;
            return;
        }
        cout << left << setw(10) << "Amount" << setw(20) << "Source" << "Date" << endl;
        for (const auto &income : incomesToPrint)
        {
            cout << fixed << setprecision(2) << setw(10) << income.amount
                 << setw(20) << income.source << income.date << endl;
        }
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
        printExpenses(expenses);
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
        printIncomes(incomes);
    }

    void updateExpense(size_t index, double newAmount, const string &newCategory, const string &newDate)
    {
        if (index >= expenses.size())
        {
            cerr << "Error: Invalid expense index." << endl;
            return;
        }
        if (!validateAmount(newAmount))
        {
            cerr << "Error: Invalid amount." << endl;
            return;
        }
        if (!validateDate(newDate))
        {
            cerr << "Error: Invalid date format. Use YYYY-MM-DD." << endl;
            return;
        }
        Expense &expense = expenses[index];
        string oldCategory = expense.category;
        string oldDate = expense.date;
        expense.amount = newAmount;
        expense.category = newCategory;
        expense.date = newDate;
        spent[oldCategory] -= expense.amount;
        spent[newCategory] += expense.amount;
        string oldMonth = getMonth(oldDate);
        string newMonth = getMonth(newDate);
        monthlyBudget[oldMonth] -= expense.amount;
        monthlyBudget[newMonth] += expense.amount;
        saveData();
        cout << "Expense updated successfully." << endl;
    }

    void deleteExpense(size_t index)
    {
        if (index >= expenses.size())
        {
            cerr << "Error: Invalid expense index." << endl;
            return;
        }
        const Expense &expense = expenses[index];
        spent[expense.category] -= expense.amount;
        string month = getMonth(expense.date);
        monthlyBudget[month] -= expense.amount;
        expenses.erase(expenses.begin() + index);
        saveData();
        cout << "Expense deleted successfully." << endl;
    }

    void updateIncome(size_t index, double newAmount, const string &newSource, const string &newDate)
    {
        if (index >= incomes.size())
        {
            cerr << "Error: Invalid income index." << endl;
            return;
        }
        if (!validateAmount(newAmount))
        {
            cerr << "Error: Invalid amount." << endl;
            return;
        }
        if (!validateDate(newDate))
        {
            cerr << "Error: Invalid date format. Use YYYY-MM-DD." << endl;
            return;
        }
        Income &income = incomes[index];
        income.amount = newAmount;
        income.source = newSource;
        income.date = newDate;
        saveData();
        cout << "Income updated successfully." << endl;
    }

    void deleteIncome(size_t index)
    {
        if (index >= incomes.size())
        {
            cerr << "Error: Invalid income index." << endl;
            return;
        }
        incomes.erase(incomes.begin() + index);
        saveData();
        cout << "Income deleted successfully." << endl;
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
        vector<Expense> filteredExpenses;
        for (const auto &expense : expenses)
        {
            if (expense.category == category)
            {
                filteredExpenses.push_back(expense);
            }
        }
        printExpenses(filteredExpenses);
    }

    void viewIncomeBySource(const string &source) const
    {
        vector<Income> filteredIncomes;
        for (const auto &income : incomes)
        {
            if (income.source == source)
            {
                filteredIncomes.push_back(income);
            }
        }
        printIncomes(filteredIncomes);
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
            cout << "\nPersonal Budget Manager\n";
            cout << "1. Add Expense\n";
            cout << "2. List Expenses\n";
            cout << "3. Update Expense\n";
            cout << "4. Delete Expense\n";
            cout << "5. Add Income\n";
            cout << "6. List Income\n";
            cout << "7. Update Income\n";
            cout << "8. Delete Income\n";
            cout << "9. Set Budget\n";
            cout << "10. Track Budget\n";
            cout << "11. Generate Summary Report\n";
            cout << "12. View Expense by Category\n";
            cout << "13. View Income by Source\n";
            cout << "14. Track Monthly Budget\n";
            cout << "15. Add User Profile\n";
            cout << "16. Switch User Profile\n";
            cout << "0. Exit\n";
            cout << "Choose an option: ";
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
                size_t index;
                double newAmount;
                string newCategory, newDate;
                cout << "Enter expense index, new amount, new category, and new date (YYYY-MM-DD): ";
                cin >> index;
                cin >> newAmount;
                cin.ignore();
                getline(cin, newCategory);
                getline(cin, newDate);
                updateExpense(index, newAmount, newCategory, newDate);
                break;
            }
            case 4:
            {
                size_t index;
                cout << "Enter expense index to delete: ";
                cin >> index;
                deleteExpense(index);
                break;
            }
            case 5:
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
            case 6:
                listIncomes();
                break;
            case 7:
            {
                size_t index;
                double newAmount;
                string newSource, newDate;
                cout << "Enter income index, new amount, new source, and new date (YYYY-MM-DD): ";
                cin >> index;
                cin >> newAmount;
                cin.ignore();
                getline(cin, newSource);
                getline(cin, newDate);
                updateIncome(index, newAmount, newSource, newDate);
                break;
            }
            case 8:
            {
                size_t index;
                cout << "Enter income index to delete: ";
                cin >> index;
                deleteIncome(index);
                break;
            }
            case 9:
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
            case 10:
                trackBudget();
                break;
            case 11:
                generateSummaryReport();
                break;
            case 12:
            {
                string category;
                cout << "Enter category to view expenses: ";
                getline(cin, category);
                viewExpenseByCategory(category);
                break;
            }
            case 13:
            {
                string source;
                cout << "Enter source to view income: ";
                getline(cin, source);
                viewIncomeBySource(source);
                break;
            }
            case 14:
                trackMonthlyBudget();
                break;
            case 15:
            {
                string username;
                cout << "Enter new username: ";
                getline(cin, username);
                addUserProfile(username);
                break;
            }
            case 16:
            {
                string username;
                cout << "Enter username to switch to: ";
                getline(cin, username);
                switchUserProfile(username);
                break;
            }
            case 0:
                return;
            default:
                cout << "Invalid option. Please try again." << endl;
            }
        }
    }
};

int main()
{
    BudgetManager manager;
    string username;
    cout << "Enter your username: ";
    getline(cin, username);
    manager.setUser(username);
    manager.run();
    return 0;
}
