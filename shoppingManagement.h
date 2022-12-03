/*
CSC 455 Project 1
Names: Jabez Nuetey, Michael Gerges, Shabura Daniel
*/

#include "data.h"
#include <vector>

// using namespace std;

class ShopHandler
{
public:
    struct ShoppingCart
    {
        Structures::Customer customer;
        vector<Structures::Product> cartItems;
        double cartTotal;
        int rewardPoints;
    };

    vector<Structures::Product> getAvaliableProducts(Data &data)
    {
        vector<Structures::Product> availableProducts;
        for (auto product : data.productData)
        {
            if (product.availableItems > 0)
            {
                availableProducts.push_back(product);
            }
            else
            {
                // Do nothing
            }
        }
        return availableProducts;
    }

    ShoppingCart createNewCart(Structures::Customer customer)
    {
        ShoppingCart newCart;
        newCart.customer = customer;
        newCart.cartTotal = 0;
        newCart.rewardPoints = 0;
        return newCart;
    }

   void addToCart(Data &data, vector<Structures::Product> &availableItems, ShoppingCart &cart)
    {
        string input;
        cout << "Enter the product ID you would like to add to your cart, or 0 to quit:" << endl;
        cin >> input;
        if (input == "0")
        {
            return;
        }
        else
        {
            auto vectorSearch = find_if(begin(availableItems), end(availableItems),
                                        [&input](const Structures::Product product)
                                        {
                                            return product.id == input;
                                        });
            if (vectorSearch == end(availableItems))
            {
                cout << input << " is not an available product ID, please try again." << endl;
                return addToCart(data, availableItems, cart);
            }
            else
            {
                Structures::Product product = data.findProductById(input);
                cart.cartItems.push_back(product);
                data.adjustStock(product.id, (product.availableItems-1));
                cart.cartTotal += product.price;
                cart.rewardPoints += product.rewardAmount;
                displayCart(cart);
            }
        }
    }

    void cleanCart(Data &data, ShoppingCart cart)
    {
        for (auto cartItem : cart.cartItems)
        {
            data.adjustStock(cartItem.id, (cartItem.availableItems + 1));
        }
    }

    void displayCart(ShoppingCart &cart)
    {
        cout << "\nCart Items:\n";
        for (auto cartItem : cart.cartItems)
        {
            cout << "- " << cartItem.id << " - " << cartItem.name << " - " << cartItem.price << endl;
        }
        cout << "Total: " << cart.cartTotal << endl;
    }

    void removeFromCart(Data &data, ShoppingCart &cart)
    {
        string input;
        cout << "Enter the product ID you would like to remove from your cart, or type 0 to quit:" << endl;
        cin >> input;
        if (input == "0")
        {
            return;
        }
        else
        {
            auto vectorSearch = find_if(begin(data.productData), end(data.productData),
                                        [&input](const Structures::Product product)
                                        {
                                            return product.id == input;
                                        });
            if (vectorSearch == end(data.productData))
            {
                cout << input << " is not a product in your cart, please try again." << endl;
                return removeFromCart(data, cart);
            }
            else
            {
                auto removeItem = cart.cartItems.erase(remove_if(cart.cartItems.begin(), cart.cartItems.end(),
                                                         [&input](const Structures::Product &product)
                                                         {
                                                             return product.id == input;
                                                         }),
                                               cart.cartItems.end());
                Structures::Product product = data.findProductById(input);
                data.adjustStock(product.id, (product.availableItems+1));
                cart.cartTotal -= product.price;
                cart.rewardPoints -= product.rewardAmount;
            }
        }
    }

    string generate_transaction_id(Data &data, string predicate, int length)
    {
        string id = "";
        srand((unsigned)time(0));
        for (int i = 0; i < length; i++)
        {
            int random = rand() % 10;
            id += to_string(random);
        }
        return validate_transaction_id(data, predicate, length, predicate + id);
    }

    string validate_transaction_id(Data &data, string predicate, int length, string id)
    {
        bool valid = none_of(data.transactionData.begin(), data.transactionData.end(), [&id](Structures::Transaction transaction)
                             { return transaction.id == id; });
        if (valid)
        {
            return id;
        }
        else
        {
            return generate_transaction_id(data, predicate, length);
        }
    }

    void checkoutShoppingCart(Data &data, ShoppingCart &cart)
    {
        Structures::Transaction transaction;
        transaction.customerId = cart.customer.id;
        transaction.id = generate_transaction_id(data, "TP", 5);
        for (auto product : cart.cartItems)
        {
            transaction.productIds.push_back(product.id);
        }
        transaction.pointsAwarded = cart.rewardPoints;
        int newCustomerPoints = cart.customer.rewardPoints - cart.rewardPoints;
        cart.customer.rewardPoints = newCustomerPoints;
        data.adjustCustomerPoints(transaction.customerId, newCustomerPoints);
        transaction.total = cart.cartTotal;
    }

    void redeemRewards(Data &data, Structures::Customer &currentUser)
    {
        Structures::Transaction transaction;
        string input;

        cout << "Enter the amount of rewards you would like to redeem, or type 0 to quit:" << endl;
        cin >> input;
        int points = stoi(input);
        double total = stod(input);

        if (input == "0")
        {
            return;
        }
        else if (points <= currentUser.rewardPoints)
        {
            transaction.productIds.push_back("Rewards");
            transaction.customerId = currentUser.id;
            transaction.id = generate_transaction_id(data, "TG", 5);
            transaction.pointsAwarded = 0;
            int newCustomerPoints = currentUser.rewardPoints + points;
            data.adjustCustomerPoints(transaction.customerId, newCustomerPoints);
            transaction.total = total;

        }
        else
        {
            cout << input << " is either above your current rewards amount or is an invalid entry." << endl;
            redeemRewards(data, currentUser);
        }
    }
};