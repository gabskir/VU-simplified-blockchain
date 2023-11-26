#ifndef USER_H
#define USER_H

#include "mylib.h"
#include "randomizer.h"

struct UTXO {
    double amount;
    string utxoID;
    string owner;

    UTXO() {}
    UTXO(double amount, string owner) 
        : amount(amount),owner(owner) {
            utxoID = getHashString(owner + to_string(amount));
        }
};

class User {
    private:
        string Name;
        string Public_key;
        vector <UTXO> utxos;

    public:
        User () {};
        User(const string& name, const string& publicKey) 
            : Name(name), Public_key(publicKey) {}

        string GetName() const { return Name; }
        string GetpKey() const { return Public_key; }
        const vector<UTXO>& GetUtxos() const { return utxos; }

        void setName(const string& name) { Name = name; }
        void setpKey(const string& publicKey) { Public_key = publicKey; }
        
        double GetBalance() const {
            double balance = 0.0;
            for (const auto& utxo : utxos) {
                balance += utxo.amount;
            }
            return balance;
        }

        void addUTXO(const UTXO& utxo) {
            utxos.push_back(utxo);
        }

        void removeUTXO(const string& utxoID) {
            auto it = std::find_if(utxos.begin(), utxos.end(), [&utxoID](const UTXO& u) {
                return u.utxoID == utxoID;
            });
            if (it != utxos.end()) {
                utxos.erase(it);
            }
        }

        void print() const {
            cout << "Name: " << Name << endl;
            cout << "Public key: " << Public_key << endl;
            cout << fixed << setprecision(2);
            cout << "Balance: " << GetBalance() << endl;
            cout << "UTXOs: " << endl;
            for (const auto& utxo : utxos) {
                cout << "    " << utxo.utxoID << " - " << setprecision(2) << utxo.amount << endl;
            }
        }
};

#endif