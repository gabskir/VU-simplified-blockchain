#include "functions.h"

unsigned long long int leftRotate (unsigned long long int n, unsigned long long int d) {
    return (n << d) | (n >> (INT_BITS - d));
}

unsigned long long int rightRotate (unsigned long long int n, unsigned long long int d) {
    return (n >> d) | (n << (INT_BITS - d));
}

//random bit operation are performed and the result is added to the sum

unsigned long long int hashing(const string& input) {
    int lastIndex = input.length() - 1;
    unsigned long long int sum = 0xb5ad4eceda1ce2a9; 

    for(int i = 0; i < input.length(); i++) {
        char curChar = input[i];
        std::bitset<64> altChar = rightRotate(curChar, (curChar + i) % INT_BITS);
        std::bitset<64> xorBuddy = leftRotate(input[lastIndex], (curChar + lastIndex) % INT_BITS); 
        std::bitset<64> xorResult = altChar ^ xorBuddy;
        unsigned long long int mult = xorResult.to_ullong() * (1672 * static_cast<unsigned long long int>(pow(876, i % 10 + 1)) + 1);
        sum += leftRotate(xorResult.to_ullong(), 13) ^ rightRotate(mult, 7);
        
        if(i < input.length() / 2)
            lastIndex--;
        else if(i > input.length() / 2)
            lastIndex++;
        else
            lastIndex = 0;
    }

    return sum;
}

//fill the bitset with random bits to reach 256 bits

void fillBits(std::bitset<256> &bits, unsigned long long int seed) {
    for (int i = 64; i < 256; i += 64) {
        seed = leftRotate(seed, 19) ^ rightRotate(seed, 29) ^ (seed * 0xb5ad4eceda1ce2a9);
        bits = bits << 64;
        std::bitset<256> block(seed);
        bits = bits ^ block;
    }
}

//convert the bitset to hex string

string getHashString(const string& input) {
    unsigned long long int sum = hashing(input);
    std::bitset<256> bits(sum);
    fillBits(bits, sum);
    
    std::stringstream ss;
    for(int i = 0; i < 256; i += 4) {
        int val = bits[i] * 8 + bits[i + 1] * 4 + bits[i + 2] * 2 + bits[i + 3];
        ss << std::hex << val;
    }

    return ss.str();
}

string getCurrentTime() {
    auto time = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(time);
    stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %X");
    return ss.str();
}