/*
clang++ ~/storage/shared/codes/london.cpp -o L
Muhammad Musaab
*/
#include "stdc++.h"
using namespace std;

int main(){
	string s; int i=1;
	cout<<"Enter string:";
	getline(cin, s);
	cout<<s<<endl;
	for(i=1; s[i]; cout <<s[i++]<<endl){
		cout <<s[i];
		for (int j=0;j< i-1;j++) cout <<"⠀";
	}
}