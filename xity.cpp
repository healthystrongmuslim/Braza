#include<stdio.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<cmath>
//#include<sstream> //for co-ordinate printing (n, f(n)), in case a table needs to be made
struct winsize w;
char garbage = ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
int N = w.ws_col; //trying for different values of n, from 0 till N — determines width of graph. Here it is equal to terminal width
#define SCALE 8 //for larger/smaller range of test set, increment scaled_n by this value in every iteration
//scaled_n is the actual tested value, and is esentially the product n*SCALE
//max tested value is (N-1)*SCALE
#define Offset 101// vertical offset for graph. 100 because plots are made as a % of the greatest obtained value of f(n). +1 because of the ANSI escape code having a min-value of 1.

void testcomplexity(int n, int &count) {
    //THE FUNCTION TO CHECK THE COMPLEXITY OF
    int i = 0, A=2,B=2,C=1,D=1,nbyb=n/B,cntod=pow(n,D)*C;//from the form: T(n)=AT(n/B)+Cn^D
    if (n >= 2) for(int j=0;j<A;j++) testcomplexity(nbyb, count);
    while(i < cntod) {
            count++; //increment count for every "step"
        i++;
    }
    //THE FUNCTION TO CHECK THE COMPLEXITY OF
}

int main() {
    printf("\e[2J\e[HStart\n"); //clear screen
    int *fofn = new int[N],
    max = 0,
    maxn = 0,
    scaled_n = 0,
    ddone = 0,
    smax=0,
    smaxn=0; float apercentofN = float(N)/100;
    //std::stringstream coords;//to store string of coordinates (n,f(n))
    for(int n = 0,p=0; n < N; n++, scaled_n += SCALE) {
        fofn[n] = 0; //THE FUNCTION TO CHECK THE COMPLEXITY OF
        testcomplexity(scaled_n, fofn[n]); //THE FUNCTION TO CHECK THE COMPLEXITY OF
        if (fofn[n] > max) smax=max, max = fofn[n],
        smaxn=maxn, maxn = scaled_n;
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
            Offset-int((100*fofn[n])/max),//as percentage of greatest fofn
            n+1, n%10); //plots (n,f(n))

        if(!(n%10) || n+1 == N) printf("\e[0;100m\e[%i;%iH%i", Offset+1, n+1, n*SCALE); //prints the n-axis
    }
    for(int jj=10;jj<=100;printf("\e[%iH\e[10%im%i",Offset-jj+1,jj%8,jj),jj+=10);
    printf("\e[%iH\e[0;94mEnd\nSCALE=%i\twindow width=%i\tGreatest=f(%i)=%i\t2nd Greatest=f(%i)=%i\n", Offset+1, SCALE, N, maxn, max,smaxn,smax);
    //printf("%s\n", coords.str().c_str());
    delete [] fofn;
}


