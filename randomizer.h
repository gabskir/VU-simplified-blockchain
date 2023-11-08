#ifndef RANDOMIZER_H
#define RANDOMIZER_H

#include "mylib.h"

// Class Random contains static methods for generating random values of various types.

class Random {
    private:
        static thread_local std::mt19937 generator;

    public:
        // Initialize the random number generator with a random seed based on std::random_device.

        static void init() {
            std::random_device rd;
            generator.seed(rd());
        }
        static string randomString(size_t length) {
            const string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
            std::uniform_int_distribution<> dist(0, str.size() - 1);
            string random_string;
            random_string.reserve(length);

            for (size_t i = 0; i < length; ++i) {
                random_string += str[dist(generator)];
            }

            return random_string;
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
};

// Initialize the static thread-local generator with a non-deterministic seed.

thread_local std::mt19937 Random::generator(std::random_device{}());



#endif

