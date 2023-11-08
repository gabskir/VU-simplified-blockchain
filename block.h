#ifndef BL0CK_H
#define BL0CK_H

#include "mylib.h"
#include "merkleTree.h"
#include "transactions.h"

#define BLOCK_VERSION 1


class Block {
    private:
    unsigned int version;
    string previousHash;
    string merkleRoot;
    unsigned long long int timestamp;
    unsigned int difficultyTarget;
    unsigned int nonce;
    vector<Transaction> transactions;


    public:
    Block(const std::string& prevHash, unsigned int diffTarget, const std::vector<Transaction>& trans)
        : version(BLOCK_VERSION), previousHash(prevHash), difficultyTarget(diffTarget), transactions(trans), nonce(0) {
        timestamp = static_cast<unsigned long long int>(std::time(nullptr));
        
        MerkleTree merkleTree(transactions); // Initialize the Merkle tree with transactions
        merkleRoot = merkleTree.createMerkleRoot(); // Create the Merkle root
        nonce = Random::randomUnsignedInt(0, UINT_MAX);
        std::ofstream outFile("blocks_info.txt", std::ios::app);
        if (outFile) {
            outFile << toString() << endl; 
            outFile.close(); 
        } else {
            std::cerr << "Unable to open file for writing block information." << std::endl;
        }
    }

    // Getters
    std::string getPreviousHash() const { return previousHash; }
    string getHash() const { return getHashString(previousHash + merkleRoot + std::to_string(timestamp) + std::to_string(difficultyTarget) + std::to_string(nonce)); }
    unsigned long long int getTimestamp() const { return timestamp; }
    unsigned int getDifficultyTarget() const { return difficultyTarget; }
    unsigned int getNonce() const { return nonce; }
    const std::vector<Transaction>& getTransactions() const { return transactions; }
    string getMerkleRoot() const { return merkleRoot; }

    unsigned int setNonce(unsigned int newNonce) { return nonce = newNonce; }

    // Method to check if the block's hash meets the difficulty requirements
    bool meetRequirements() {
        return getHash().substr(0, difficultyTarget) == string(difficultyTarget, '0');
    }

    string toString() const {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2); 


        //long double totalAmount = std::accumulate(transactions.begin(), transactions.end(), 0.0,
                                                  //[](long double sum, const Transaction& tx) { return sum + tx.GetAmount(); });        

        //long double averageTransaction = transactions.empty() ? 0.0 : totalAmount / transactions.size();

        ss << "Block Information\n"
           << "Version: " << version << "\n"
           << "Previous Hash: " << previousHash << "\n"
           << "Merkle Root: " << merkleRoot << "\n"
           << "Timestamp: " << timestamp << "\n"
           << "Difficulty Target: " << difficultyTarget << "\n"
           << "Nonce: " << nonce << "\n"
           << "Number of Transactions: " << transactions.size() << "\n";
           //<< "A total of " << totalAmount << " coins were sent in the block\n"
           //<< "The average transaction is " << averageTransaction << " coins .\n";
        return ss.str();
    }
};

// Mine class represents a miner that attempts to mine blocks
class Mine {
    private:
    unsigned int id;
    unsigned int blocksMined;

    public:
    Mine(unsigned int id) : id(id), blocksMined(0) {}
    // Method to mine a block with given parameters
    Block mineBlock(string prevHash, unsigned int difficultyTarget, const vector<Transaction>& transactions, unsigned int attempts) {
        Block block(prevHash, difficultyTarget, transactions);
        cout << "Starting mining process for Block by Miner ID " << id << endl;

        // Attempt to mine the block by trying different nonce values
        for (unsigned int i = 0; i < attempts; i++) {
            unsigned int nonce = Random::randomUnsignedInt(0, UINT_MAX);
            block.setNonce(nonce);

            if (block.meetRequirements()) {
                cout << "Block " << id << " mined successfully with hash: " << block.getHash() << endl;
                return block;
            }
        }

        throw std::runtime_error("Block " + std::to_string(id) + " failed to mine");
    }

    void incrementBlocksMined() { blocksMined++; }
    unsigned int getBlocksMined() const { return blocksMined; }
    unsigned int getID() const { return id; }
};

#endif