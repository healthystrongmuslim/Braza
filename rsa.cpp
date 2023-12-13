/*  
clang++ ~/storage/shared/codes/rsa.cpp -o rsa

  This is RSA.
  If the entered p q and e do not fulfill the conditions, they are automatically, randomly changed until they do.  p and q would be between 3 and 202.
  Created by Muhammad Musab on 20/10/2023.
*/
#include "stdc++.h"
using namespace std;
unsigned st=time(0);

int exuc(int a,int n){
    int q=0,u=1,v=0,oa,ou;
    cout<<"\33[0md="<<a<<"-¹mod "<<n<<endl;
    int om=n;
    cout<<"\33[36moa=a; a=n;//oa is the old a\nn=oa-n*q;\nou=u; u=v;\nv=ou-v*q;\nq= a/n;\na\tn\tq\tu\tv\n"
    <<a<<'\t'<<n<<'\t'<<q<<'\t'<<u<<'\t'<<v<<'\n';
    
    while(n!=1){
        oa=a; a=n;//oa is the old a
        n=oa-n*q;
        ou=u; u=v;
        v=ou-v*q;
        q=a/n;
        cout<<a<<'\t'<<n<<'\t'<<q<<'\t'<<u<<'\t'<<v<<'\n';
    }
    while(v<0) {v+=om; cout<<"v: "<<v<<endl;}
    cout<<"\33[32;1mFinal:\na\tn\tq\tu\tv\n"
    <<a<<'\t'<<n<<'\t'<<q<<'\t'<<u<<'\t'<<v<<"\33[0m\n";
    return v;
}
int gcd(int a,int b){int fac=1;
    for(int i=1;i<=((a<b)?a:b);i++) if(a%i==0 && b%i==0) fac=i;
    return fac;
}

bool prime(int a){
    bool fac=0;
    for(int i=2;i<a && !fac;i++) if(!(a%i)) fac=1;
    return !fac;
}

int main(){
    srand(st);
    cout<<"\33[43m\n\n\33[0m\np q e\n";
    int p,q;
    cin>>p>>q;
    while(p==q||!prime(p)||!prime(q)||p<3||q<3) {
        p=((p==q||!prime(p)||p<3)?rand()%200+3:p),
        q=((p==q||!prime(q)||q<3)?rand()%200+3:q);
        //printf("\33[36mrandomized (p,q)=(%i,%i)\n\33[0m",p,q);
    }
    
    int n=p*q, phi=(p-1)*(q-1), e=rand()%(phi-2)+2;
    cin>>e;
    while (gcd(phi, e)!=1 || e>=phi || e<2) e=rand()%(phi-2)+2;
    cout<<"\33[34;1mp\tq\tpq=n\n"<<p<<'\t'<<q<<'\t'<<n
    <<"\nphi(n)=(p-1)(q-1)="<<phi<<"\ne="<<e<<endl;
    int d=exuc(e,phi);
    
    cout<<"\33[31;1mKeys\nPublic (PU ={e,n})=\t{"<<e<<','<<n<<"}"<<"\nPrivate (PR ={d,n})=\t{"<<d<<','<<n<<"}\n\33[0m";
    
}

