//
//  AVL.cpp
//  mm
//
//  Created by Muhammad Musab on 2/11/2023.
//
#include "./stdc++.h"
using namespace std;

class Node{
    Node** child;//i (tried to) have a general cn-ary tree made, each node having cn children, and by default 2 children
    int cn, d;//d is the data
public:
    Node(int data, int chldrn=2): d(data), cn(chldrn), child(new Node* [chldrn]) {for(int i=0;i<cn;child[i++]=NULL);}
    
    Node* operator[](int n){return (n<cn && n>=0) ? child[n]: NULL;}
    int getdata() {return d;}
    
    bool insert(int n){bool flag=0;//to check whether it was inserted
        if(d>n){if (child[0]) flag=child[0]->insert(n);
												else child[0]=new Node(n),flag=1;}
    else if(d<n) {if (child[1]) flag=child[1]->insert(n);
								else child[1]=new Node(n),flag=1;}
    else if(d==n) flag=0;
        //balancing
        if(flag) for(int i=0;i<cn;i++){
            if(child[i]->bal()==-2){//right heavy
                Node* z=child[i],*y,*x;
                if(z->child[1]->bal()==-1){//right heavy
                    y=z->child[1], z->child[1]=y->child[0],y->child[0]=z, child[i]=y;//left rotate z
                }
                else if(z->child[1]->bal()==1){//left heavy
                    y=z->child[1],x=y->child[0], z->child[1]=x,y->child[0]=x->child[1],x->child[1]=y;//right rotate y, left rota z
                    z->child[1]=x->child[0], x->child[0]=z,  child[i]=x;
                }
            }
            else if(child[i]->bal()==2){//left
                Node* z=child[i],*y,*x;
                if(z->child[0]->bal()==1){//left
                    y=z->child[0], z->child[0]=y->child[1],y->child[1]=z, child[i]=y;//right rotate z
                }
                else if(z->child[0]->bal()==-1){//right
                    y=z->child[0],x=y->child[1], z->child[0]=x,y->child[1]=x->child[0],x->child[0]=y;//left rotate y, right rota z
                    z->child[0]=x->child[1],x->child[1]=z, child[i]=x;
                }
            }
        }
        return flag;
    }
    void inorder(){
        if (child[0]) child[0]->inorder();
        cout<<d<<' ';
        if(child[1]) child[1]->inorder();}
    
    int count(){if(leaf()) return 1;int c=1;
        for(int i=0;i<cn;i++) if (child[i]) c+=child[i]->count();
        return c;}
    int height(){if (leaf()) return 0; int ht=0;//tracks greatest height of child
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
        while((t)? n!=t->d : 0) t=t->child[(n> t->d)];
            return t;}
    
    bool leaf(){int i=0,n=0; for(;i<cn; (child[i])? i++,n++:i++)
        ; return n==cn;}
    bool onechild(){int i=0,n=0; for(;i<cn; (child[i])? i++,n++:i++)
        ; return n==1;}
    int whichone(){for(int i=0;i<cn;i++) if(child[i]) return i; cout<<"\33[31;1mWHICH ONE IS -1\33[0m\n"; return -1;}
    
    void del(int n){Node* t= find(n);
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
				bool full(){if(leaf()) return 1;
        bool hai=1;//full hai ya nahi
        for(int i=0;i<cn && hai;hai=((child[i])? (child[i]->full())? hai:0:0),i++);
        return hai;}
    bool perfect(){if(leaf()) return 1;
        return count() == (pow(cn,height()+1)-1)/(cn-1);}
    bool degenerate(){return (leaf())? 1: (onechild())? child[whichone()]->degenerate():0;}
				
				int bal(){int bal=0;
        if(child[0]) bal+=child[0]->height();
        if(child[1]) bal-=child[1]->height();
        return bal;}
				bool bald(){if(leaf()) return 1; bool bald=1;
								if(bal()>1 || bal()<-1) bald=0;
								for(int i=0;i<2 && bald;i++) if(child[i]) if(child[i]->bal()>1 || child[i]->bal()<-1 || !child[i]->bald()) bald=0;
								return bald;}
				
				~Node(){for(int i=0; i<cn;i++) if(child[i]) {delete child[i]; child[i]=NULL;}
        delete [] child; child=NULL;}
    
    friend class AVL;
    
};

class AVL{
    Node* root;
public:
    AVL(Node* rt=NULL): root(rt) {;}
    void insert(int n){if(root) root->insert(n); else root=new Node(n);
        if(root->bal()==-2){//right heavy
            Node *z=root,*y,*x;
            if(z->child[1]->bal()==-1){//right heavy
                y=z->child[1], z->child[1]=y->child[0],y->child[0]=z, root=y;//left rotate z
            }
            else if(z->child[1]->bal()==1){//left heavy
                y=z->child[1],x=y->child[0], z->child[1]=x,y->child[0]=x->child[1],x->child[1]=y;//right rotate y, left rota z
                z->child[1]=x->child[0],x->child[0]=z, root=x;
            }
        }
        else if(root->bal()==2){//left
            Node *z=root,*y,*x;
            if(z->child[0]->bal()==1){//left
                y=z->child[0], z->child[0]=y->child[1],y->child[1]=z, root=y;//right rotate z
            }
            else if(z->child[0]->bal()==-1){//right
                y=z->child[0],x=y->child[1], z->child[0]=x,y->child[1]=x->child[0],x->child[0]=y;//left rotate y, right rota z
                z->child[0]=x->child[1],x->child[1]=z, root=x;
            }
        }
    }
				bool bald(){if(root)
								{if (root->leaf()) return 1; bool bald=1;
								if(root->bal()>1 || root->bal()<-1) bald=0;
												for(int i=0;i<2 && bald;i++) if(root->child[i]) if(!root->child[i]->bald()) bald=0;
												}
								return 1;
				}
				bool leaf(){return (root)?root->leaf():0;}
				bool onechild(){return (root)?root->onechild():0;}
				int whichone(){return (root)?root->whichone():0;}
				Node* max(){return (root)?root->max():0;}
				Node* min(){return (root)?root->min():0;}
				int findMax(){return (root)?root->findMax():0;}
				int findMin(){return (root)?root->findMin():0;}
				bool search(int n){return (root)?root->search(n):0;}
				bool full(){return (root)? root->full():0;}
				bool degenerate(){return (root)?root->degenerate():0;}
				bool perfect(){return (root)?root->perfect():0;}
    void breadthfirst(){
        queue<int> items;
								if(root){Node* temp=root;
            for(items.push(temp->d); !items.empty(); items.pop() ){
																temp=root->find(items.front());
                for(int i=0;i<root->cn;i++) if (temp->child[i]) items.push(temp->child[i]->d);
																cout<<items.front()<<endl;
            }
        }
    }
				int treeNodeCount(){return (root)? root->count():0;}
    int treeHeight(){return (root)? root->height():-1;}
    void inorder(){if (root) root->inorder();}
    void del(int n){if(root) {
								if(root->onechild()) {Node *child= root->child[!root->child[1]];
        root->del(n); root=child;}
        else root->del(n);}	}
				
    ~AVL(){delete root;}
    bool empty(){return !root;}
};
void foo(){;}
/*
	int main(){
				
    AVL tree= new Node(0); int n;
    for(int i=-1;i++<6;tree.insert(n)) cin>>n;
    
}
*/
