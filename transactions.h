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
        // CONSTRUCTORS
        Transaction() {}

        // Parameterized constructor that initializes transaction and calculates its hash ID.

        Transaction(const string& from, const string& to, double amount, string time)
            : sender(from), receiver(to), sentAmount(amount), timeStamp(time) {
            ID = getHashString(from + to + std::to_string(amount));
        }

        // GETTERS

        string GetSender() const { return sender; }
        string GetReceiver() const { return receiver; }
        string GetID() const { return ID; }
        long double GetAmount() const { return sentAmount; }
        string GetTimeStamp() const { return timeStamp; }

        // Method to convert the transaction details to a string for display or logging.

        string toString() const {
            std::ostringstream ss;
            ss << std::fixed << std::setprecision(2) << sentAmount;
            return "Transaction: " + ID + "\nFrom: " + sender + "\nTo: " + receiver + "\nAmount: " + ss.str() + "\n" + "AT: " + timeStamp + "\n";
        }
};

// Class responsible for building a list of transactions.

class TransactionBuilder {
    private:
        vector <Transaction> transactions;

        Transaction newTransaction(const vector<User>& users) {
            string recipientPublicKey = getRandomPublicKey(users);
            string senderPublicKey;
            do {
                senderPublicKey = getRandomPublicKey(users);
            } while (senderPublicKey == recipientPublicKey); // Ensure sender and receiver are not the same.

            double amount = Random::randomDouble(100, 100000);
            amount = std::floor(amount * 100.0 + 0.5) / 100.0; // Round the amount to two decimal places.


            string timeStamp = getCurrentTime();  // Get the current time for the timestamp.
            return Transaction(senderPublicKey, recipientPublicKey, amount, timeStamp);
        }

        string getRandomPublicKey(const vector<User>& users) {
            if (users.empty()) {
                throw std::runtime_error("No users in the system");
            }
            int randomIndex = Random::randomUnsignedInt(0, users.size() - 1);
            return users[randomIndex].GetpKey();
        }

        // Helper method to get the current time as a formatted string.

        string getCurrentTime() {
            auto time = std::chrono::system_clock::now();
            auto time_t = std::chrono::system_clock::to_time_t(time);
            stringstream ss;
            ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %X");
            return ss.str();
        }


    public:

    // Constructor that generates a specified number of transactions.
    TransactionBuilder(unsigned int n, UserManager& userManager) {
        auto users = userManager.getUsers();
        std::ofstream outFile("transactions.txt", std::ios::out | std::ios::app);
        if (!outFile) {
            throw std::runtime_error("Unable to open file for writing transactions.");
        }

        unsigned int validTransactionsCount = 0;
        for (unsigned int i = 0; i < n; i++) {
            Transaction trans = newTransaction(users);
            std::string validationMessage;
            if (validateTransaction(trans, userManager, validationMessage)) {
                transactions.push_back(trans);
                validTransactionsCount++;
                outFile << "Valid transaction added: " << trans.GetID() << endl;
                outFile << trans.toString() << endl;
            } else {
                outFile << "Invalid transaction detected: " << trans.GetID() << endl;
                outFile << validationMessage << endl;
            }
        }

        outFile << "Total number of valid transactions added to the pool: " << validTransactionsCount << endl;
        outFile.close();
    }

    // Getter to access the list of transactions.

    const vector<Transaction>& getTransactions() const { return transactions; }

    // Method to validate the transaction's integrity and legitimacy.

    bool validateTransaction(const Transaction& transaction, const UserManager& userManager, string& message) {
        const User* sender = userManager.findUser(transaction.GetSender());
        if (!sender) {
            message = "Sender not found: " + transaction.GetSender();
            return false;
        }
        std::string recalculatedHash = getHashString(transaction.GetSender() + transaction.GetReceiver() + std::to_string(transaction.GetAmount()));

        if (transaction.GetID() != recalculatedHash) {
            message = "Transaction hash mismatch: Expected hash = " + recalculatedHash + ", Actual hash = " + transaction.GetID();
            return false;
        }

        if (sender->GetBalance() < transaction.GetAmount()) {
            std::ostringstream ss;
            ss << std::fixed << std::setprecision(2) << sender->GetBalance();
            message = "Insufficient funds for transaction: " + transaction.GetID() + ". Balance: " + ss.str() + ", Amount to send: " + std::to_string(transaction.GetAmount());
            return false;
        }

        message.clear();
        return true;
    }
 
};

#endif