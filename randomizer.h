#ifndef RANDOMIZER_H
#define RANDOMIZER_H

#include "mylib.h"
#include "functions.h"
#include "user.h"
#include "transactions.h"


class DataGenerator {
    private:
        static thread_local std::mt19937 generator;

    public:
        static void init() {
            std::random_device rd;
            generator.seed(rd());
        }

        static double randomDouble(double min, double max) {
            std::uniform_real_distribution<double> dist(min, max);
            return dist(generator);
        }

        static int randomInt(int min, int max) {
            std::uniform_int_distribution<> dist(min, max);
            return dist(generator);
        }

        static unsigned int randomUnsignedInt(unsigned int min, unsigned int max) {
            std::uniform_int_distribution<unsigned int> dist(min, max);
            return dist(generator);
        }

        vector<User> generateUsers(int n) {

            vector<User> users;

            for (int i = 0; i < n; i++) {

                string name = "user" + to_string(i);
                string publicKey = getHashString(name);
                                
                User user(name, publicKey);
                users.push_back(user);

                int numberfInitialUTXOs = randomInt(1, 10);
                for (int j = 0; j < numberfInitialUTXOs; j++) {
                    double amount = randomDouble(100, 100000);
                    UTXO utxo = {amount, publicKey};
                    users[i].addUTXO(utxo);
                }
            }
            return users;
        }

        vector<Transaction> createTransactions (const vector<User>& users, int n) {
            vector<Transaction> transactions;
            std::ofstream outFile("transactions.txt");

            if (!outFile.is_open()) {
                std::cout << "Unable to open file";
                exit(1);
            }

            for (int i = 0; i < n; i++) {
                string recipientPublicKey = getRandomPublicKey(users);
                string senderPublicKey;
                do {
                    senderPublicKey = getRandomPublicKey(users);
                } while (senderPublicKey == recipientPublicKey);

                double amount = randomDouble(100, 100000);
                
                Transaction transaction(senderPublicKey, recipientPublicKey, amount);
                transactions.push_back(transaction);

                if (outFile.is_open()) {
                    transaction.print(outFile);
                    outFile << endl;
                }
            }

            outFile.close();
            return transactions;
        }

        string getRandomPublicKey(const vector<User>& users) {
            int index = randomInt(0, users.size() - 1);
            return users[index].GetpKey();
        }
};

thread_local std::mt19937 DataGenerator::generator(std::random_device{}());

#endif
