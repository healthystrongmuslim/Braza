#include<print> //use flag --std=c++23 while compiling. Or replace all the print with cout…
#include<random>
#include<unordered_map>
#define NHILIMIT 40001
#define EHILIMIT 120001
#define LOLIMIT 9999
#define GRAPHILE "./graph.txt"
#define INFFILE "./infs.txt"
#define LABLS "./labl.txt"
using std::print,std::stoi,std::unordered_map,std::vector,std::pair;
std::mt19937 gen32;
FILE *tonc,*tinf,*labl;
union randName{unsigned int l;char c[4];};//char array overlayed on int
inline int mod(int a,int b){return (a%b+b)%b;}
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
  if (3 != argc) print("Enter only 2 numbers (nodeCount and edgeCount) after object filename (Users/Nodes and Connections/Edges)\
  \nnodeCount should be square (pls), edgeCount should be less than nodeCount(nodeCount-1)/2"), exit(1);
  try
  {
  givenArgs[0] = stoi(argv[1]); if(!(givenArgs[0] > LOLIMIT and givenArgs[0] < NHILIMIT)) exit(3);
  givenArgs[1] = stoi(argv[2]); if(!(givenArgs[1] > LOLIMIT and givenArgs[1] < EHILIMIT)) exit(3);
  }
  catch (std::invalid_argument &e){print("Invalid arg\n");exit(2);}
  catch (std::out_of_range &e){print("Out of range\n");exit(4);}
  int userC=givenArgs[0], edgeC=givenArgs[1];
  if(edgeC*2>= userC *(userC-1)) print("Really{}…\n",((edgeC*2)==userC*(userC-1))?" clever":""),exit(100);//Ending
  int onePcentOfUsers=userC/100, dimension=sqrt(userC),
  nayborSqr=sqrt(onePcentOfUsers),naySqrHalf=nayborSqr/2;
  if(userC!=dimension*dimension) print("{} isn't a square nodecount, try {} or {}\n",userC,(dimension+1)*(1+dimension),dimension*dimension),exit(5);
  gen32.seed(time(0));
  unordered_map<unsigned int, unsigned char> userEdgeC;
  unordered_map<pair<unsigned int, unsigned int>, unsigned char,pair_hash> graph;
  
  for(int i=0,clashes=0,overOnePcent=0,src,srcx,srcy,dest,destx,desty; i<edgeC; i++){//single declaration
    src = gen32() % userC;
    srcx = src % dimension, srcy = src / dimension;
    dest = gen32() % onePcentOfUsers; // destination in close proximity (visually) to source
    destx = dest % nayborSqr - naySqrHalf, desty = dest / nayborSqr - naySqrHalf;
    if (!(destx || desty)) (dest & 4)? // funky way to change offset from 0 (using smallest 3 bits of dest), forcing src!=dest
      destx=(dest&1)?-1:1, desty=(dest&2)?-1:1:
      ((dest&2)?destx:desty)=(dest&1)?-1:1;
    dest = mod(srcx + destx, dimension) + dimension * mod(srcy + desty, dimension);
    while (graph.contains({src, dest}) or graph.contains({dest, src}))
    {
      if(userEdgeC[src] > onePcentOfUsers)
        {
        print("\e[41mOVER1%??{}\e[m ", ++overOnePcent);
        src=gen32()%userC,srcx=src%dimension,srcy=src/dimension;
        dest=mod(srcx+destx,dimension)+ dimension*mod(srcy+desty,dimension);
        continue;
        }
      print("huh{} ", ++clashes);
      dest = gen32() % onePcentOfUsers;
      destx = dest % nayborSqr - naySqrHalf, desty = dest / nayborSqr - naySqrHalf;
      if(!(destx || desty)) (dest&4)?
        destx=(dest&1)?-1:1, desty=(dest&2)?-1:1:
        ((dest&2)?destx:desty)=(dest&1)?-1:1;
      dest = mod(srcx+destx,dimension)+ dimension*mod(srcy+desty,dimension);
    }
    unsigned char weight=gen32()%50+1;
    graph[{src,dest}]=weight;
    userEdgeC[src]++,userEdgeC[dest]++;
  }

  randName AA;
  for(auto &[edge,wayt] : graph) print(tonc,"{:5} {:5} {:5}\n",edge.first,edge.second, wayt);

  for(int i=0;i<userC;i++){ nameSet(AA), print(tinf,"{} {}\n",i,AA.l%100+1), print(labl, "{} {}\n",i,AA.c);}
}
