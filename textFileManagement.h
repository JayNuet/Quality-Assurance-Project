/*
CSC 455 Project 1
Names: Jabez Nuetey, Michael Gerges, Shabura Daniel
*/

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include "data.h"

using namespace std;

class Reader
{
public:
    vector<Structures::Customer> customerData;
    vector<Structures::Product> productData;
    vector<Structures::Transaction> transactionData;



    /**
     * reads in customer data from a file
     * @param parsefile the file to read from
     * @return a vector of customer objects
    */
    vector<Structures::Customer> customerList(ifstream &parseFile)
    {
        string line;
        vector<Structures::Customer> customers;
        if (parseFile.is_open())
        {
            Structures::Customer readCustomer = Structures::Customer();
            while (getline(parseFile, line))
            {
                if (regex_search(line, customerIdRegex))
                {
                    readCustomer.id = getLastValue(line);
                }
                else if (regex_search(line, customerUsernameRegex))
                {
                    readCustomer.username = getLastValue(line);
                }
                else if (regex_search(line, customerFNameRegex))
                {
                    readCustomer.fname = getLastValue(line);
                }
                else if (regex_search(line, customerLNameRegex))
                {
                    readCustomer.lname = getLastValue(line);
                }
                else if (regex_search(line, customerDobRegex))
                {
                    readCustomer.dob = getLastValue(line);
                }
                else if (regex_search(line, customerRewardRegex))
                {
                    readCustomer.rewardPoints = stoi(getLastValue(line));
                }
                else if (regex_search(line, customerccNumRegex))
                {
                    readCustomer.ccNum = getLastValue(line);
                }
                else
                {
                    // New line, start next object parse
                    customers.push_back(readCustomer);
                    readCustomer = Structures::Customer();
                }
            }
            parseFile.close();
        }
        else
        {
            cout << "Error: attempted to access an unopened file" << endl;
        }
        return customers;
    }


    /**
     * reads in transaction data from a file
     * @param parsefile the file to read from
     * @return a vector of transaction objects
    */
    vector<Structures::Transaction> transactionList(ifstream &parseFile)
    {
        string line;
        vector<Structures::Transaction> transactions;
        if (parseFile.is_open())
        {
            Structures::Transaction readTransaction = Structures::Transaction();
            while (getline(parseFile, line))
            {
                if (regex_search(line, transactionIdRegex))
                {
                    readTransaction.id = getLastValue(line);
                }
                else if (regex_search(line, transactionUserRegex))
                {
                    readTransaction.customerId = getLastValue(line);
                }
                else if (regex_search(line, transactionProductRegex))
                {
                    readTransaction.productIds = parseTransactionProducts(line);
                }
                else if (regex_search(line, transactionTotalRegex))
                {
                    readTransaction.total = stod(getLastValue(line));
                }
                else if (regex_search(line, transactionRewardsRegex))
                {
                    readTransaction.pointsAwarded = stoi(getLastValue(line));
                }
                else
                {
                    // New line, start next object parse
                    transactions.push_back(readTransaction);
                    readTransaction = Structures::Transaction();
                }
            }
            parseFile.close();
        }
        else
        {
            cout << "Error: attempted to access an unopened file" << endl;
        }
        return transactions;
    }



    /**
     * reads in product data from a file
     * @param parsefile the file to read from
     * @return a vector of product objects
     * 
    */
    vector<Structures::Product> productList(ifstream &parseFile)
    {
        string line;
        vector<Structures::Product> products;
        if (parseFile.is_open())
        {
            Structures::Product readProduct = Structures::Product();
            while (getline(parseFile, line))
            {
                if (regex_search(line, productIdRegex))
                {
                    readProduct.id = getLastValue(line);
                }
                else if (regex_search(line, productNameRegex))
                {
                    readProduct.name = getLastValue(line);
                }
                else if (regex_search(line, productPriceRegex))
                {
                    readProduct.price = stod(getLastValue(line));
                }
                else if (regex_search(line, productAvailableRegex))
                {
                    readProduct.availableItems = stoi(getLastValue(line));
                }
                else if (regex_search(line, productPointsAwardedRegex))
                {
                    readProduct.rewardAmount = stoi(getLastValue(line));
                }
                else
                {
                    // New line, start next object parse
                    products.push_back(readProduct);
                    readProduct = Structures::Product();
                }
            }
            parseFile.close();
        }
        else
        {
            cout << "Error: attempted to access an unopened file" << endl;
        }
        return products;
    };

private:
    regex customerIdRegex = regex("customer ([0-9]+) ID ");
    regex customerUsernameRegex = regex("customer ([0-9]+) user name");
    regex customerFNameRegex = regex("customer ([0-9]+) first name ");
    regex customerLNameRegex = regex("customer ([0-9]+) last name ");
    regex customerDobRegex = regex("customer ([0-9]+) date of birth ");
    regex customerRewardRegex = regex("customer ([0-9]+) total reward points ");
    regex customerccNumRegex = regex("customer ([0-9]+) cc info ");

    regex productIdRegex = regex("product ([0-9]+) ID ");
    regex productNameRegex = regex("product ([0-9]+) name ");
    regex productPriceRegex = regex("product ([0-9]+) price ");
    regex productAvailableRegex = regex("product ([0-9]+) total items in store ");
    regex productPointsAwardedRegex = regex("product ([0-9]+) points awarded");

    regex transactionIdRegex = regex("Transaction ID ");
    regex transactionUserRegex = regex("User ID ");
    regex transactionProductRegex = regex("Product ([0-9]+) ID ");
    regex transactionTotalRegex = regex("Total amount ");
    regex transactionRewardsRegex = regex("Total reward points ");



    /**
     * gets the last value of a line
     * @param valueString the string to parse
     * @param delimiter the delimiter to use
     * @return the last value of the string
    */
    string getLastValue(string valueString, string delimiter = " ")
    {
        vector<string> elems;
        size_t pos = 0;
        while ((pos = valueString.find(delimiter)) != string::npos)
        {
            elems.push_back(valueString.substr(0, pos));
            valueString.erase(0, pos + delimiter.length());
        }
        return valueString;
    }

    /**
     *Function reads product ids from the transaction file and pushes to productId vector.
     * @param valueString the string to parse
     * @return a vector of productIds
    */

    vector<string> parseTransactionProducts(string valueString)
    {
        string delimiter = ", ";
        vector<string> productIds;
        int start = 0;
        int end = valueString.find(delimiter);
        while (end != -1)
        {
            productIds.push_back(getLastValue(valueString.substr(start, end - start)));
            start = end + delimiter.size();
            end = valueString.find(delimiter, start);
        }
        return productIds;
    }
};

class Writer
{
public:


    /**
     * writes customer data to a file
     * @param customerData a vector of customer objects
     * @param writeFile the file to write to
     * @return void
    */
    void customerList(vector<Structures::Customer> customerData, ofstream &writeFile)
    {
        if (writeFile.is_open())
        {
            for (int i = 0; i < customerData.size(); i++)
            {
                writeFile << "customer " << i + 1 << " ID " << customerData[i].id << endl;
                writeFile << "customer " << i + 1 << " user name " << customerData[i].username << endl;
                writeFile << "customer " << i + 1 << " first name " << customerData[i].fname << endl;
                writeFile << "customer " << i + 1 << " last name " << customerData[i].lname << endl;
                writeFile << "customer " << i + 1 << " date of birth " << customerData[i].dob << endl;
                writeFile << "customer " << i + 1 << " cc info " << customerData[i].ccNum << endl;
                writeFile << "customer " << i + 1 << " total reward points " << customerData[i].rewardPoints << endl;
                writeFile << "-" << endl;
            }
            writeFile.close();
        }
        else
        {
            cout << "Error: attempted to access an unopened file" << endl;
        }
        return;
    }


    /**
     * writes transaction data to a file
     * @param transactionData a vector of transaction objects
     * @param writeFile the file to write to
     * @return void
    */

    void transactionList(vector<Structures::Transaction> transactionData, ofstream &writeFile)
    {
        if (writeFile.is_open())
        {
            for (auto customer : transactionData)
            {
                writeFile << "Transaction ID "
                          << " ID " << customer.id << endl;
                writeFile << "User ID " << customer.customerId << endl;
                stringstream products;
                for (int i = 0; i < customer.productIds.size(); i++)
                {
                    products << "Product " << i + 1 << " ID " << customer.productIds[i] << ", ";
                }
                string productsString = products.str();
                writeFile << productsString << endl;
                writeFile << "Total amount " << customer.total << endl;
                writeFile << "Total reward points " << customer.pointsAwarded << endl;
                writeFile << "-" << endl;
            }
            writeFile.close();
        }
        else
        {
            cout << "Error: attempted to access an unopened file" << endl;
        }
        return;
    }


    /**
     * writes product data to a file
     * @param productData a vector of product objects
     * @param writeFile the file to write to
     * @return void
    */

    void productList(vector<Structures::Product> productData, ofstream &writeFile)
    {
        if (writeFile.is_open())
        {
            for (int i = 0; i < productData.size(); i++)
            {
                writeFile << "product " << i + 1 << " ID " << productData[i].id << endl;
                writeFile << "product " << i + 1 << " name " << productData[i].name << endl;
                writeFile << "product " << i + 1 << " price " << productData[i].price << endl;
                writeFile << "product " << i + 1 << " total items in store " << productData[i].availableItems << endl;
                writeFile << "product " << i + 1 << " points awarded " << productData[i].rewardAmount << endl;
                writeFile << "-" << endl;
            }
            writeFile.close();
        }
        else
        {
            cout << "Error: attempted to access an unopened file" << endl;
        }
        return;
    }
};