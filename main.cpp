#include "randomizer.h"
#include "user.h"
#include "transactions.h"
#include "user.h"
#include "block.h"
#include <omp.h>
#include <cfloat>

int findUserIndex(const vector<User>& users, const string& publicKey) {
    auto it = std::find_if(users.begin(), users.end(), [&publicKey](const User& user) {
        return user.GetpKey() == publicKey;
    });
    return (it != users.end()) ? std::distance(users.begin(), it) : -1;
}

int main() {
    DataGenerator::init();
    DataGenerator generator;
    vector<User> users = generator.generateUsers(1000);

    // for (const auto& user : users) {
    //     user.print();
    //     cout << endl;
    // }

    vector<Transaction> transactionMemPool = generator.createTransactions(users, 10000);

    vector<Block> blocks;

    while (!transactionMemPool.empty()) {
        int ctr = 1;
        vector<Block> blockCandidates;

        for (int i = 0; i < 4; i++) {

            string prevHash = blocks.empty() 
                ? "0000000000000000000000000000000000000000000000000000000000000000"
                : blocks.back().getHash();

            int getTransactions = std::min(100, static_cast<int>(transactionMemPool.size()));

            Block block(prevHash, 4, vector<Transaction>(transactionMemPool.begin(), transactionMemPool.begin() + getTransactions));
            blockCandidates.push_back(block);
        }
        
        Block minedBlock;
        bool blockMined = false;
        omp_set_num_threads(4);
        blockMinedGlobal = false;

        #pragma omp parallel for
        for (int i = 0; i < blockCandidates.size(); i++) {
            blockCandidates[i].Mine();

            #pragma omp critical
            {
                if (blockCandidates[i].getMined() && !blockMined) {
                    minedBlock  = blockCandidates[i];
                    blockMined = true;
                    blockMinedGlobal = true;
                }
            }
        }

        if(blockMined) {
            vector<string> generalIDs;
            vector<string> invalidIDs;

            for (const auto& transaction : minedBlock.getTransactions()) {
                string senderPublicKey = transaction.GetSender();
                string recipientPublicKey = transaction.GetReceiver();
                double amount = transaction.GetAmount();

                int fromIndex = findUserIndex(users, senderPublicKey);
                int toIndex = findUserIndex(users, recipientPublicKey);


                if (fromIndex != -1 && toIndex != -1 && users[fromIndex].GetBalance() >= amount && getHashString(senderPublicKey + recipientPublicKey + to_string(amount)) == transaction.GetID()) {
                    double closestAmount = DBL_MAX;
                    UTXO closestUTXO;
                    UTXO secondClosestUTXO;
                    bool isSecondUTXOUsed = false;

                    for (const auto& utxo : users[fromIndex].GetUtxos()) {
                        double difference = abs(utxo.amount - amount);
                        if (difference < closestAmount) {
                            closestAmount = difference;
                            closestUTXO = utxo;
                        }
                    }

                    if (closestUTXO.amount < amount) {
                        double secondClosestAmount = DBL_MAX;
                        for (const auto& utxo : users[fromIndex].GetUtxos()) {
                            if (utxo.utxoID != closestUTXO.utxoID) {
                                double combinedAmount = closestUTXO.amount + utxo.amount;
                                double difference = abs(combinedAmount - amount);
                                if (difference < secondClosestAmount && combinedAmount >= amount) {
                                    secondClosestAmount = difference;
                                    secondClosestUTXO = utxo;
                                    isSecondUTXOUsed = true;
                                }
                            }
                        }
                    }

                    double totalUsed = closestUTXO.amount + (isSecondUTXOUsed ? secondClosestUTXO.amount : 0);
                    double change = totalUsed - amount;

                    UTXO newUtxo(amount, recipientPublicKey);
                    users[toIndex].addUTXO(newUtxo);

                    if (change > 0) {
                        string changeID = getHashString(senderPublicKey + recipientPublicKey + to_string(change));
                        UTXO changeUTXO(change, senderPublicKey);
                        users[fromIndex].addUTXO(changeUTXO);
                    }

                    users[fromIndex].removeUTXO(closestUTXO.utxoID);
                    if (isSecondUTXOUsed) {
                        users[fromIndex].removeUTXO(secondClosestUTXO.utxoID);
                    }

                    cout << "Transaction " << transaction.GetID() << " is valid" << endl;
                    ctr++;
                } else {
                    cout << "Transaction " << transaction.GetID() << " is invalid" << endl;
                    invalidIDs.push_back(transaction.GetID());
                }
                generalIDs.push_back(transaction.GetID());
            }

            transactionMemPool.erase(
                std::remove_if(
                    transactionMemPool.begin(), 
                    transactionMemPool.end(), 
                    [&generalIDs](const Transaction& transaction) {
                        return std::find(generalIDs.begin(), generalIDs.end(), transaction.GetID()) != generalIDs.end();
                    }
                ), 
                transactionMemPool.end()
            );

            auto& transactions = minedBlock.getTransactions(); 
            transactions.erase(
                std::remove_if(
                    transactions.begin(), 
                    transactions.end(), 
                    [&invalidIDs](const Transaction& transaction) {
                        return std::find(invalidIDs.begin(), invalidIDs.end(), transaction.GetID()) != invalidIDs.end();
                    }
                ), 
                transactions.end()
            );
            if (ctr != 1) minedBlock.setValidBlock(true);
        }
        if (ctr != 1) {
            ctr--;
            blocks.push_back(minedBlock);

            std::ofstream outFile("minedBlocks.txt", std::ios::app);
            if (outFile.is_open()) {
                minedBlock.print(outFile);  
                outFile << std::endl; 
                outFile.close(); 
            } else {
                std::cerr << "Unable to open file for writing." << std::endl;
            }
        }
    }

    cout << endl;
    cout << "Created Blockchain contains " << blocks.size() << " blocks" << endl;

        // for (const auto& user : users) {
        //     user.print();
        //     cout << endl;
        // }

    cout << endl << "Choose what to do next: \n1. Find Block \n2. Find Transaction \n3. Find User \n4. Exit" << endl;
    int choice;
    cin >> choice;

    while (choice != 4) {
        switch (choice) {
            case 1: {
                cout << "Enter block hash: ";
                string hash;
                cin >> hash;
                auto it = std::find_if(blocks.begin(), blocks.end(), [&hash](const Block& block) {
                    return block.getHash() == hash;
                });
                if (it != blocks.end()) {
                    cout << "Block found!" << endl;
                    it->print(cout);
                } else {
                    cout << "Block not found!" << endl;
                }
                break;
            }
            case 2: {
                cout << "Enter transaction ID: ";
                string id;
                cin >> id;
                bool found = false;
                for (const auto& block : blocks) {
                    auto it = std::find_if(block.getTransactions().begin(), block.getTransactions().end(), [&id](const Transaction& transaction) {
                        return transaction.GetID() == id;
                    });
                    if (it != block.getTransactions().end()) {
                        cout << "Transaction found in block: " << block.getHash() << endl;
                        it->print(cout); 
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    cout << "Transaction not found!" << endl;
                }
                break;
            }
            case 3: {
                cout << "Enter user public key: ";
                string publicKey;
                cin >> publicKey;
                auto it = std::find_if(users.begin(), users.end(), [&publicKey](const User& user) {
                    return user.GetpKey() == publicKey;
                });
                if (it != users.end()) {
                    cout << "User found!" << endl;
                    it->print();
                } else {
                    cout << "User not found!" << endl;
                }
                break;
            }
            default:
                cout << "Invalid choice!" << endl;
                break;
            }
            cout << endl << "Choose what to do next: \n1. Find Block \n2. Find Transaction \n3. Find User \n4. Exit" << endl;
            cin >> choice;
        }

        cout << "Exiting program." << endl;

}