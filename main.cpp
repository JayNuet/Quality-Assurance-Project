/*
CSC 455 Project 1
Names: Jabez Nuetey, Michael Gerges, Shabura Daniel
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "data.h"
#include "textFileManagement.h"
#include "customerManagement.h"
#include "shoppingManagement.h"
#include "productManagement.h"

using namespace std;

string customersFile = "customers.txt";
string productsFile = "products.txt";
string transactionsFile = "transactions.txt";

// Function Prototypes
void customerMenu(Data &data, Structures::Customer &currentUser);
void shoppingMenu(Data &data, Structures::Customer &currentUser);
void rewardsMenu(Data &data, Structures::Customer &currentUser);


/**
 * read appropriate files and store data in Data object
 * @param filepath filepath string
 * @return boolean value indicating success or failure of reading the file
*/

bool ReadFileData(Data &data, string filePath)
{
    Reader parse;
    ifstream readFile;
    readFile.open(filePath);
    if (readFile.is_open())
    {
        cout << "Reading: " << filePath << endl;
        // Parse files
        if (filePath == customersFile)
        {
            data.customerData = parse.customerList(readFile);
        }
        else if (filePath == productsFile)
        {
            data.productData = parse.productList(readFile);
        }
        else if (filePath == transactionsFile)
        {
            data.transactionData = parse.transactionList(readFile);
        }
        else
        {
            cout << "Error: attempted to read an invalid file" << endl;
            readFile.close();
            return false;
        }
    }
    else
    {
        cout << "Could not Read: " << customersFile << endl;
        // File unable to be opened
        readFile.close();
        return false;
    }
    readFile.clear();
    filePath.clear();
    return true;
}

/**
 * Write to appropriate file
 *
 * @param filePath File path String
 * @return `bool` A boolean denoting if the values were successfully written
 */
bool WriteFileData(Data &data, string filePath)
{
    Writer write;
    ofstream writeFile;
    writeFile.open(filePath);
    if (writeFile.is_open())
    {
        cout << "Writing to: " << filePath << endl;
        // Parse files
        if (filePath == customersFile)
        {
            write.customerList(data.customerData, writeFile);
        }
        else if (filePath == productsFile)
        {
            write.productList(data.productData, writeFile);
        }
        else if (filePath == transactionsFile)
        {
            write.transactionList(data.transactionData, writeFile);
        }
        else
        {
            cout << "Error: attempted to write to an invalid file" << endl;
            writeFile.close();
            return false;
        }
    }
    else
    {
        cout << "Could not Write to: " << customersFile << endl;
        // File unable to be opened
        writeFile.close();
        return false;
    }
    return true;
}


/**
 * customer login function, checks if a username exists and if it does, 
 * uses it as the currently logged in customer
 * @param data data object
 * @return no return value
 * 
*/
void customerLogin(Data &data)
{
    Structures::Customer currentUser;
    string input;
    bool running = true;
    while (running)
    {
        cout << "Please enter your username to login, or type 0 to exit:" << endl;
        cin >> input;
        if (input == "0")
        {
            return;
        }
        else
        {
            auto vectorSearch = find_if(begin(data.customerData), end(data.customerData),
                                        [&input](const Structures::Customer customer)
                                        {
                                            return customer.username == input;
                                        });
            if (vectorSearch == end(data.customerData))
            {
                cout << "Username not found, please try again." << endl;
            }
            else
            {
                currentUser = *vectorSearch;
                customerMenu(data, currentUser);
                running = false;
            }
        }
    }
}

/**
 * customer menu function, displays the customer menu and allows the user to 
 * navigate through the menu
 * @param data data object
 * @param currentUser currently logged in customer
 * @return no return value
 * 
*/
void customerMenu(Data &data, Structures::Customer &currentUser)
{
    bool running = true;
    while (running)
    {
        int selection;
        cout << "***Welcome Back Loyal Customer, what would you like to do today?***\n"
             <<"Please select one of the following: \n"
             << "   1. Shopping\n"
             << "   2. Redeem Rewards\n"
             << "   0. Quit\n"
             << "Enter a number to select: " << endl;
        cin >> selection;
        cin.ignore();

        switch (selection){
            case 0:
                running = false;
                break;
            case 1:
                shoppingMenu(data, currentUser);
                break;
            case 2:
                rewardsMenu(data, currentUser);
                break;
            default:
                cout << "Invalid option" << endl;
                break;
        };
    }
}

/**
 * shopping menu function, displays the shopping menu and allows the user to 
 * navigate through the menu
 * @param data data object
 * @param currentUser currently logged in customer
 * @return no return value
 * 
*/

void shoppingMenu(Data &data, Structures::Customer &currentUser)
{
    productHandler prods;
    ShopHandler shopSession;
    ShopHandler::ShoppingCart shoppingCart = shopSession.createNewCart(currentUser);
    vector<Structures::Product> availableProducts;
    bool running = true;
    while (running)
    {
        prods.list_available_products(data, shopSession, availableProducts);
        cout << "\n";
        int selection;
        cout << "\n"
             <<"To begin shopping, please select one of the Menu options: \n"
             << "   1. Add Product to Cart\n"
             << "   2. Remove Product from Cart\n"
             << "   3. View Cart\n"
             << "   4. Checkout\n"
             << "   0. Quit\n"
             << "Enter a number to select: " << endl;
        cin >> selection;
        cin.ignore();

        switch (selection){
            case 0:
                cout << "Goodbye" << endl;
                shopSession.cleanCart(data, shoppingCart);
                running = false;
                break;
            case 1:
                shopSession.addToCart(data, availableProducts, shoppingCart);
                break;
            case 2:
                shopSession.removeFromCart(data, shoppingCart);
                break;
            case 3:
                shopSession.displayCart(shoppingCart);
                break;
            case 4:
                shopSession.checkoutShoppingCart(data, shoppingCart);
                WriteFileData(data, customersFile);
                WriteFileData(data, productsFile);
                ReadFileData(data, customersFile);
                ReadFileData(data, productsFile);
                currentUser = data.findCustomerById(currentUser.id);
                shoppingCart = shopSession.createNewCart(currentUser);
                break;
            default:
                cout << "Invalid option" << endl;
                break;
        };
    }
}


/**
 * rewards menu function, displays the rewards menu and allows the user to 
 * navigate through the menu
 * @param data data object
 * @param currentUser currently logged in customer
 * @return no return value
 * 
*/

void rewardsMenu(Data &data, Structures::Customer &currentUser)
{
    ShopHandler shopSession;

    bool running = true;
    while (running)
    {
        int selection;
        cout << "Please enter one of the following: \n"
             << "   1. View Points\n"
             << "   2. Redeem Rewards\n"
             << "   0. Quit\n"
             << "Enter a number to select: " << endl;

            cin >> selection;
            cin.ignore();
            switch (selection){
                case 0:
                    running = false;
                    break;
                case 1:
                    cout << "Congatulations!!! You have " << currentUser.rewardPoints << "reward points available to redeem" << endl;
                    break;
                case 2:
                    shopSession.redeemRewards(data, currentUser);
                    break;
                default:
                    cout << "Invalid option" << endl;
                    break;
            };
    }
}


/**
 * main function, program entry point
 * @return int EXIT_SUCCESS
*/

int main(){
    Data data;
    CustomerHandler newUser;
    productHandler newProduct;


    // Read File Data into memory
    ReadFileData(data, customersFile);
    ReadFileData(data, productsFile);
    ReadFileData(data, transactionsFile);

    bool running = true;
    while(running)
    {
        int selection;
        cout << "\n"
             <<"Welcome! Please select one of the Menu options: \n"
             << "   1. Customer registration\n"
             << "   2. Customer Removal\n"
             << "   3. Product Addition\n"
             << "   4. Product Removal\n"
             << "   5. Shopping\n"
             << "   6. View Customer using customer ID\n"
             << "   7. Redeem Rewards\n"
             << "   0. Quit\n"
             << "Enter a number to select: " << endl;
        cin >> selection;
        cin.ignore();

    switch (selection){
        case 0:
            cout << "Goodbye!" << endl;
            running = false;
            break;
        case 1:
            newUser.registerCustomer(data);
            WriteFileData(data, customersFile);
            ReadFileData(data, customersFile);
            break;
        case 2:
            newUser.remove_customer(data);
            WriteFileData(data, customersFile);
            ReadFileData(data, customersFile);
            break;
        case 3:
            newProduct.addProduct(data);
            WriteFileData(data, productsFile);
            ReadFileData(data, productsFile);
            break;
        case 4:
            newProduct.remove_product(data);
            WriteFileData(data, productsFile);
            ReadFileData(data, productsFile);
            break;
        case 5:
            customerLogin(data);
            break;
        case 6:
            newUser.find_customer(data);
            break;
        case 7:
            customerLogin(data);
            break;
        default:
            cout << "Invalid option" << endl;
            break;
        }

    }
    WriteFileData(data, customersFile);
    WriteFileData(data, productsFile);
    WriteFileData(data, transactionsFile);

    return EXIT_SUCCESS;
}