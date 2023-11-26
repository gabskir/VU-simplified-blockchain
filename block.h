#ifndef BL0CK_H
#define BL0CK_H

#include "mylib.h"
#include "merkleTree.h"
#include "transactions.h"

#define BLOCK_VERSION 1
const int NUM_THREADS = 4;
bool blockMinedGlobal = false;


class Block {
    private:
        string hash;
        string previousHash;
        string timestamp;
        unsigned int version;
        string merkleRoot;
        unsigned int difficultyTarget;
        unsigned int nonce;
        vector<Transaction> transactions;

        bool Mined = false;
        bool Valid = false;

    public:
        Block() {};
        Block(const string& prevHash, unsigned int difficultyTarget, const vector<Transaction>& txs)
            : previousHash(prevHash), difficultyTarget(difficultyTarget), transactions(txs), version(BLOCK_VERSION) {
                timestamp = getCurrentTime();
                merkleRoot = MerkleTree(transactions).createMerkleRoot();
                hash = getHashString(getHashString(previousHash + merkleRoot + timestamp + to_string(difficultyTarget) + to_string(version)) + to_string(nonce));
            }

        void setValidBlock (bool valid) {
            Valid = valid;
        }

        string getHash() const { return hash; }
        string getPreviousHash() const { return previousHash; }
        string getTimestamp() const { return timestamp; }
        unsigned int getVersion() const { return version; }
        string getMerkleRoot() const { return merkleRoot; }
        unsigned int getDifficultyTarget() const { return difficultyTarget; }
        unsigned int getNonce() const { return nonce; }
        vector<Transaction>& getTransactions() { return transactions; }
        const vector<Transaction>& getTransactions() const { return transactions; }
        bool getMined() const { return Mined; }

        void Mine() {
            int randomNonce = 0;
            string guessHash = getHashString(getHashString(previousHash + merkleRoot + timestamp + to_string(difficultyTarget) + to_string(version)) + to_string(randomNonce));

            while (!Mined && !blockMinedGlobal) {
                if (!hashMeetDifficultyTarget(guessHash)) {
                    randomNonce += NUM_THREADS;
                    guessHash = getHashString(getHashString(previousHash + merkleRoot + timestamp + to_string(difficultyTarget) + to_string(version)) + to_string(randomNonce));
                } else {
                    nonce = randomNonce;
                    hash = guessHash;
                    Mined = true;
                    #pragma omp critical
                    {
                        blockMinedGlobal = true;
                    }
                }
            }
        }

        bool hashMeetDifficultyTarget(string guessHash) {
            return guessHash.substr(0, difficultyTarget) == string(difficultyTarget, '0');
        }

        void print(std::ostream& os) const {
            os << "Hash: " << hash << endl;
            os << "Previous Hash: " << previousHash << endl;
            os << "Timestamp: " << timestamp << endl;
            os << "Version: " << version << endl;
            os << "Merkle Root: " << merkleRoot << endl;
            os << "Difficulty Target: " << difficultyTarget << endl;
            os << "Nonce: " << nonce << endl;
            os << "Number of Transactions: " << transactions.size() << endl;
            os << "Transactions: " << endl;
            for (const Transaction& tx : transactions) {
                os << tx.GetID() << endl;
            }
        }

};


#endif