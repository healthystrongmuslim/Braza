#include<stdio.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<cmath>
#include<pthread.h>
#include<sstream>
char cord[]="co_ordinateList.txt";//co-ordinate printing to a file
struct winsize w;
char garbage = ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
int N = w.ws_col; //code tries for different values of n, from 0 till N — determines width of graph. Here it is equal to terminal width (might not work on Windows; different #includes needed).
#define SCALE 4 //for larger/smaller range of test set, increment scaled_n by this value in every iteration
//scaled_n is the actual tested value, and is esentially the product n*SCALE
//max tested value is (N-1)*SCALE
int Offset=w.ws_row+1;// vertical offset for graph (now equa to terminal height. Previously 101: 100 because plots were made as a % of the greatest obtained value of f(n), +1 because of the ANSI escape code having a min-value of 1.
const int A=0,B=2,C=1,D=1;
FILE* co_ordinatelist;
void* testcomplexity(void* N) {
    //THE FUNCTION TO CHECK THE COMPLEXITY OF
    int i=0,j=0,n=(int)((unsigned long)N),cntod=pow(n,D)*C,sum=0;
    unsigned long nbyb=n/B,count=0;
    
    pthread_t* subtcalls=0;//for threaded-recursive calls to run, while caller itself also does its own processing
    if (n >= B && B>1 && A>0) {//recursive
        subtcalls=new pthread_t[A];
        for(j=0;j<A;j++) pthread_create(&subtcalls[j],0,testcomplexity,(void*)nbyb);
    }
    i=1;
    while(i <= cntod) {//function's own processing i.e. Θ(n) (theta of n) in some notations
        for(j=1;j<=i;j*=2)
        count++;//increment count for every "step"
        i*=2;
    }
    
    if (subtcalls) {void* store=0;//joining and summing up the recursive calls. could also be replaved
        for(j=0;j<A;j++) pthread_join(subtcalls[j],&store),
       count+=(unsigned long)store,
       store=0;
       delete[] subtcalls;
    }
    pthread_exit((void*)count);
    //THE FUNCTION TO CHECK THE COMPLEXITY OF
}

int main() {
    printf("\e[2J\e[HStart\n"); //clear screen
    co_ordinatelist = fopen(cord,"w");
    std::stringstream coords;//to store string of coordinates (n,f(n))
    int *F_Of = new int[N],
    max = 0,
    maxn = 0,
    ddone = 0; float apercentofN = float(N)/100;
    pthread_t *ogcalls=new pthread_t[N];
    unsigned long scaled_n=0;
    for(int n = 0; n < N; n++, scaled_n += SCALE) {
        F_Of[n] = 0; //THE FUNCTION TO CHECK THE COMPLEXITY OF
        pthread_create(&ogcalls[n],0,testcomplexity,(void*)scaled_n); //THE FUNCTION TO CHECK THE COMPLEXITY OF
    }

    for(int n = 0,p=0,scaled_n=0; n<N; n++, scaled_n+=SCALE){
        void* store=0;
        pthread_join(ogcalls[n],&store);
        F_Of[n]=(unsigned long) store;
        store=0;
        coords<<"("<<scaled_n<<","<<F_Of[n]<<"),";if(!((n+1)%10)) coords<<"\n";//appending to string of coordinates
        if (F_Of[n] > max) max = F_Of[n],
        maxn = scaled_n;
        if(n > int(apercentofN*p)) {//PROGRESS BAR
            p=(n*100)/N;
            p++;
            printf("%i", int(p-1)%10);fflush(stdout);
            if(int(p)%10 == 1) ++ddone,
            printf("\e[10D\e[0K\e[4%im", ddone%8);
        }//PROGRESS BAR

        //printf("\e[42m\e[%i;%iH%i",SN-count+Offset,n+1,n%10); //count determines the height/y-coordinate, n determines horizontal offset from left. See: https://en.m.wikipedia.org/wiki/ANSI_escape_code#Description
        //if(n<10) coords<<"("<<scaled_n<<","<<count<<"),";if(!((n+1)%10)) coords<<"\n";//appending to string of coordinates
    }
    printf("\e[0m\e[2K");
    for(int n = 0; n < N; n++) {
        printf("\e[30;4%im\e[%i;%iH%i",
            (n/10)%7+1, //change background every ten steps
            Offset-int(((Offset-1)*F_Of[n])/max),//as percentage of greatest F_Of
            n+1, n%10); //plots (n,f(n))

        if(!(n%10) || n+1 == N) printf("\e[0;100m\e[%i;%iH%i", Offset+1, n+1, n*SCALE); //prints the n-axis
    }
    printf("\e[%iH\e[0;94mSCALE=%i\nwindow (width/col,height/rows)=(%i,%i)\nGreatest=f(%i)=%i\tco-ordinates in: %s\n", Offset+7, SCALE, N,Offset-1, maxn, max,cord);
    fprintf(co_ordinatelist,"%s\n", coords.str().c_str());
    //printf("%s\n", coords.str().c_str());
    delete [] F_Of;
}