#include <iostream>
#include <string>
#include "sha256.h"
using namespace std;


char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";

string int_to_string(long long val) { 
    string ret;
    int size = sizeof(digits) -1;
    while (val) { 
        ret += digits[val % size];
        val /= size;
    }
    return ret;
}



string guess(string hash){
    unsigned long long i = 0;
    while(true){
        string g = sha256(int_to_string(i));
          if(g.compare(hash) == 0){
              return int_to_string(i);
        }
        i++;
    }
}

int main() {
    string to_crack = "3e23e8160039594a33894f6564e1b1348bbd7a0088d42c4acb73eeaed59c009d";
    cout << guess(to_crack) << endl;
    return 0;
}

