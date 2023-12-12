/*
g++ -o r retardedcap.cpp
Muhammad Musaab
*/
#include "./stdc++.h"
using namespace std;

int main(){
    string s; int i=0; unsigned st=time(0); srand(st);
    cout<<"Enter string:";
    getline(cin, s);
    while(s[i]) cout<< char( isalpha(s[i])? isupper(s[i])? s[i++]+32*(rand()%2) : s[i++]-32*(rand()%2) : s[i++]); cout<<endl;
}

