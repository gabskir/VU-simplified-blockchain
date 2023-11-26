#ifndef MERKLETREE_H
#define MERKLETREE_H

#include "mylib.h"
#include "functions.h"
#include "transactions.h"


class MerkleTree {
private:
    vector<Transaction> transactions;

    string hashCombinedTransactions(const string& left, const string& right){
        return getHashString(left + right);
    }
public:
    MerkleTree(const vector<Transaction>& transactions) : transactions(transactions) {}

    string createMerkleRoot() {

        if(transactions.empty()){
            return getHashString("Empty Tree Placeholder Hash");
        }

        vector<string> tree;
        for (const auto& transaction : transactions) {
            tree.push_back(transaction.GetID());
        }

        while (tree.size() > 1) {
            vector<string> newLevel;
            for (size_t i = 0; i < tree.size(); i += 2) {
                if(i + 1 < tree.size()){
                    newLevel.push_back(hashCombinedTransactions(tree[i], tree[i + 1]));
                } else {
                    newLevel.push_back(hashCombinedTransactions(tree[i], tree[i]));
                }
            }
            tree = newLevel;
        }
    return tree.front();
    }
};

#endif

