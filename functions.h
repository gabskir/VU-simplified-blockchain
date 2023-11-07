#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "mylib.h"

unsigned long long int leftRotate (unsigned long long int n, unsigned long long int d);
unsigned long long int rightRotate (unsigned long long int n, unsigned long long int d);
unsigned long long int hashing(const string& input);
void fillBits(std::bitset<256> &bits, unsigned long long int seed);
string getHashString(const string& input);



#endif