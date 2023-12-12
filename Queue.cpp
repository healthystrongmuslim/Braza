//
//  Queue.cpp
//  mm
//
//  Created by Muhammad Musab on 20/09/2023.
//

#include "./stdc++.h"
using namespace std;

class Student{
    int a=17;
    string n="John Doe",i="90406";
    double g=3.0;
public:
    
    Student( string name="John Doe",int age=17, string id="90406",double gpa=3.0): a(age), n(name), i(id), g(gpa) {;}
    Student(Student &c): a(c.a), n(c.n), i(c.i), g(c.g) {;}
    
//    bool match(string m){int i=0,lm=0;bool mch=1;
//        for (;cnic[i++];); for(;m[lm++];);
//        if (i!=lm) return 0;
//        for(i=0; cnic[i] && mch;(cnic[i]==m[i++])? mch:mch=0)
//        ; return mch;}
    void print(){cout<<"Name: "<<n<<"\tID: "<<i<<"\tAge: "<<a<<"\tCGPA: "<<g<<endl;}
    friend class Node; friend class Queue;
};




class Node{
    Student n; Node *x; int p;//n=data, x=neXt, p=priority
public:
    Node(Student N, Node* nex=NULL, int pr=rand()%500): n(N), x(nex), p(pr) {;}
    Node(Node &N): n(N.n), x(N.x), p(N.p) {;}
    
    friend class Queue;
    Node* getx(){return x;}
    double getg(){return n.g;}
    int getp(){return p;}
    
    void setx(Node &a){x=&a;}
    void setx(Node* a){x=a;}
    void setn(Student N){n=N;}
    void setp(int pr){p=pr;}
};








class Queue{
public:
    Node *h,*l; unsigned short s=0; //h=head, l=last, s=size
    Queue():h(NULL), l(NULL), s(0) {;}
    
    int len(){
        if (h==NULL) s=0;
        else {Node* p=h; s=1;
            for(;p->x; p= p->x) s++;}
								return s;}
    void print(){Node* p= h;
        for(int i=0;p; i++, p=p->x) cout<< i<<':'<< p->p <<'\t';
            cout<<endl;}
    
				void enQ(Student &std, int pri){s++;
        if (!h) {h=new Node(std),l=h; return;}
        Node* p=l;
        l->x=new Node(std,NULL,pri), l=l->x;
    }
    void deQ(){
        if(h){
            Node *p=h->x;
            h->x=NULL; delete h;
            h=p; s--;}
    }
    void pbdeQ(){int high=-1; //tracking highest priority
        Node *p=h,*t=h;
        if(h){
            
            while(p->p<499 && p->x!=l) high= ((p->p>high)? p->p:high), p=p->x;
            s--;
            
            if (p->x==l && l->p>high) {delete l;l=p, l->x=NULL; return;}
            
            if(p->p==499) {while(t->x != p) t=t->x; t->x=p->x,p->x=NULL; delete p; return;}
            
            p=h; while(p->p!=high) p=p->x; while(t->x!=p) t=t->x; t->x=p->x,p->x=NULL; delete p;
        }
    }
    bool mt(){return !h;}
    
    void display(){Node *p=h;
        cout<<"\33[1;31mDisplaying Queue:\33[0m\n";
        if(h) {for(;p;p=p->x) {cout<<"Priority: "<<p->p<<endl; p->n.print(); cout<<endl;} cout<<"\33[1;31mEnd of Queue\n\33[0m";}
        else cout<<"\33[1;31mQueue is empty\n\33[0m";
    }
    void clear(){
        if(h){while(h->x){Node *p=h->x;
            if(p) {h->x=p->x, p->x=NULL;delete p;}} }
        delete h; h=NULL;
    }
    ~Queue(){if (!h) { clear();}
    }
};




int main (){
    Queue q;
    Student e1("Musab");
        Student e2("Sawd");
        Student e3("Braza");
        q.enQ(e1,3);
        q.enQ(e2,5);
        q.enQ(e3,1);
        q.display();
        q.deQ();
        q.display();
        q.pbdeQ();
        q.display();
        q.clear();
        cout << "Isempty : " << q.mt() << endl;
        //q.display();
}

/*
class IntQ{
    int f, r, N, *a;const unsigned s;
public:
    IntQ(unsigned S=10): s(S), f(-1), r(-1), N(0), a(new int[s]) {;}
    
    void enQ(int n){
        if (N) {
        if(Full()) {cout<<"Overflow\n"; return;}
        else N++,r++, a[mod(r,s)]=n;
        }
        else {f=0,r=0, a[0]=n, N++;}
    
    }
    void MakeNull(){
        ;
    }
    void print(){if(!N) {cout<<"Queue is empty\n"; return;}
        cout<<"N:"<<N<<"\tFront:"<<f<<"\tRear:"<<r<<endl;
        for(int i=0;i<N;cout<<(f+i)%s<<':'<<a[f+i++]<<'\t')
        cout<<endl;}
    
    int deQ(){int n=0;
        if (N) {N--,n=a[f];
            if (N==1) f=-1,r=-1;
            else f++, mod(f,s);
    }
    else cout<<"Queue is empty\n";
        return n;
    }
    const unsigned gets(){return s;}
    bool Full(){return N==s;}
    bool mt(){return !N;}
    ~IntQ(){delete[] a;}
};
int main(){
    IntQ qqq(100); unsigned st=time(0); srand(st);
    qqq.print();
    for (int i=0; i<qqq.gets();i++) if (rand()%2) qqq.enQ(rand()%100),qqq.print(); else qqq.deQ(), qqq.print();
    qqq.print();
}*/
