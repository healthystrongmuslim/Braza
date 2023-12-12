//
//  tree.cpp
//  mm
//
//  Created by Muhammad Musab on 18/10/2023.
//
#include "./stdc++.h"
using namespace std;

class Node{
    Node** child;//i (tried to) have a general cn-ary tree made, each node having cn children, and by default 2 children
    int cn, d;//d is the data
public:
    Node(int data, int chldrn=2): d(data), cn(chldrn), child(new Node* [chldrn]) {for(int i=0;i<cn;child[i++]=NULL);}
    
    Node* operator[](int n){return (n<cn && n>=0) ? child[n]: NULL;}
    bool newchild(int data){int i=-1;
        for(;i<cn && child[i++];);
        if(i<cn) child[i]=new Node(data);
        return i<cn;}
    Node* nth(int n){ return &(this[n]);}
    
    void insert(int n){if(d>n) {if (child[0]) child[0]->insert(n); else child[0]=new Node(n);}
        else if(d<n) {if (child[1]) child[1]->insert(n); else child[1]=new Node(n);}
            else if(d==n) NULL;}
    
    void inorder(){
        if (child[0]) child[0]->inorder();
        cout<<d<<' ';
        if(child[1]) child[1]->inorder();}
    
    int getdata() {return d;}
    int count(){if(leaf()) return 1;int c=1;
        for(int i=0;i<cn;i++) if (child[i]) c+=child[i]->count();
        return c;}
    int height(){if (leaf()) return 0; int ht=0;
            for(int i=0;i<cn;i++) if (child[i]) if (ht<child[i]->height()) ht=child[i]->height();
            return 1+ht;}
    Node* max(){Node* t=this;
        while(t->child[1]) t=t->child[1];
        return t;}
    Node* min(){Node* t=this;
        while(t->child[0]) t=t->child[0];
        return t;}
    int findMax(){return max()->d;}
    int findMin(){return min()->d;}
    bool search(int n){ if(find(n)) cout<<"Found "<<n<<endl; return  (!!find(n));}
    Node* find(int n){Node* t=this;
        while(n!=t->d && t) t=t->child[(n> t->d)];
            return t;}
    bool leaf(){int i=0,n=0; for(;i<cn; (child[i])? i++,n++:i++)
        ; return n==cn;}
    bool onechild(){int i=0,n=0; for(;i<cn; (child[i])? i++,n++:i++)
        ; return n==1;}
    
    void del(int n){Node* t= find(n); //return 0 if the deleted Node is the root
        if(t){
            if(t->leaf()) {cout<<"deleting leaf with data:"<< t->d <<endl; delete t; t=0;}
            
            else if (t->onechild() && t!=this) {cout<<"deleting one-child node with data:"<< t->d <<endl;
                Node *temp =t->child[!t->child[0]],/*whichever child of 2 exists is accessed*/ *par=this;
                while(n!=par->child[0]->d && n!=par->child[1]->d) par=par->child[(n> par->d)];
                par->child[t==par->child[1]] =temp;
                t->child[!t->child[0]]=0;
                delete t; t=0;}
            
            else if (t->onechild() && t==this) {
                t->child[!t->child[1]]=0; delete t;}
            
            else if (t->child[1] && t->child[0]) {
                Node* min= t->child[1]->min(), *par=t->child[1]; int mindat=min->d;
                while(par!=min && par->child[0]!=min) par=par->child[0];
                Node *R=min->child[1];
                (par==min)? t->child[1]=R: par->child[0]=R;
                min->child[1]=0; delete min;
                t->d=mindat;}
        }
    }
    
    ~Node(){for(int i=0; i<cn;i++) if(child[i]) {cout<<"\33[0;31mDeleting child["<<i<<"] of "<<d<<"\33[0m\n"; delete child[i]; child[i]=NULL;}
        delete [] child; child=NULL;
        cout<<"\33[31;1mDeleted children of "<<d<<"\33[0m\n";}
    
    friend class BST;
    
};

class BST{
    Node* root;
public:
    BST(Node* rt=NULL): root(rt) {;}
    void insert(int n){root->insert(n);}
    int treeNodeCount(){return (root)? root->count():0;}
    int treeHeight(){return (root)? root->height():-1;}
    void inorder(){if (root) root->inorder();}
    void del(int n){if(root) {if(root->onechild()) {Node *child= root->child[!root->child[1]];
        root->del(n); root=child;}
        else root->del(n);}}
    ~BST(){delete root;}
    bool empty(){return !root;}
};
/*
int main(){
    BST tree= new Node(0); int n;
    for(int i=-1;i++<6;tree.insert(n)) cin>>n;
    cout<<"Done insert\n";
    tree.inorder();
    cout<<"\nDone inorder\n";
}
*/
