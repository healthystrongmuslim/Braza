/*
for stats/prob graphs
Muhammad Musaab
*/
#include <iostream>
#include <stdio.h>
using namespace std;
#define D 5 //denominator
#define N 2 //numerator
// P(Success)= N/D
int main(){
srand(time(0));
int n=0, *a=0, x=0,done=1, ddone=0;
printf("Enter n set-size, x number of sets\t");
cin>>n>>x;
int xby100=x/100;
printf("done:");
a=new int[n+1];
for(int i=0, k=0; i<x; a[k]++,i++, k=0){
    for(int j=0;j<n;j++, k+= ((rand()%D)<N));
    //success has 1/5 chance of coming --> D=5, N=1
    if((i/(xby100))==done){
    printf("%i",(done-1)%10);done++;fflush(stdout);
    if(done%10==1) ++ddone,printf("\e[10D\e[0K\e[4%im",ddone%8);
    }
}
printf("\e[m\n\e[A\e[J");
//reset color, next line, cursor up 1 line, clear from cursor to end of screen
for(int i=0,csum=0;csum!=x; printf("\e[9%im%i%c",i%8,i,(i%8==7)?'\n':' '),csum+=a[i++]);
printf("\e[m\n");//cumulative sum
for(int i=0,csum=0;csum!=x; printf("\e[9%im%i%c",i%8,a[i],(i%8==7)?'\n':' '),csum+=a[i++]);
printf("\e[m\n");
for(int i=0,csum=0;csum!=x//(i<n*N/float(D)) or a[i]//i< E(x)=setsize x chance = n * (N/D)
;i++){
    printf("\e[0;9%im%i\t\e[10%im",i%8, i,i%8);
    for(float j=0;j<(a[i]/float(x));j+=0.008) printf(" ");
    printf("\e[0;90m%i\n",a[i]);csum+=a[i];
}
delete [] a;
printf("\e[m");
}
