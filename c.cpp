#include <iostream>
#include <string>
#include <cmath>
#include "sha256.h"
#include <chrono>
using namespace std;

char digits[] = "abcdefghijklmnopqrstuvwxyz";

string int_to_string(long long val) { 
    string ret;
    int size = sizeof(digits) -1;
    while (val) { 
        ret = digits[val % size] + ret;
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

int main(int argc, char *argv[]) {
    //can roughly check 435,415 hashes per second
    string needle = argv[1];
    long long min = stoll(argv[2]);
    long long max = stoll(argv[3]); // pow(sizeof(digits-1),numOfDigitsYouWantToSearchThrough)
    cout<<"Hash to crack:"<<needle<<endl;
    cout<<"Starting from:"<<min<<endl;
    cout<<"Ending at:"<<max<<endl;
    auto t1 = std::chrono::high_resolution_clock::now();
    for(int i = min ; i < max;i++){
        string temp = int_to_string(i);
        if(i%100000 == 0){
            cout<<i/(double)max<<endl;
            cout<<temp<<endl;
        }
        if(sha256(temp).compare(needle) == 0){
            cout<< temp<<endl;
            auto t2 = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
            cout<<"Microseconds taken:"<<duration<<endl;
            cout<<i-min<<endl;
            return 0;
        }
    }
    cout<<"Not found"<<endl;
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    cout<<"Microseconds taken:"<<duration<<endl;
    cout<<min-min<<endl;
    return 0;
}