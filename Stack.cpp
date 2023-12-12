//
//  Stack.cpp
//  mm
//
//  Created by Muhammad Musab on 22/09/2023.
//

#include "./stdc++.h"
using namespace std;
unsigned st=time(0);

class Node{
    int c; Node *x;
public:
    Node(char C=0): c(C), x(NULL) {;}
    int getc(){return c;}
    friend class Stack;
};
/*
	Question 1: [5+5+10+5=25 Marks]
	a. Write a C++ program that uses a stack to convert a binary number into an
	equivalent decimal number and test your function for the following values:
	11000101, 10101010, 11111111, 10000000, 1111100000
	b. Write a program that uses a stack to convert a decimal number into an
	equivalent binary number and test your function for the following values: 4,
	5, 6, 7, 8, 9.
	c. Create a program that reads the postfix form of an arithmetic expression,
	evaluates it, and outputs the result. Assume that the expression consists of
	single-digit, nonnegative integers (‘0’ to ‘9’) and the five basic arithmetic
	operators (‘+’,’-‘,’*’,’/’,'^’) (note to correctly handle ‘^’). Further assume
	that the arithmetic expression is input from the keyboard with all the
	characters separated by white space on one line.
	d. Create a test plan involving the execution of 5 expressions for which you
	must provide the infix and postfix notations, alongside their result in the
	form of a short report document.
*/
class Stack{
    unsigned n; Node *h;// n is number of items in stack, h is head.
public:
    Stack(): n(0), h(NULL){;}
    
				int push(int c){Node *t=h;
								void(n++), void(h=new Node(c)), h->x=t;
        return c;}
    int pop(){if(h){int c;Node *t;
								t=h->x; void(c=h->c), h->x=NULL;
								delete h; void(h=t), n--;
								return c;}
        else cout<<"\33[1;31mStack empty\n\33[0m";return -1;}
				void peak(){cout<<"\33[1;33mpeak: "<<h->c<<"\33[0m\n";}
    bool empty(){return !h;}
				int getn(){return n;}
    void print(){for(Node *t=h;t;t=t->x) cout<<"\33[1;34m"<< t->c <<'\t'<<"\33[0m"; cout<<endl;}
				void clear(){while(n) pop();}
				
    ~Stack(){clear();}
};
int b2d(int n){
				Stack s;
				while(n) void(s.push(n%10)), n/=10;
				while(!s.empty()) n= n*2+s.pop();
								return n;
}
int d2b(int n){
				Stack s;
				while(n) void(s.push(n%2)), n/=2;
				while(!s.empty()) n= n*10+s.pop();
								return n;
}
int isop(char c){return 0+ (c=='+')*1 + (c=='-')*2 + (c=='*')*3 +(c=='/')*4 + 5*(c=='^');}
int postfixeval(string s){
				int val=0,l=0,r=0;
				Stack st;
				int i=0;
				for(;s[i];i++) {
								if(s[i]<='9' && s[i]>='0') st.push(s[i]-'0');
								switch(isop(s[i])){
												case 1:r=st.pop(), l=st.pop(), val=l+r, st.push(val);break;
												case 2:r=st.pop(), l=st.pop(), val=l-r, st.push(val);break;
												case 3:r=st.pop(), l=st.pop(), val=l*r, st.push(val);break;
												case 4:r=st.pop(), l=st.pop(), val=l/r, st.push(val);break;
												case 5:r=st.pop(), l=st.pop(), val=pow(l,r), st.push(val);break;
												default: break;
								}
				}
				return val;
}
/*
int main(){srand(st);
    cout<<"enter -1 to stop\n\n";
				int i=0;string s;
				for(;s[0]!='-';cout<<"\33[0m"<<postfixeval(s)<<"\n\33[32mnext choice:") getline(cin, s);
//				for (;i>=0;cout<<"\33[0m"<<b2d(i)<<"\n\33[32mnext choice:") cin>>i;
//				for (;i>=0;cout<<"\33[0m"<<d2b(i)<<"\n\33[32mnext choice:") cin>>i;
}
*/
