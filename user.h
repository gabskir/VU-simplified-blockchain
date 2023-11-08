#ifndef USER_H
#define USER_H

#include "mylib.h"
#include "randomizer.h"

//class to represent an user (with a name, public key and balance)
class User {
    private:
        string Name;
        string Public_key;
        long double Balance;

    public:
        User () = default;
        User(const string& name, const string& publicKey, long double balance)
            : Name(name), Public_key(publicKey), Balance(balance) {}

        void UpdateBalance(long double amount) {
            Balance += amount;
        }
        // Copy constructor
        User(const User& other) = default;

        // Move constructor
        User(User&& other) noexcept = default;

        // Copy assignment operator
        User& operator=(const User& other) = default;

        // Move assignment operator
        User& operator=(User&& other) noexcept = default;

        // Getters
        long double GetBalance() const { return Balance; }
        const string& GetpKey() const { return Public_key; }
};       

//class to manage the users, create them and update their balance
class UserManager {
private:
    vector <User> users;

    User createUser() {
        string name = Random::randomString(10);
        string publicKey = Random::randomString(50);
        double balance = Random::randomDouble(100, 1000000);
        return User(name, publicKey, balance);
    }


public:
    // Constructor that creates a specified number of users
    UserManager(int numberOfUsers) {
        users.reserve(numberOfUsers);
        for (int i = 0; i < numberOfUsers; i++) {
            users.push_back(createUser());
        }
    }

    // Updates the balance of a user identified by their public key.

    void updateBalance(const string& publicKey, long double amount) {
        for (auto& user : users) {
            if (user.GetpKey() == publicKey) {
                user.UpdateBalance(amount);
                return;
            }
        }
    }

    const vector<User>& getUsers() const { return users; }

    // Finds a user by their public key and returns a pointer to the user object.
    const User* findUser(const string& publicKey) const {
        for (const auto& user : users) {
            if (user.GetpKey() == publicKey) {
                return &user;
            }
        }
        return nullptr;
    }
};
#endif