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
        Transaction() {}

        Transaction(const string& from, const string& to, double amount)
            : sender(from), receiver(to), sentAmount(amount) {
            ID = getHashString(from + to + to_string(amount));
            timeStamp = getCurrentTime();
        }

        string GetSender() const { return sender; }
        string GetReceiver() const { return receiver; }
        string GetID() const { return ID; }
        double GetAmount() const { return sentAmount; }
        string GetTimeStamp() const { return timeStamp; }

        void SetSender(const string& from) { sender = from; }
        void SetReceiver(const string& to) { receiver = to; }
        void SettimeStamp(const string& time) { timeStamp = time; }
        void SetAmount(const long double& amount) { sentAmount = amount; }

        void print(std::ostream& os) const {
            os << "Transaction ID: " << ID << endl;
            os << "Sender: " << sender << endl;
            os << "Receiver: " << receiver << endl;
            os << "Amount: " << sentAmount << endl;
            os << "Time: " << timeStamp << endl;
        }

        void print() const {
            print(std::cout);
        }
};

#endif

