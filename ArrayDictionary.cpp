/**  ArrayDictionary..cpp
 Practise Task 1
 Created by Muhammad Musab on 27/08/2023
 */
#include "stdc++.h"
using namespace std;
unsigned st=time(0);
class Person{
    string name, adrs, cnic;
public:
    Person(string c="000000000", string n="John Doe", string a="Karachi, Dead Sea"): name(n), adrs(a), cnic(c) {;}
    void reasinc(unsigned short l=1){srand(st*l); int i=0; while(cnic[i]) cnic[i++]='0'+rand()%10;}
    char disp(){cout<<"\33[1;42m"<<cnic<<'\t'<<name<<'\t'<<adrs<<"\33[0m\n"; return 0;}
    string getc(){return cnic;}
    
    bool armach(string m){int i;bool mch=1;
        for(i=0; cnic[i] && mch;(cnic[i]==m[i++])? mch:mch=0)
        ; return mch;}
    
    bool dctmch(string m){int i; bool mch=1;
        for(i=1; cnic[i] && mch;(cnic[i]==m[i++])? mch:mch=0)//dictionary matching just starts one letter forward
        ; return mch;}
    
    void operator=(string c){int i=0; while(cnic[i]) cnic[i++]=c[i];}
};
bool valdtr(const string);
void dltarr(Person**);
bool srch(string, Person*, int, std::chrono::duration<long double, milli> &);
bool dserch(string, Person**, int*, std::chrono::duration<long double, milli> &);




int main(){
    srand(st);
    int i=0,pop=0,j=0, dig=0,digcont[10]={0},digcont1[10]={0};
//    string cn;
    cout<<"\33[1;34mEnter population size:\t";
    cin>>pop;
    Person* ppl=new Person[pop];
    for(i=0;i<pop; i++) ppl[i].reasinc(rand()); //reassign random cnics
    for(i=0;i<pop;digcont[(ppl[i++].getc()[0])-'0']++)//increments counts of cnic starting-digit, so dynamic dictionary can be made accordingly
    ;cout<<"\33[0mFirst digit count:\n";
    for(i=0;i<10;cout<<i<<':'<<digcont[i++]<<' ')
    ; cout<<endl;
    
    Person** dict=new Person*[10];//making dictionary
    for(i=0;i<10; dict[i++]=new Person[digcont[i]])
        ;for(i=0; i<pop;dig=(ppl[i].getc()[0]-'0'), dict[dig][digcont1[dig]++]=ppl[i++].getc())//entering ppl into dictionary
            ;
//    char y='y';
//    while(y=='y'){
//        cout<<"Enter cnic to search:\t\33[36m"; cin>>cn; cout<<"\33[0m";
    std::chrono::duration<long double, milli> timea_put, timeb_put;
    for(i=0;i<pop;i++){
        srch(ppl[i].getc(),ppl,pop,timea_put);
        dserch(ppl[i].getc(),dict,digcont,timeb_put);
    }
    timea_put /=pop;
    timeb_put /=pop;
    cout << "\33[1;44mArray took:\t\t" << timea_put.count() << " milliseconds on average\33[0m\n";
    cout << "\33[1;44mDictionary took:\t" << timeb_put.count() << " milliseconds on average\33[0m\n\n";
    
    bool fstr = timea_put.count() > timeb_put.count();
    cout<<"\33[1;43m"<<((fstr)? "dictionary":"array")<<" was faster\33[46m\nTime difference:\t"<<
    ((fstr)? timea_put.count()-timeb_put.count() : timeb_put.count()-timea_put.count())<<
    "\nTime ratio:\t\t"<<
    ((fstr)? timea_put.count()/timeb_put.count() : timeb_put.count()/timea_put.count())<<"\n\33[0m";
//        cout<<"Search another cnic? (Enter y for yes)\t"; cin>>y;
//    }
//    cout<<"Nice searching\n";
    dltarr(dict);
    delete[] ppl;
}


bool valdtr(const string m){
    int i=0;bool numonly=1;
    for(;m[i] && numonly;i++) if(m[i]<'0' && m[i]>'9') numonly=0;
    return (i==9 && numonly);
}

void dltarr(Person**A){//deallocate memory
    unsigned short i;
    for (i=0;i<10;delete[] A[i++]);
    delete[] A;
    A=NULL;
}


bool srch(const string m, Person* ppl, int pop, std::chrono::duration<long double, milli> &time_put){
    int i=0;bool mch=0;
    if (valdtr(m)) {std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        for(i=0; i<pop && !mch;(ppl[i++].armach(m))? mch=1:mch=0);
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        time_put += t2 - t1;}
    else {mch=0;
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        time_put = t1 - t1;}
    //cout<< "\33[1mCNIC "<<((mch)?"\33[32mfound":"\33[31mNOT found")<<"!\33[0m\n";
    return mch;
}
bool dserch(const string m,Person**pdct, int digc[10], std::chrono::duration<long double, milli> &time_put){
    int i=0;bool mch=0;
    if (valdtr(m)) {std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        for(i=0; i<digc[m[0]-'0'] && !mch; mch=pdct[m[0]-'0'][i++].dctmch(m));
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        time_put += t2 - t1;}
    else {mch=0;
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        time_put = t1 - t1;}
    //cout<< "\33[1mCNIC "<<((mch)?"\33[32mfound":"\33[31mNOT found")<<"!\33[0m\n";
    return mch;
}
