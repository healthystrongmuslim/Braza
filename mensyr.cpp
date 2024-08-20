/*
for stats/prob graphs
Braza Musaab
*/
#include <iostream>
#include <stdio.h>
#define F 100
#define S 1000
using namespace std;
const char prange[10]={2,3,4,4,5,5,5,6,6,7};
//period range:
// 0.2 chance of 4 days, 0.3 chance of 5 days, 0.2 of 6,
// and 0.1 of 2, 3, and 7
struct laps{short lapno[F+1]={0};};
laps arr[S];
void* simul(void* B){
    srand((unsigned long)pthread_self());
    int b= (unsigned long)B;
    short days[F][3],overlaps=0,done=1;
    for(char i=0;i<F;i++){
        //done*= i+2;
        days[i][2]=21+rand()%15;//duration of cycle {between 21 to 35 days, equal chance}
        days[i][0]=rand()%days[i][2];//first period start day out of year
        days[i][1]=days[i][0]+prange[rand()%10]-1;//first period end day
        
        /*printing period start(green) day and end(red) day on 31 day-month
        printf("%i here.\t%i:start %i,\tend %i\t\e[103m \e[31C \e[32D\e[%iC\e[4%im \e[D\e[%iC\e[4%im \e[0m \n",
        b,i,days[i][0]+1,days[i][1]+1,
        days[i][flip[i]]+1,2-flip[i],//*bar print
        days[i][!flip[i]]-days[i][flip[i]],1+flip[i]);
        //*/
    }
    int i=0;
    for(;i<365*2 //&& done!=1
    ;i++){
        char todayon=0;//how many on period today/on ith-day
        bool lol=0;//lol if current day is period for jth woman
        for(char j=0;j<F;j++) todayon+=
        lol=(i>=days[j][0] and i<=days[j][1]),
        ((i==days[j][1])? days[j][0] +=days[j][2],days[j][1] +=days[j][2] : 0 )
        //if the current day is j-woman's period end day, add j's cycle-length for next period
        ;
        arr[b].lapno[todayon]++;
        if(todayon>1)++overlaps;//printf("%i here. %ith overlap on day %i\n",b,,i+1);
    }
    //printf("%i here. Os=%i, i=%i\n",b,overlaps,i);
    pthread_exit((void *)(unsigned long)overlaps);
}
int main(){
    pthread_t aa[S];unsigned long marr[F+1]={0};
    void* xx=0;unsigned long tot=0,most=0; 
    for(int i=0;i<S;i++) pthread_create(&aa[i], 0, simul, (void*)(unsigned long)i);
    for(int i=0;i<S;i++) {
        pthread_join(aa[i],&xx);
        tot+=(unsigned long)xx;
        if(most<((unsigned long)xx)) most=(unsigned long)xx;
        for (char j = 0; j < F+1; marr[j]+=arr[i].lapno[j],j++)
        ;
    }

    printf("||Total overlaps (out of %i days, with %i women) =%i,\nAvg overlaps per 2 years=%.1f, most=%i\t"
    ,2*S*365,F,(int)tot, ((int)tot)/float(S), (int)most);
    float perc=100*((int)tot)/(2*365.0*S);
    printf("Average overlap rate: %.2f%%\nLapping stats:\n",perc);
    perc=0;//now being used for E(x)
    int csum=0;//cumulative sum
    for (char j = 0; csum!=2*S*365;
    perc+=(j*marr[j])/(2*S*365.0),j++){
        printf("\e[0m%i:\t\e[10%i;30m",//
        j,j%8);
    for(int i=0;i<(100*marr[j])/(2*S*365);printf(" "),i++);
    printf("\e[0;90m%i  %.1f%%\n",marr[j],(100.0*marr[j])/(2*S*365.0));csum+=marr[j];
    }
    printf("\e[102;97mE(x)=%.2f\e[0m (Expected number of women (out of any %i) menstruating on any given day) ||\n",perc,F);
}
