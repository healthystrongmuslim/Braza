#include <stdio.h>
#include <queue>
#include <stdlib.h>
#include <map>
#include <termios.h>
using std::vector, std::priority_queue, std::map;
struct Job {
  int profit;
  int dline;

  // Constructor
  Job(int p, int t): profit(p),
  dline(t) {}
  Job(const Job& j): profit(j.profit),
  dline(j.dline) {}
};
// Comparator to create a max-heap based on profit
struct CompareProfit {
  bool operator()(Job const& j1, Job const& j2) {
    return j1.profit < j2.profit; // Max-heap based on profit
  }
};

char* getCursorPosition()
{
  // Send the escape sequence to get the cursor position
  printf("\e[6n");
  static struct termios oldt, newt;
  tcgetattr(0, &oldt);
  newt = oldt; newt.c_lflag &= ~10;//ICANON (8) | ECHO =10
  tcsetattr(0, 0, &newt);
  static char response[20];
  int i = 0;
  char ch;
  printf("\e[35m");
  // Read chars from stdin until 'R'
  while ((ch=getchar()) != 'R' && i<20) response[i++]=ch;
  tcsetattr(0, 0, &oldt);
  printf("\e[m");
  response[i] = 0;  // Null-terminate the string
  return response;
}
int getRowFromResponse(const char* response)
{
  // The response format will be "\033[row;colR"
  // So we need to extract the row value between the first '(' and ';'
  const char* semicolon = strchr(response, ';');
  if (semicolon != NULL)
  {
    char rowStr[20];
    int length = semicolon - (response + 2);  // Skip "\e"
    strncpy(rowStr, response + 2, length);
    rowStr[length] = 0;  // Null-terminate the row string
    return atoi(rowStr);  // Convert to integer and return
  }
  return -1;  // In case of an error
}
// Function to build a max-heap from a list of jobs
priority_queue<Job,vector<Job>,CompareProfit> build_max_heap(vector<Job>& jobs)
{
  priority_queue<Job,vector<Job>,CompareProfit> max_heap;
  for (const auto& job: jobs) {
    max_heap.push(job);
  }
  return max_heap;
}

vector<Job> randJobArr(int totalJobs,int lastDline=0){
  vector<Job> jobs;
  const int maxProfit=20;
  if(lastDline<1) lastDline=totalJobs/1.6;//truly magical number
  if(lastDline<1 || totalJobs<1) return {};
  for(int i=0;i<totalJobs;i++) jobs.emplace_back(Job(rand()%maxProfit+1, rand()%lastDline+1));
  return jobs;
}

//Function for the algorithm I designed
map<int,Job> jobSequencer(vector<Job>& jobs){
  map<int, Job> optimalSeq;
  auto max_heap = build_max_heap(jobs);
  /*/ Print the jobs, sorted by the max-heap
  printf("\n\e[102;30m Sorted by profit: \e[m\n");
  while (!max_heap.empty())
  {
    auto [profit, dline] = max_heap.top();
    printf("(\e[92m$%i\e[m, \e[31m%i\e[m)  ",
    profit, dline);
    max_heap.pop();
  }
  max_heap = build_max_heap(jobs);
  //*/
  int tprofit=0,addedjobs=0;
  printf("\n\e[0;94m Building sequence \t\e[0;32m (Dropped \e[31mjobs) \e[m");
  //Sequencing/Ordered-Mapping---The algorithm itself
  while (!max_heap.empty())
  {
    auto [profit, dline] = max_heap.top();
    int addAt=0;
    Job Jj(profit, dline);
    for(int i=dline;i && !addAt;i--)
      if(!optimalSeq.count(i)) addAt=i, optimalSeq.emplace(addAt,Jj);
    if(!addAt) printf("(\e[32m$%i\e[m,\e[31m%i\e[m)  ", profit, dline);
    else
    {
      printf(" Profit:\e[0;32m$%i \n",tprofit);
      int row=getRowFromResponse(getCursorPosition());
      for(auto [index,j]: optimalSeq)
      {
        printf("\e[%i4m\e[%i;%iH%i ",((addedjobs&1)?3:9),
        row,(index-1)*4+2,j.profit);
      }
      tprofit+=Jj.profit; addedjobs++;
      printf("\e[m");
    }
    max_heap.pop();
  }
  printf("\n\e[104m Built sequence ↑ Total Profit:$%i \e[m\n", tprofit);
  return optimalSeq;
}

int main() {
  srand(time(0));
  int totalJobs=20, lastDline=0;
  //Total number of jobs available, the last deadline timeslot available
  //0 deadline changes to some sensible deadline
  auto jobs=randJobArr(totalJobs,lastDline);
  printf("\e[91;4mASSUMING DEADLINE ITSELF IS INCLUDED \e[;90;2;3m(\"Due on 30th\" includes the 30th day?) \n\e[;101;30m Original array (total jobs %i, last deadline %i): \e[m",totalJobs, lastDline);
  for(auto [profit, dline]: jobs)
  {
    //if(lastDline<dline) lastDline=dline;
    printf("(\e[32m$%i\e[m, \e[31m%i\e[m)  ",
      profit, dline);
  }
  
  auto optimalSeq=jobSequencer(jobs);
  printf("\n\e[104;30m Profitmaxxing sequence: \e[m\n ");
  for(auto [index,j]: optimalSeq)
  {
    printf("\e[96m%i\e[m:(\e[92m$%i\e[m,\e[31m%i\e[m)  ",index, j.profit, j.dline);
  }
  printf("\n");
  return 0;
}