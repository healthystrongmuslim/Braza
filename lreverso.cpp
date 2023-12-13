/*  
clang++ ~/storage/shared/codes/lreverso.cpp -o lr

*///
//  mm
//
//  Created by Muhammad Musab on 22/09/2023.

//

#include "stdc++.h"
using namespace std;
unsigned st=time(0);
class Node{
    char c; Node *x;
public:
    Node(char C=0): c(C), x(NULL) {;}
    char getc(){return c;}
    friend class List;
};

class List{
    Node *h; int len=0;
public:
    List():  h(NULL), len(0){;}
    void add(char c=0){Node *p=h,*t=h;
    if(h) {for(;p;t=p,p=p->x);t->x= new Node(c);}
    else h=new Node(c);
    }
    
    void print(){for(Node *p=h; p;p=p->x) cout<<"\33[32m"<<p->c<<((p->x)?"→":"\33[0m\n");}
    void lreverso(){Node *x=NULL,*c=h,*p=NULL;//next,cur,prev
    while(c) {x=c->x, c->x=p, p=c, c=x, h=p; print();};}
    
    
    void clear(){Node *p=h;while(h) {p=h->x, h->x=NULL; delete h; h=p;}}
    ~List(){while(h) clear();}
};

int main(){srand(st);
    string s;
    List l;getline(cin,s);
    for(char c:s) l.add(c), l.print();
    l.lreverso(), l.print();
}