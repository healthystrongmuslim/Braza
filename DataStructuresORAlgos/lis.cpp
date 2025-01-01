#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define LEN 20
int main()
{
  srand(time(0));
  int arr[LEN], nextInd[LEN], lis[LEN]={0}, longestInd=0;

  for(int i=0;i<LEN;printf("%i ", arr[i++]))
    arr[i]=rand()%100, nextInd[i]=-1;
  //INITIALISING: random number arr, -1 nextInd, 0 lis
  printf("\n\e[44m Longest chain: \e[0;34m");
  for(int i=LEN;i--;(lis[i]>=lis[longestInd])? longestInd=i:i)
    for(int j=i+1;j<LEN;j++)
      if(arr[i]<arr[j] && lis[i]<lis[j]+1) nextInd[i]=j, lis[i]=lis[j]+1;
  for(int i=longestInd;i<LEN && i>=0;printf("%i ",arr[i]), i=nextInd[i]);
  printf("\e[m\n");
}
