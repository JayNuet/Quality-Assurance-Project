/*
CSC 455 Project 1
Names: Jabez Nuetey, Michael Gerges, Shabura Daniel
*/

#ifndef DATA
#define DATA

#include <iostream>
#include <string>
#include <vector>
#include <regex>

using namespace std;

class Structures
{
public:
    struct Customer
    {
        string id;
        string username;
        string fname;
        string lname;
        string dob;
        string ccNum;
        int rewardPoints;
    };

    struct Transaction
    {
        string id;
        string customerId;
        vector<string> productIds;
        double total;
        int pointsAwarded;
    };

    struct Product
    {
        string id;
        string name;
        double price;
        int availableItems;
        int rewardAmount;
    };
};

class Data
{
public:
    vector<Structures::Customer> customerData;
    vector<Structures::Product> productData;
    vector<Structures::Transaction> transactionData;


    Structures::Customer findCustomerById(string id)
    {
        auto vectorSearch = find_if(begin(customerData), end(customerData),
                                    [&id](const Structures::Customer customer)
                                    {
                                        return customer.id == id;
                                    });
        if (vectorSearch == end(customerData))
        {
            Structures::Customer blankCustomer;
            blankCustomer.id = "0";
            return blankCustomer;
        }
        else
        {
            return *vectorSearch;
        }
    }


    Structures::Product findProductById(string id)
    {
        auto vectorSearch = find_if(begin(productData), end(productData),
                                    [&id](const Structures::Product product)
                                    {
                                        return product.id == id;
                                    });
        if (vectorSearch == end(productData))
        {
            Structures::Product blankProduct;
            blankProduct.id = "0";
            return blankProduct;
        }
        else
        {
            return *vectorSearch;
        }
    }


    void adjustStock(string id, int adjustAmount)
    {
        vector<Structures::Product>::iterator it;
        for (it = productData.begin(); it != productData.end(); it++)
        {
            if (it->id == id)
            {
                it->availableItems = adjustAmount;
                break;
            }
            else
            {
                // Do Nothing
            }
        }
    }


    void adjustCustomerPoints(string id, int adjustAmount)
    {
        vector<Structures::Customer>::iterator it;
        for (it = customerData.begin(); it != customerData.end(); it++)
        {
            if (it->id == id)
            {
                it->rewardPoints = adjustAmount;
                break;
            }
            else
            {
                return;
            }
        }
    }

    void adjustProductPoints(string id, int newAmount)
    {
        vector<Structures::Product>::iterator it;
        for (it = productData.begin(); it != productData.end(); it++)
        {
            if (it->id == id)
            {
                it->rewardAmount = newAmount;
                break;
            }
            else
            {
                return;
            }
        }
    }
};

class DateValidator
{
public:

    bool isLeap(int year)
    {
        return (((year % 4 == 0) &&
                 (year % 100 != 0)) ||
                (year % 400 == 0));
    }

    bool isValidDate(int month, int day, int year)
    {
        // If year, month and day are not in given range
        if ((month >= 1 && month <= 12) && (day >= 1 && day <= 31))
        {
            // Handle February month with leap year
            if (month == 2)
            {
                if (isLeap(year))
                {
                    return (day <= 29);
                }
                else
                {
                    return (day <= 28);
                }
            }

            // Months of April, June, Sept and Nov must have number of days <= 30
            if (month == 4 || month == 6 ||
                month == 9 || month == 11)
            {
                return (day <= 30);
            }
            else
            {
                return (day <= 31);
            }
        }
        else
        {
            return false;
        }
    }
};

#endif