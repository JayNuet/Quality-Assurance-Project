/*
CSC 455 Project 1
Names: Jabez Nuetey, Michael Gerges, Shabura Daniel
*/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include "data.h"

using namespace std;

class CustomerHandler
{
public:

    void registerCustomer(Data &data)
    {
        Structures::Customer newCustomer;
        newCustomer.id = generate_customer_id(data);
        newCustomer.username = input_username(data);
        newCustomer.fname = input_name("first");
        newCustomer.lname = input_name("last");
        newCustomer.dob = input_dob();
        newCustomer.rewardPoints = input_points();
        newCustomer.ccNum = input_card(data);
        data.customerData.push_back(newCustomer);
    }

    string generate_customer_id(Data &data)
    {
        string id = "";
        srand((unsigned)time(0));
        for (int i = 0; i < 10; i++)
        {
            int random = rand() % 10;
            id += to_string(random);
        }
        return validate_customer_id(data, "CID" + id);
    }

    string validate_customer_id(Data &data, string id)
    {
        bool valid = none_of(data.customerData.begin(), data.customerData.end(), [&id](Structures::Customer customer){
                return customer.id == id;
            });
        if(valid)
        {
            return id;
        }
        else {
            return generate_customer_id(data);
        }
    }

    string input_username(Data &data)
    {
        string input;
        cout << "Please input a new username:" << endl;
        cin >> input;
        if (validate_username(data, input))
        {
            return input;
        }
        else
        {
            return input_username(data);
        }
    }

    bool validate_username(Data &data, string username)
    {
        regex username_matcher = regex("^([a-zA-Z]{8,17})([0-9]{0,3})$");
     
        if (regex_search(username, username_matcher))
        {
            bool valid = none_of(data.customerData.begin(), data.customerData.end(), [&username](Structures::Customer customer)
                            { return customer.username == username; });

            if (valid) return true;
            else 
            {
                cout << "A user with that username already exists." << endl;
                return false;
            }
        }
        else
        {
            cout << "Invalid input: Your username must contain between 8-17 characters followed by at most 3 letters." << endl;
            return false;
        }
    }

    string input_name(string nameType)
    {
        string input;
        cout << "Please enter your " << nameType << " name:" << endl;
        cin >> input;
        if (validate_name(input))
        {
            return input;
        }
        else
        {
            return input_name(nameType);
        }
    }

    bool validate_name(string name)
    {
        regex number_matcher = regex("([0-9])");
        if ((name.length() < 0))
        {
            cout << "Invalid input: No input provided." << endl;
            return false;
        }
        else if (name.length() >= 15)
        {
            cout << "Invalid input: Name cannot be longer than 15 characters." << endl;
            return false;
        }
        else if (regex_search(name, number_matcher))
        {
            cout << "Invalid input: Please enter a name that is 15 characters or less and does not include non-alphabetic characters." << endl;
            return false;
        }
        else
        {
            return true;
        }
    }


    string input_dob()
    {
        string input;
        cout << "Please enter your dob in MM-DD-YYYY format:" << endl;
        cin >> input;
        if (validate_dob(input))
        {
            return input;
        }
        else
        {
            return input_dob();
        }
    }


    bool validate_dob(string dateOfBirth)
    {
        DateValidator dateValidator;
        regex dobMatcher = regex("^([0-9]{1,2}-[0-9]{1,2}-[0-9]{4})$");
        if (!regex_search(dateOfBirth, dobMatcher))
        { // Checks to see that there are 10 characters
            cout << "Invalid input. Date must be in MM-DD-YYYY format." << endl;
            return false;
        }
        else
        {
            int month, day, year;
            const char *dobToParse = dateOfBirth.c_str();
            sscanf(dobToParse, "%2d-%2d-%4d",
                   &month,
                   &day,
                   &year);
            bool valid = dateValidator.isValidDate(month, day, year);
            if (valid)
                return true;
            else
            {
                cout << "Invalid input. Date must be in MM-DD-YYYY format." << endl;
                return false;
            }
        }
    }


    string input_card(Data &data)
    {
        string input;
        cout << "Please input your credit card number in the format of XXXX-XXXX-XXXX:" << endl;
        cin >> input;
        bool valid_card = validate_card(data, input);
        if (valid_card)
        {
            return input;
        }
        else
        {
            cout << "Invalid input. Please enter in the format of XXXX-XXXX-XXXX.\n";
            return input_card(data);
        }
    }


    bool validate_card(Data &data, string cardNumber)
    {
        regex card_matcher = regex("^([0-9]{4}-[0-9]{4}-[0-9]{4})$");
        if (regex_search(cardNumber, card_matcher))
        {
            bool valid = none_of(data.customerData.begin(), data.customerData.end(), [&cardNumber](Structures::Customer customer)
                            { return customer.ccNum == cardNumber; });

            if (valid) return true;
            else 
            {
                cout << "Card is already in use." << endl;
                return false;
            }
        }
        else
        {
            return false;
        }
    }

    int input_points()
    {
        string input;
        cout << "Enter customer starting reward points:" << endl;
        cin >> input;
        if (validate_points(input))
        {
            return stoi(input);
        }
        else
        {
            return input_points();
        }
    }

    bool validate_points(string str)
    {
        regex numbersMatcher = regex("^([0-9])+$");
        if (regex_search(str, numbersMatcher))
        {
            return true;
        }
        else
        {
            cout << "Please enter a positive numerical value (no letters or other characters)." << endl;
            return false;
        }
    }

    void find_customer(Data &data)
    {
        string currentInput;
        cout << "Enter customer ID or 0 to quit: ";
        cin >> currentInput;

        if (currentInput == "0")
        {
            return;
        }
        else
        {
            Structures::Customer customer = data.findCustomerById(currentInput);
            if (customer.id == "0")
            {
                cout << "Could not find a customer with that ID." << endl;
            }
            else
            {
                cout << "Found!" << endl;
                cout << customer.id << " - " << customer.fname << " " << customer.lname << "\n";
            }
        }
    }

    void remove_customer(Data &data)
    {
        string currentInput;
        cout << "Enter customer ID or 0 to quit: ";
        cin >> currentInput;
        if (currentInput == "0")
        {
            return;
        }
        else
        {
            Structures::Customer customer = data.findCustomerById(currentInput);
            if (customer.id == "0")
            {
                cout << "Could not find a customer with that ID." << endl;
            }
            else
            {
                data.customerData.erase(remove_if(data.customerData.begin(), data.customerData.end(), 
                [&currentInput](const Structures::Customer &customer){return customer.id == currentInput;}) ,data.customerData.end());
            }
        }
    } 
};