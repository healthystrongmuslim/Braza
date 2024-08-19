/*

*/
#include <chrono>
#include <pthread.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#define CLK std::chrono::high_resolution_clock
using namespace std;
CLK::time_point t1 = CLK::now(), t2 = t1;
std::chrono::duration < long double, milli > tput = t1-t1;
void* timmr(void*), plswait(long double);
char c=0;
bool repeat=0;
int r = 0; long tcounts[20]={0};
FILE *train;
int main() {
    train = fopen("training.txt","w");
    time_t result=time(nullptr);
    fprintf(train, "\e[2J\e[1;1H\e[11H\e[mStart time: %s", asctime(localtime(&result)));
    pthread_t aa;
    printf("\e[2J\e[1;1H\e[m");
    printf("\e[11HStart time: %s\n", asctime(localtime(&result)));
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
    ////
    for(; r < 20; r++) pthread_create(&aa, 0, timmr, 0),
    c=getchar(),
    //printf("\e[20H\e[K\e[0;31mC:%c, %i r=%i\e[0m\t",c,c,r),
    pthread_join(aa,0),
    repeat=(r>0)*(tcounts[r]==tcounts[r-1]),
    tcounts[r]=(!repeat)*tcounts[r],
    r-=repeat;//for when bad input
    result=time(nullptr);
    printf("\e[12H\e[0;1mEnd time: %s\n\e[0m(now write the bash line below)\n\t\e[33mcat training.txt || printf\n\e[m", asctime(localtime(&result)));
    fprintf(train, "\e[12H\e[0;1mEnd time: %s\n", asctime(localtime(&result))); /*restore the old settings*/
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
}

void* timmr(void* b) {
    t1 = CLK::now(); int R=r,ss=0;
    printf("\e[14H\e[0m\e[J");
    while(!c)
    {
        t2 = CLK::now();
        tput = t2-t1;
        printf("\e[%i;%iH\e[0;102;%imDone %i reps:\e[1;35m%.1Lf",
        R%10+1, (R>9)*23, 34+(R&1)*56, R, tput.count()/1000);
        fflush(stdout);//,progress bar
        if(int(tput.count()/1000)==ss){
        printf("\e[%i;%iH\e[30;4%im%i\e[0m",
        14+ss/30,ss%30+1,2+(ss>29)-2*(ss>59),ss%10);
        ss++;
        }
        plswait(0.05);
    }
    
    c=0;tcounts[R]=tput.count();
    if(R>0 && tcounts[R]==tcounts[R-1]) return (void*) (long) tput.count();
    fprintf(train, "\e[%i;%iH\e[0;102;%imDone %i reps:\e[1;35m%im %.1Lfs",
    R%10 + 1, (R > 9)*23, 34 + (R & 1)*56, R, int(tput.count()/1000)/60, tput.count()/1000 -60.0*(int(tput.count()/1000)/60));
    fflush(train);
    return (void*) (long) tput.count();
}
inline void plswait(long double k){//float seconds value as argument
    struct timespec remaining, request ={static_cast<long>(k),
        static_cast<long>(long((k-(long)k)*1000000000)%1000000000)};
    nanosleep(&request, &remaining);
}
