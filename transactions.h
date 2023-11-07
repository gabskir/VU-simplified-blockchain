#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include "mylib.h"
#include "functions.h"
#include "user.h"


class Transaction {
    private:
        string ID;
        string sender;
        string receiver;
        double sentAmount;
        string timeStamp;

    public:
        //CONSTRUCTORS
        Transaction() {}

        Transaction(const string& from, const string& to, double amount, string time)
            : sender(from), receiver(to), sentAmount(amount), timeStamp(time) {
            ID = getHashString(from + to + std::to_string(amount));
        }

        //GETTERS

        string GetSender() const { return sender; }
        string GetReceiver() const { return receiver; }
        string GetID() const { return ID; }
        long double GetAmount() const { return sentAmount; }

        string toString() const {
            return "Transaction: " + ID + "\nFrom: " + sender + "\nTo: " + receiver + "\nAmount: " + std::to_string(sentAmount) + "\n" + "AT: " + timeStamp + "\n";
        }
};

class TransactionBuilder {
    private:
        vector <Transaction> transactions;

        Transaction newTransaction(const vector<User>& users) {
            string recipientPublicKey = getRandomPublicKey(users);
            string senderPublicKey;
            do {
                senderPublicKey = getRandomPublicKey(users);
            } while (senderPublicKey == recipientPublicKey);

            double amount = Random::randomDouble(100, 1000000);
            string timeStamp = getCurrentTime();
            return Transaction(senderPublicKey, recipientPublicKey, amount, timeStamp);
        }

        string getRandomPublicKey(const vector<User>& users) {
            if (users.empty()) {
                throw std::runtime_error("No users in the system");
            }
            int randomIndex = Random::randomUnsignedInt(0, users.size() - 1);
            return users[randomIndex].GetpKey();
        }

        string getCurrentTime() {
            auto time = std::chrono::system_clock::now();
            auto time_t = std::chrono::system_clock::to_time_t(time);
            stringstream ss;
            ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %X");
            return ss.str();
        }


    public:
        TransactionBuilder(unsigned int n, UserManager &userManager) {
            auto users = userManager.getUsers();
            for (unsigned int i = 0; i < n; i++) {
                Transaction trans = newTransaction(users);
                transactions.push_back(trans);
            }
        }

        const vector<Transaction>& getTransactions() const { return transactions; }
 
};

#endif