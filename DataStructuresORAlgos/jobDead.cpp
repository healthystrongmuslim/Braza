#include <print>
#include <iostream>
#include <queue>
#include <stdlib.h>
#include <map>
#include <termios.h>
using std::vector, std::priority_queue, std::map, std::print, std::cin;
bool tchanged=0,dchanged=0,clickForIter=0;//if additional arg has non number, per-keypress iteration
struct Job {
  int profit;
  int dline;

  // Constructor
  Job(int p, int t): profit(p),
  dline(t) {}
};
// Comparator to create a max-heap based on profit
struct CompareProfit {
  bool operator()(Job const& j1, Job const& j2) {
    return j1.profit < j2.profit;
  }
};

// Function to build a max-heap from a list of jobs
priority_queue<Job,vector<Job>,CompareProfit> build_max_heap(vector<Job>& jobs)
{
  priority_queue<Job,vector<Job>,CompareProfit> max_heap;
  for (const auto& job: jobs)
    max_heap.push(job);
  return max_heap;
}

vector<Job> randJobArr(int totalJobs,int lastDline=0){
  vector<Job> jobs;
  const int maxProfit=20;
  for(int i=0;i<totalJobs;i++) jobs.emplace_back(Job(rand()%maxProfit+1, rand()%lastDline+1));
  return jobs;
}

//Function for the algorithm I designed
map<int,Job> jobSequencer(vector<Job>& jobs){
  map<int, Job> optimalSeq;
  auto max_heap = build_max_heap(jobs);
  /*/ Print the jobs, sorted by the max-heap
  print("\n\e[102;30m Sorted by profit: \e[m\n");
  while (!max_heap.empty())
  {
    auto [profit, dline] = max_heap.top();
    print("(\e[92m${}\e[m, \e[31m{}\e[m)  ",
    profit, dline);
    max_heap.pop();
  }
  max_heap = build_max_heap(jobs);
  //*/
  int tprofit=0,addedjobs=0;
  print("\n\e[;94m Building sequence \t \e[2;32m(Dropped \e[31mjobs) \e[m Running Profit:");
  //Sequencing/Ordered-Mapping---The algorithm itself
  while (!max_heap.empty())
  {
    auto [profit, dline] = max_heap.top();
    int addAt=0;
    Job Jj(profit, dline);
    for(int i=dline;i && !addAt;i--)
      if(!optimalSeq.count(i)) addAt=i, optimalSeq.emplace(addAt,Jj);
    if(!addAt) print("\e[2;32m(${},\e[31m{})\e[m  ", profit, dline);
    else
    {
      print("\e[;92m${} \n\e[{}4m",tprofit,((addedjobs&1)?3:9));if(clickForIter) getchar();
      //int row=getRowFromResponse(getCursorPosition());
      for(auto [index,j]: optimalSeq) print("\e[{}G{} ",(index-1)*4+2,j.profit);
      tprofit+=Jj.profit; addedjobs++;
      print("\e[m");
    }
    max_heap.pop();
  }
  print("\n\e[104m Built sequence ↑ Total Profit:${} \e[m\n", tprofit);
  return optimalSeq;
}

void manageArgs(int& argc, char** argv, int &totalJobs, int &lastDline)
{
  if(argc>1)
  {
    if(!atoi(argv[1]) or (atoi(argv[1])<0 and atoi(argv[1])!=-1)){ clickForIter=1;return;}//bad arg
    totalJobs=atoi(argv[1]);tchanged=1;
    if(totalJobs==-1) totalJobs=rand()%15+5, lastDline=rand()%10+5,dchanged=1;
    if(argc>2)
    {
      if(!atoi(argv[2]) or (atoi(argv[2])<0 and atoi(argv[2])!=-1)){ clickForIter=1;return;}//bad arg
      lastDline=atoi(argv[2]);dchanged=1;
      if(lastDline==-1) lastDline=rand()%10+5;
      if(argc>3) {clickForIter=1; return;}
    }
  }
}

int main(int argc, char **argv) {
  srand(time(0));
  int totalJobs, lastDline;
  //Total number of jobs available, the last deadline timeslot available
  manageArgs(argc,argv,totalJobs,lastDline);
  if(!tchanged){
    print("Enter jobcount: \e[90m(-1 for random values)\e[m ");
    while (!(cin >> totalJobs) or (totalJobs<1 and totalJobs!=-1))
    {
      cin.clear();
      cin.ignore();
      print("\e[101mAAA\e[m ");
    }
    if(totalJobs==-1) totalJobs=rand()%15+5, lastDline=rand()%10+5,dchanged=1;
  }
  if(!dchanged){
    print("Enter deadline: \e[90m(-1 for random value)\e[m ");
    while (!(cin >> lastDline) or (lastDline<1 and lastDline!=-1))
    {
      cin.clear();
      cin.ignore();
      print("\e[101mAAA\e[m ");
    }
    if(lastDline==-1) lastDline=rand()%10+5;
  }
  auto jobs=randJobArr(totalJobs,lastDline);
  print("\e[91;4mASSUMING DEADLINE ITSELF IS INCLUDED\e[;90;2;3m \
(\"Due on 30th\" includes the 30th day?) \n\e[m Original array (total jobs {}, last deadline {}): ",totalJobs, lastDline);
  for(auto [profit, dline]: jobs) print("(\e[32m${}, \e[31m{}\e[m)  ", profit, dline);

  static struct termios oldt, newt;
  tcgetattr(0, &oldt);
  newt = oldt; newt.c_lflag &= ~264;//ICANON (0x100) | ECHO (8) // doesnt wait for newline/Enter, doesn't display input keys
  tcsetattr(0, 0, &newt);
  auto optimalSeq=jobSequencer(jobs);
  print("\n Profitmaxxing sequence: \e[m\n ");
  for(auto [index,j]: optimalSeq) print("\e[96m{}\e[m:(\e[92m${}\e[m,\e[31m{}\e[m)  ",index, j.profit,j.dline);
  tcsetattr(0, 0, &oldt);
  print("\n");
  return 0;
}
