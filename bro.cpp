/*

*/
#include <chrono>
#include <pthread.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#define CLK std::chrono::high_resolution_clock
#define J 30 //number of particles/threads
#define T 0.05 //printing delay
#define R 5 //radius of movement
#define SETTIME 4 //number of seconds after

using namespace std;
void* timmr1(void*),*timmr2(void*), plswait(long double);
const char LEN= 2*R +1; //side length of movement square
const char PLACES=LEN*LEN;//number of places that can be moved to
const char MIDPO=PLACES/2;//no-movement point
char c=0;//tap any key to stop the threads' loops
std::chrono::duration < long double,milli > tput[J],tone;
unsigned long long roundno[J]={0},rone;

int main(){
    pthread_t aa[J];
    printf("\e[3J\e[31mHELLO WORLD");
    ////
     static struct termios oldt, newt;
    /*tcgetattr gets the parameters of the current terminal
    STDIN_FILENO will tell tcgetattr that it should write the settings
    of stdin to oldt*/
    tcgetattr( STDIN_FILENO, &oldt);
    /*now the settings will be copied*/
    newt = oldt;

    /*ICANON normally takes care that one line at a time will be processed
    that means it will return if it sees a "\n" or an EOF or an EOL*/
    newt.c_lflag &= ~(ICANON);
    newt.c_lflag &= ~ECHO;
    /*Those new settings will be set to STDIN
    TCSANOW tells tcsetattr to change attributes immediately. */
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);

    /*This is your part:
    I choose 'e' to end input. Notice that EOF is also turned off
    in the non-canonical mode
    while((c=getchar())!= 'e')
        putchar(c);*/
    //
    char IAm='A',r;
    for(r=0;r < J; r++,IAm++) pthread_create(&aa[r], 0, timmr1, (void*)IAm);
    //c=getchar();//to make it so the iterations stop when you enter a key, uncomment this line and comment the next
    plswait(SETTIME);c++;//to make it so the iterations stop after the settime (in seconds), uncomment this line and comment the above.
    //printf("\e[20H\e[K\e[0;31mC:%c, %i r=%i\e[0m\t",c,c,r),
    for(r=0;r<J;r++)
        pthread_join(aa[r],0)
        //, printf("\n\e[0;1;43mThread took:\t\t%Lf milliseconds for %Lu rounds.\e[m", tput[r].count(), roundno[r])
        ;
    for(r=1;r<J;r++) tput[0]+=tput[r], roundno[0]+=roundno[r];
    rone=roundno[0],tone=tput[0];
    printf("\e[10;10H\e[0;102;37mDONE ORIGINAL\n\e[m");
    c=0;
    IAm='A';
    for(r=0;r < J; r++,IAm++) pthread_create(&aa[r], 0, timmr2, (void*)IAm);
    //c=getchar();//to make it so the iterations stop when you enter a key, uncomment this line and comment the next
    plswait(SETTIME);c++;//to make it so the iterations stop after the settime (in seconds), uncomment this line and comment the prev
    //printf("\e[20H\e[K\e[0;31mC:%c, %i r=%i\e[0m\t",c,c,r),
    for(r=0;r<J;r++)
        pthread_join(aa[r],0)
        //, printf("\n\e[1;43mThread took:\t\t%Lf milliseconds for %Lu rounds.\e[m", tput[r].count(), roundno[r])
        ;
    for(r=1;r<J;r++) tput[0]+=tput[r], roundno[0]+=roundno[r];
    printf("\n\e[1;37;44mTook total of:\t%Lf milliseconds for %Lu rounds.\nTime per round= %Lfms\n\e[m", tone.count(), rone,tone.count()/rone);
    printf("\n\e[1;37;45mTook total of:\t%Lf milliseconds for %Lu rounds.\nTime per round= %Lfms\n\e[m", tput[0].count(), roundno[0],tput[0].count()/roundno[0]);

    printf("\n\e[0;30;10%s was faster.\e[m\n",
    ((tput[0].count()/roundno[0])>(tone.count()/rone))? "1mOriginal": "2mTwo Rand");
    printf("\e[0;34m(Faster by %.2Lf times)\n",
    ((tput[0].count()/roundno[0])>(tone.count()/rone))?
        tput[0].count()*rone/(tone.count()*roundno[0]):
        tone.count()*roundno[0]/(tput[0].count()*rone));
    /*restore the old settings*/
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
}

void* timmr1(void* b) {//
    CLK::time_point t1,t2;
    srand(time(0));
    char x=rand()%100+1, y=rand()%30+1,//no real reason for 100 and 30. +1 because ANSI escape sequence \e[a;bH has a minimum 1 for the values of a and b, for the row and column of the cursor.
    m=MIDPO, IAm= (char) (unsigned long)b,
    Iamp8=IAm%8; //to not constantly mod for colour output.
    printf("\e[%i;%iH\e[3%i%c",y,x,Iamp8,IAm);
    while(!c){
        plswait(T);
        printf("\e[%i;%iH ",y,x);
        t1=CLK::now();
        m=rand()%PLACES;
        /*if (MIDPO==m)//particle never stationary
        m+=(1-((rand()&1)<<1))// + or -
            *(1+(LEN-1)*(rand()&1));//±1 or ±LEN i.e. the places immediately left, right, up, or down relative to the middle*/
        y+= m/LEN-R;
        x+= m%LEN-R;
        t2=CLK::now();
        tput[IAm-'A']+=t2-t1;
        if(x<1) x=1;
        if(y<1) y=1;
        printf("\e[%i;%iH\e[3%im%c",y,x,Iamp8,IAm); if('A'==IAm) fflush(stdout);
        roundno[IAm-'A']++;

    }
    return b;
}

void* timmr2(void* b) {//
    CLK::time_point t1,t2;
    srand(time(0));
    char x=rand()%100+1, y=rand()%30+1,//no real reason for 100 and 30. +1 because ANSI escape sequence \e[a;bH has a minimum 1 for the values of a and b, for the row and column of the cursor.
    mx=R,my=R, IAm= (char) (unsigned long)b,
    Iamp8=IAm%8; //to not constantly mod for colour output.
    roundno[IAm-'A']=0;
    tput[IAm-'A']-=tput[IAm-'A'];
    printf("\e[%i;%iH\e[3%i%c",y,x,Iamp8,IAm);
    while(!c){
        plswait(T);
        printf("\e[%i;%iH ",y,x);
        t1=CLK::now();
        mx=rand()%LEN;
        my=rand()%LEN;
        /*if (R==mx && R==my)//particle never stationary
        ((rand()&1)?my:mx)=(1+((rand()&1)<<1));//offset of x or y will be R-1 or R+1
            //i.e. the places immediately left, right, up, or down relative to the middle (R)*/
        y+= my-R;
        x+= mx-R;
        t2=CLK::now();
        tput[IAm-'A']+=t2-t1;
        if(x<1) x=1;
        if(y<1) y=1;
        printf("\e[%i;%iH\e[3%im%c",y,x,Iamp8,IAm); if('A'==IAm) fflush(stdout);
        roundno[IAm-'A']++;
    }
    return b;
}
inline void plswait(long double k){//float seconds value as argument
    struct timespec remaining, request ={static_cast<long>(k),
        static_cast<long>(long((k-(long)k)*1000000000)%1000000000)};
    nanosleep(&request, &remaining);
}