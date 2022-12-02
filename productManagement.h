/*
CSC 455 Project 1
Names: Jabez Nuetey, Michael Gerges, Shabura Daniel
*/

#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <iomanip>
#include "data.h"

using namespace std;

class productHandler
{
public:

    void addProduct(Data &data)
    {
        Structures::Product newProduct;
        newProduct.id = generate_product_id(data);
        newProduct.name = input_product_name(data);
        newProduct.price = input_product_price();
        newProduct.availableItems = input_product_quantity();
        newProduct.rewardAmount = input_product_reward_amount();
        data.productData.push_back(newProduct);
    }

    string generate_product_id(Data &data)
    {
        string id = "";
        srand((unsigned)time(0));
        for (int i = 0; i < 5; i++)
        {
            int random = rand() % 10;
            id += to_string(random);
        }
        return validate_product_id(data, "P" + id);
    }

    string validate_product_id(Data &data, string id)
    {
        bool valid = none_of(data.productData.begin(), data.productData.end(), [&id](Structures::Product product)
                             { return product.id == id; });
        if (valid)
        {
            return id;
        }
        else
        {
            return generate_product_id(data);
        }
    }

    string input_product_name(Data &data)
    {
        string input;
        cout << "Please enter the name of the product:" << endl;
        getline(cin, input);
        bool valid_input = validate_product_name(data, input);
        if (valid_input)
        {
            return input;
        }
        else
        {
            return input_product_name(data);
        }
    }

    bool validate_product_name(Data &data, string product_name)
    {
        regex name_matcher = regex("^([a-zA-Z0-9_]+)$");
        if (regex_search(product_name, regex("[/\\s]")))
        {
            cout << "Invalid Input: Product name cannot contain spaces. Please use underscores for spaces." << endl;
            return false;
        }
        else if (regex_search(product_name, name_matcher))
        {
            bool valid = none_of(data.productData.begin(), data.productData.end(), [&product_name](Structures::Product product)
                                 { return product.name == product_name; });
            if (valid)
            {
                return true;
            }
            else
            {
                cout << "A product with that name already exists." << endl;
                return false;
            }
        }
        else
        {
            cout << "Please enter alphanumeric values only." << endl;
            return false;
        }
    }

    double input_product_price()
    {
        string input;
        cout << "Enter the product's price." << endl << "$";
        cin >> input;
        bool valid = validate_product_price(input);
        if (valid)
        {
            double price = ceil(stod(input) * 100.0) / 100.0;
            if (price > 0)
            {
                return price;
            }
            else
            {
                cout << "Invalid input. Must enter a price greater than 0." << endl;
                return input_product_price();
            }
        }
        else
        {
            return input_product_price();
        }
    }

    bool validate_product_price(string product_price)
    {
        regex numbers_matcher = regex("^([0-9]+.[0-9]{2})$");
        if (regex_search(product_price, numbers_matcher))
        {
            return true;
        }
        else
        {
            cout << "Price must be a valid number in regular currency format (X.XX)." << endl;
            return false;
        }
    }

    int input_product_quantity()
    {
        string input;
        cout << "Enter how much of the product is in stock:" << endl;
        cin >> input;
        bool valid = validate_product_quantity(input);
        if (valid)
        {
            int quantity = stoi(input);
            if (quantity < 0)
            {
                cout << "Invalid input. Must enter a quantity of 0 or more." << endl;
                return input_product_quantity();
            }
            else
            {
                return quantity;
            }
        }
        else
        {
            return input_product_quantity();
        }
    }


    bool validate_product_quantity(string product_quantity)
    {
        regex numbers_matcher = regex("^[0-9]+$");
        if (regex_search(product_quantity, numbers_matcher))
        {
            return true;
        }
        else
        {
            cout << "Please enter a positive integer value (no letters or other characters)." << endl;
            return false;
        }
    }

    int input_product_reward_amount()
    {
        string input;
        cout << "Enter how much rewards is earned from the product:" << endl;
        cin >> input;
        bool valid = validate_product_reward_amount(input);
        if (valid)
        {
            int reward_amount = stoi(input);
            if (reward_amount < 0)
            {
                cout << "Must enter a reward amount of 0 or more." << endl;
                return input_product_reward_amount();
            }
            else
            {
                return reward_amount;
            }
        }
        else
        {
            return input_product_reward_amount();
        }
    }

    bool validate_product_reward_amount(string product_reward_amount)
    {
        regex numbers_matcher = regex("^[0-9]+$");
        if (regex_search(product_reward_amount, numbers_matcher))
        {
            return true;
        }
        else
        {
            cout << "Please enter a integer numerical value (no letters or other characters)." << endl;
            return false;
        }
    }

    void list_available_products(Data &data, ShopHandler &shop, vector<Structures::Product> &availableProducts)
    {
        for (auto product : data.productData)
        {
            if (product.availableItems > 0)
            {
                availableProducts.push_back(product);
            }
            else
            {
                continue;
            }
        }
        cout << "\nAvailable Products:\n";
        for (auto product : availableProducts)
        {
            cout << product.id << " | " << product.name << " - " << product.price << " | " << product.availableItems << " available\n";
        }
    }

    void remove_product(Data &data)
    {
        string currentInput;
        cout << "Enter product ID or 0 to exit:" << endl;
        cin >> currentInput;
        if(currentInput == "0")
        {
            return;
        }
        else
        {
            Structures::Product customer = data.findProductById(currentInput);
            if (customer.id == "0")
            {
                cout << "Could not find a product with that ID." << endl;
            }
            else
            {
                data.productData.erase(remove_if(data.productData.begin(), data.productData.end(),
                                                [&currentInput](const Structures::Product &product)
                                                {
                                                    return product.id == currentInput;
                                                }),
                                    data.productData.end());
            }
        }
    }
};