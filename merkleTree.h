#ifndef MERKLETREE_H
#define MERKLETREE_H

#include "mylib.h"
#include "functions.h"
#include "transactions.h"


class MerkleTree {
private:
    // A vector of transactions to build the Merkle tree from.
    vector<Transaction> transactions;

    // Helper function that takes two hashes (left and right), concatenates them, and returns their hash.
    string hashCombinedTransactions(const string& left, const string& right){
        return getHashString(left + right);
    }
public:
    // Constructor that initializes the Merkle tree with a vector of transactions.
    MerkleTree(const vector<Transaction>& transactions) : transactions(transactions) {}

    string createMerkleRoot() {

        // If there are no transactions, return a placeholder hash for an empty tree.
        if(transactions.empty()){
            return getHashString("Empty Tree Placeholder Hash");
        }

        // Initialize the tree vector with the transaction IDs (hashes of transactions).
        vector<string> tree;
        for (const auto& transaction : transactions) {
            tree.push_back(transaction.GetID());
        }

        while (tree.size() > 1) {
            vector<string> newLevel;
            // Combine each pair of adjacent node hashes and hash them together.
            for (size_t i = 0; i < tree.size(); i += 2) {
                if(i + 1 < tree.size()){
                    newLevel.push_back(hashCombinedTransactions(tree[i], tree[i + 1]));
                } else {
                    // If we're at the end with an odd number of nodes, hash the last node with itself.
                    newLevel.push_back(hashCombinedTransactions(tree[i], tree[i]));
                }
            }
            tree = newLevel;
        }
    return tree.front();
    }
};

#endif

