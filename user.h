#ifndef USER_H
#define USER_H

#include "mylib.h"
#include "randomizer.h"

class User {
    private:
        string Name;
        string Public_key;
        long double Balance;

    public:

        User(const string& name, const string& publicKey, long double balance)
            : Name(name), Public_key(publicKey), Balance(balance) {}

        long double GetBalance() const { return Balance; }
        const string& GetpKey() const { return Public_key; }

};       

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
    UserManager(int numberOfUsers) {
        users.reserve(numberOfUsers);
        for (int i = 0; i < numberOfUsers; i++) {
            users.push_back(createUser());
        }
    }

    const vector<User>& getUsers() const { return users; }
};
#endif