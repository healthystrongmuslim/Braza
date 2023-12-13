/*  
clang++ ~/storage/shared/codes/diffie.cpp -o dif

  This is Diffie-Hellman
  Created by Muhammad Musab on 20/10/2023.
*/
#include "stdc++.h"
using namespace std;
unsigned st=time(0);/*
int sqrmult(int a,int b, int n){
    int r=1;
    cout<<"\33[35mr\ta\tb\n"<<r<<'\t'<<a<<'\t'<<b<<'\n';
    while(b) {if(b%2) r=(r*a)%n;
    b/=2,a=(a*a)%n;
    cout<<r<<'\t'<<a<<'\t'<<b<<'\n';}
    cout<<"\33[0;31mfinal r:"<<r<<endl;
}*/

int main(){int a, b, n, r=1;
cout<<"M e n d\n";
    cin>>a>>b>>n;
    cout<<"\33[35mC=M^e mod n\nInitially:r=1,a=M, b=e.\nif b is odd r=(r*a)\%n; \nb/=2,a=(a*a)\%n\nwhile(b)\nr\ta\tb\n"
    <<r<<'\t'<<a<<'\t'<<b<<'\n';
    while(b) {if(b%2) r=(r*a)%n;
    b/=2,a=(a*a)%n;
    cout<<r<<'\t'<<a<<'\t'<<b<<'\n';}
    cout<<"\33[0;31;1mCiphertext C="<<r
    <<"\n";
    
    a=r, r=1;
    cin>>b;
    cout<<"\33[0;35mM=C^d mod n\nr\ta\tb\n"
    <<r<<'\t'<<a<<'\t'<<b<<'\n';
    while(b) {if(b%2) r=(r*a)%n;
    b/=2,a=(a*a)%n;
    cout<<r<<'\t'<<a<<'\t'<<b<<'\n';}
    cout<<"\33[0;31;1mMessage M:"<<r<<endl;
}