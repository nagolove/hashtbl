#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
 
typedef uint64_t NUMBER;

NUMBER i = 0, j = 0;

// function check whether a number is prime or not
bool isPrime(NUMBER n) {
    /*printf("isPrime %lu\n", n);*/
    // Corner case
    if (n <= 1)
        return false;

    // Check from 2 to sqrt(n-1)
    /*for (i = 2; i <= sqrt(n); i++)*/
    for (i = 2; i < n; i++)
        if (n % i == 0) {
            /*printf("stopped after %lu iterations\n", i);*/
            return false;
        }

    return true;
}

// Function to print primes
void printPrime(NUMBER n) {
    for (NUMBER i = 2; i <= n; i++)
        if (isPrime(i))
            printf("%lu ", i);
}

void sig_handler(int sig) {
    /*printf("%s signal catched\n", strsignal(sig));*/
    printf("\ni=%lu j=%lu\n", i, j);
    exit(EXIT_SUCCESS);
}

// Driver Code
int main(int argc, char **argv) {
    signal(SIGINT, sig_handler);

    for (int k = 0; k < argc; ++k) {
        printf("%s\n", argv[k]);
    }

    /*
    printf("isPrime(%i) %i\n", 5, isPrime(5));
    printf("isPrime(%i) %i\n", 17, isPrime(17));
    printf("isPrime(%i) %i\n", 53, isPrime(53));
    printf("isPrime(%i) %i\n", 773, isPrime(773));
    printf("isPrime(%i) %i\n", 991, isPrime(991));
    printf("isPrime(%i) %i\n", 992, isPrime(992));
    exit(0);
    // */

    bool prime = false;
    /*NUMBER range_initial = 256, range_final = pow(2, 32);*/
    NUMBER range_initial = 256, range_final = pow(2, 33);

    j = range_initial;

    while (j < range_final) {
        NUMBER v = j;
        do {
            prime = isPrime(v);
            if (prime) {
                printf("%lu is prime\n", v);
            }
            if (v > 2)
                v -= 1;
            else
                break;
        } while (!prime);
        j *= 1.5;
    }
 }

