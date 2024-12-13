#include<print> //use flag --std=c++23 while compiling. Or replace all the print with cout…
#include<random>
#include<unordered_map>
#define HILIMIT 40001
#define LOLIMIT 100
#define GRAPHILE "./graph.txt"
#define INFFILE "./infs.txt"
#define LABLS "./labl.txt"
using std::print,std::stoi,std::unordered_map,std::vector,std::pair;
std::mt19937 gen32;
FILE *tonc,*tinf,*labl;
union randName{unsigned int l;char c[4];};//char array overlayed on int
void nameSet(randName& AA)//randomise 3 letters
{AA.l=gen32(); for(char i=0;i<3;i++) AA.c[i]='A'+32*((unsigned char)(AA.c[i])&1)+(unsigned char)(AA.c[i])%26;AA.c[3]=0;}
struct pair_hash
{
  template <class T1, class T2>
  std::size_t operator()(const std::pair<T1, T2> &p) const{return std::hash<T1>{}(p.first) ^ std::hash<T2>{}(p.second);}
};
int main(int argc, char** argv){
  tonc = fopen(GRAPHILE, "w");
  tinf = fopen(INFFILE, "w");
  labl = fopen(LABLS, "w");
  int givenArgs[2] = {0};
  print("\e[m");
  if (3 != argc) print("Enter only 2 numbers after object filename (Users/Nodes and connections/Edges)\n"), exit(1);
  try{for (int i = 1; i < argc; i++) { givenArgs[i-1] = stoi(argv[i]); if(!(givenArgs[i-1] > LOLIMIT and givenArgs[i-1] < HILIMIT)) exit(3);}}
  catch (std::invalid_argument &e){print("Invalid arg\n");exit(2);}
  catch (std::out_of_range &e){print("Out of range\n");exit(4);}
  int userC=givenArgs[0], edgeC=givenArgs[1];
  if(edgeC*2>= userC *(userC-1)) print("Really{}…\n",((edgeC*2)==userC*(userC-1))?" clever":""),exit(100);//Ending
  int onePcentOfUsers=userC/100;
  gen32.seed(time(0));
  unordered_map<unsigned int, unsigned char> userEdgeC;
  unordered_map<pair<unsigned int, unsigned int>, unsigned char,pair_hash> graph;
  
  for(int i=0,clashes=0,overElven=0; i<edgeC; i++){
    int src=gen32()%userC,dest=gen32()%userC;
    while(src==dest or graph.contains({src,dest}) or graph.contains({dest,src}))
      (userEdgeC[src]>onePcentOfUsers)? (print("\e[41mOVER1%??{}\e[m ",++overElven),src=gen32()%userC):
        (print("bruh wow{} ",++clashes),dest=gen32()%userC);
    unsigned char weight=gen32()%50+1;
    graph[{src,dest}]=weight;
    userEdgeC[src]++,userEdgeC[dest]++;
  }
  randName AA;
  for(auto &[edge,wayt] : graph) print(tonc,"{:5} {:5} {:5}\n",edge.first,edge.second, wayt);
  for(int i=0;i<userC;i++){ nameSet(AA), print(tinf,"{} {}\n",i,AA.l%100+1), print(labl, "{} {}\n",i,AA.c);}
}
