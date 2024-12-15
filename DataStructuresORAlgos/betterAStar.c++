// written in C++23
// add the flag --std=c++23 when compiling
#include <print>
#include <termios.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#define NHILIMIT 40001
#define LOLIMIT 100
#define p1file "./graph.txt"
#define p2file "./infs.txt"
#define lablfile "./labl.txt"
#define MAX std::numeric_limits<int>::max()
using std::print, std::ifstream, std::vector,
  std::cin, std::unordered_map, std::priority_queue,
  std::string, std::tuple, std::getline,std::to_string,
  std::istringstream, std::reverse, std::greater;
FILE* results;
char charInp = 0;
void plswait(long double k)
{
  struct timespec remaining, request = {static_cast<long>(k),
                                        static_cast<long>(long((k - (long)k) * 1000000000) % 1000000000)};
  nanosleep(&request, &remaining);
}
static struct termios oldt, newt;
void termiEcho(bool bb = 0)
{
  newt.c_lflag &= ~ECHO;
  if (bb)
    newt.c_lflag &= ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}
void termiOneKey(bool bb = 0)
{
  newt.c_lflag &= ~ICANON;
  if (bb)
    newt.c_lflag &= ICANON;
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}
void termiInit()
{
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
}
void termiReset()
{
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

class Node
{
  int weight = -1, connNode = -1;
  Node *prev = 0, *next = 0;

public:
  Node(int n, int connID, Node *pre = 0, Node *nex = 0):
    weight(n), connNode(connID), prev(pre), next(nex) {}
  friend class SortedAdjList;
  friend class Graph;
};

class SortedAdjList
{
  Node *head = 0, *tail = 0;

public:
  int size = 0, chainNext = -1;
  unsigned char influ = 0, Len = 0;
  string name = "";

  static int listscount;
  SortedAdjList() { tail = head = new Node(0, listscount++); } // self connection has no weight
  bool insert(int weight, int id)                              // insert in order of weight (lowes weight to largest weight)
  {
    if (weight < 1 or id < 0 or getConn(id))
      return 0;
    Node *tmp = head;
    while (tmp->next and tmp->next->weight < weight)
      tmp = tmp->next;
    Node *nex = tmp->next;
    tmp->next = new Node(weight, id, tmp, nex);
    if (nex)
      nex->prev = tmp->next;
    else
      tail = tmp->next; // adding at tail
    size++;
    return 1;
  }
  void deleteNode(Node *tmp)
  {
    if (!tmp)
      return;
    Node *pre = tmp->prev, *nex = tmp->next;
    if (pre)
      pre->next = nex;
    if (nex)
      nex->prev = pre;
    if (tmp == tail)
      tail = pre;
    if (tmp == head)
      head = nex;
    delete tmp;
    size--;
  }
  bool remove(int weight = 0)
  {
    if (weight < 0 or weight > tail->weight)
      return 0; // weight not in range
    Node *tmp = head;
    while (tmp->weight != weight and tmp->next and tmp->next->weight <= weight) // early stop based on sortedness
      tmp = tmp->next;
    if (tmp->weight == weight)
    {
      deleteNode(tmp);
      return 1;
    }
    return 0;
  }
  Node *getConn(int id)
  {
    if (id < 0)
      return 0; // id not in range
    Node *tmp = head;
    while (tmp and id != tmp->connNode)
      tmp = tmp->next;
    return tmp;
  }
  bool removeConn(int id)
  {
    if (id < 0 or id == head->connNode)
      return 0; // weight not in range or deleting source
    Node *tmp = getConn(id);
    deleteNode(tmp);
    return tmp; // if connection existed, it is removed and returns True.
  }
  ~SortedAdjList()
  {
    Node *tmp = head, *nex;
    while (size)
      nex = tmp->next, deleteNode(tmp), tmp = nex;
    listscount--;
  }
  void display()
  {
    print("\e[A\e[2K\e[4{}m{} ({})\tInfluence: {}\tChain Length:{}\nDesti:\t", head->connNode % 7 + 1, name, head->connNode, influ, Len);
    Node *tmp = head;
    for (int i = 0; i < size; i++)
      tmp = tmp->next, print("{:6}", tmp->connNode);
    print("\nPower:\t"), tmp = head;
    for (int i = 0; i < size; i++)
      tmp = tmp->next, print("{:6}", tmp->weight);
    print("\n");
  }

  friend class Graph;
};
int SortedAdjList::listscount = 0;

class Graph
{
  SortedAdjList *lists;
  int dimensions,//for printing board
  destx,desty;//heuristic
public:
  int nofEdges, nofNodes;
  bool *inShortestPath = 0;
  Graph(int nodes = 1)
  {
    if (nodes < 1 or nodes > NHILIMIT)
      print("\e[101mOh bhai kya hua? control.\n"), exit(2);
    nofNodes = nodes;
    lists = new SortedAdjList[nofNodes];
    nofEdges = 0;
    dimensions=sqrt(nofNodes);
    destx=-1,desty=-1;
  }
  bool insertEdge(int src, int dest, int weight)
  {
    if (src < 0 or dest < 0 or weight < 0 or src >= nofNodes or dest >= nofNodes)
    {
      print("\e[42mbad insert {} {} {}\n", src, dest, weight);
      return 0;
    }
    bool success = lists[src].insert(weight, dest) and lists[dest].insert(weight, src); // undirected
    nofEdges += success;
    return success;
  }
  int heuheu(int id,int dest)
  {
    if(destx==-1) destx=dest%dimensions, desty=dest/dimensions;
    int idx=id%dimensions-destx,idy=id/dimensions-desty;
    return sqrt(idx*idx+idy*idy); // Subtract 1 to exclude self-connection
  }
  void InspectNodes()
  {
    int id;
    do
    {
      print("\e[mEnter Node ID:");
      while (!(cin >> id))
      {
        cin.clear();
        cin.ignore();
        print("\e[101mAAA\e[m ");
      }
      if (validId(id))
        lists[id].display();
    } while (validId(id));
  }
  void setName(int id, string naam)
  {
    lists[id].name = naam;
  }
  void setInf(int id, unsigned char infl)
  {
    lists[id].influ = infl;
  }
  bool validId(int id)
  {
    return id > -1 and id < nofNodes;
  }
  ~Graph()
  {
    delete[] lists; // print("Number of lists:{}\n",SortedAdjList::listscount);
  }

//Chain
  int checkChain(int id, bool *checked)
  {
    //if(checked[id])
    Node *tmp = lists[id].head;
    int C=-2;
    unsigned char Influ = lists[id].influ, L=0; // to-be longest
    while ((tmp = tmp->next))
    {
      int dest = tmp->connNode; // curr neber id
      if (Influ >= lists[dest].influ)
        continue;
      if (!checked[dest])
        checkChain(dest, checked);
      if (L < lists[dest].Len + 1)
        L = lists[dest].Len+1, C=dest; // tentative updated
    }
    lists[id].Len = L;
    lists[id].chainNext = C;
    checked[id] = 1;
    return 1;
  }
  int setChains()
  {
    bool *checked=new bool[nofNodes];
    for(int i=0;i<nofNodes;checked[i++]=0);
    int idLongest = 0;
    for (int i = 0; i < nofNodes; i++)
    {
      if (!checked[i])
        checkChain(i, checked);
      if (lists[i].Len > lists[idLongest].Len)
        idLongest = i; // keep id of longest yet
    }
    delete[] checked;
    return idLongest;
    /*Dynamicking how?
    When a node is newly accessed, the function recursively runs on that,
    and its own longest Chain's length (L) is saved.
    If it's been traversed, and is now being viewed by a new node,
    and the viewer node's tentative best length is just the L+1
    (the viewed's length + viewer's own append);
    */
  }
  vector<tuple<int, unsigned char>> showChain(int id = -111)
  {
    if (-111 == id)
      id = setChains();
    else if(-1 == lists[id].chainNext)
      setChains();
    vector<tuple<int, unsigned char>> chain;
    int curr = id;
    for (; curr > -1; curr = lists[curr].chainNext) chain.emplace_back(curr, lists[curr].influ);
    return chain;
  }

  vector<int> traceAStarPath(int src, int dest, unordered_map<int, int> &cameFrom,int &cost) // Reconstruct the path
  {
    vector<int> path;
    if (inShortestPath)
      delete[] inShortestPath;
    inShortestPath = new bool[nofNodes];
    for (int i = 0; i < nofNodes; inShortestPath[i++] = 0)
      ;
    cost = 0;
    int curr = dest;
    while (cameFrom.find(curr) != cameFrom.end())
    {
      inShortestPath[curr] = 1;
      Node *tmp = lists[curr].head->next;
      while (tmp and tmp->connNode != cameFrom[curr])
        tmp = tmp->next;
      cost += tmp->weight;
      path.push_back(curr);
      curr = cameFrom[curr];
    }
    path.push_back(src);
    inShortestPath[src] = inShortestPath[dest]=1;
    reverse(path.begin(), path.end());
    return path;
  }
  void printBoard(bool* processed=0, int src = -1, int dest = -1,int curr=-1) // 405x205 for 40000 nodes; [2√N+5]x[√N+5] for N nodes
  {
    string inPathNodes,color;
    //print("\e[H\e[46mBoard:");
    for (int i=0;i<nofNodes;i++)
    {
      if (!processed[i]) continue;
      print("\e[{};{}H\e[0;", i / dimensions + 1, 2 * (i % dimensions) + 1);
      if (src == i)
          color = "102"; // green
      else if (dest == i)
          color = "101";
      else if(curr==i)
          color = "106";
      if (inShortestPath and inShortestPath[i])
      {
        inPathNodes+="\e[" + (to_string(i/dimensions +1))+ ";" + (to_string(2*(i%dimensions)+1))+ "H\e[0;";
        color = "103"; // internal-yellow
        if (src == i)
          color = "102"; // green
        if (dest == i)
          color = "101"; // red
        inPathNodes += color + "m  " + to_string(i);// print node ID next to colored cell
      }
      print("{}m  ",color);
      color="100"; 
    }
    print("{}\e[m\e[{}H\n", inPathNodes,dimensions+1);
  }
  vector<int> astra(int src, int dest) // means star lol
  {
    if (src == dest)
      return {src};
    if (!validId(src) or !validId(dest))
      return {};
    // Priority queue to store (fScore, nodeID) tuples
    priority_queue<tuple<int, int>, vector<tuple<int, int>>, greater<>> openSet;
    openSet.emplace(heuheu(src,dest), src);
    // Maps to store the g_score and f_score of nodes
    unordered_map<int, int> gs, fs;
    bool* processed=new bool[nofNodes];
    for (int i = 0; i < nofNodes; processed[i++]=0)
      gs[i] = fs[i] = MAX;
    gs[src] = 0; // g[n] = as yet lowest known distance from src to n
    fs[src] = heuheu(src,dest) + gs[src]; // f[n] = as yet best guess distance from src to dest thru n
    // Map to store the path
    unordered_map<int, int> cameFrom;
    int count = 0, nodesProcessed = 0, prevx=-1,prevy=-1;
    print("\e[2J\e[H\e[46mBoard:");
    while (!openSet.empty())
    {
      count++;
      auto [currentfScore, curr] = openSet.top();
      if (!processed[curr])  nodesProcessed += processed[curr] = 1;
      // print("\e[33m{}\e[m",openSet.top());
      if (curr == dest)//You have arrived at your destination. مع سلاما
      {
        int cost;
        auto tracedPath = traceAStarPath(src, dest, cameFrom,cost);
        printBoard(processed,src,dest);
        print("\e[101mCOUNT={}\tNodes Visited:{}\e[m\n", count, nodesProcessed);
        print("\e[0;91mCOST={}\e[m\n", cost);
        delete[] processed;
        desty=destx=-1;
        if (inShortestPath)
        {
          delete[] inShortestPath;
          inShortestPath = 0;
        }
        return tracedPath;
      }
      openSet.pop();
      Node *nodebor = lists[curr].head->next;
      while (nodebor)
      {
        int nayborID = nodebor->connNode;
        int topG = gs[curr] + nodebor->weight;
        if (topG < gs[nayborID])
        {
          cameFrom[nayborID] = curr;
          gs[nayborID] = topG;
          fs[nayborID] = topG + heuheu(nayborID,dest);
          openSet.emplace(fs[nayborID], nayborID);
        }
        // print("\n\e[m");
        nodebor = nodebor->next;
      }
      //if(count%500==0)printBoard(processed,src,dest,curr), 
      switch(count){
        case 1: print("\e[{};{}H\e[102m  ",src/dimensions+1,(src%dimensions)*2+1);break;//curr=src
        case 2: prevx=(curr%dimensions)*2+1,prevy=curr/dimensions+1;//first neighbor node
          print("\e[{};{}H\e[106m  ",prevy,prevx);break;
        default: print("\e[{};{}H\e[100m  ",prevy,prevx);//previous grey
          prevx=(curr%dimensions)*2+1,prevy=curr/dimensions+1;
          print("\e[{};{}H\e[106m  ",prevy,prevx);break;//current blue
      }
      plswait(0.001);fflush(stdout);
    }
    //print("\e[101mCOUNT={}\tNodes Visited:{}\e[m\n", count, nodesProcessed);
    delete[]  processed;desty=destx=-1;
    return {}; // Return an empty path if no path found
  }

};

void p1fileDataReader(char *, Graph &),
    p2fileDataReader(char *, Graph &),
    readIdNames(char *, Graph &);

int main(int argc, char** argv){
  int givenArg =0;
  print("\e[m");
  if (2 != argc) print("Enter only 1 number after object filename (Users/Nodes and connections/Edges)\n"), exit(1);
  try{givenArg = std::stoi(argv[1]); if(!(givenArg> LOLIMIT and givenArg< NHILIMIT)) throw (3);}
  catch (std::invalid_argument &e){print("Invalid arg\n");exit(2);}
  catch (std::out_of_range &e){print("Out of range of int\n");exit(4);}
  catch (int err){print("Out of limit\n");exit(err);}
  int NODECOUNT=givenArg;
  termiInit();
  Graph projgraph(NODECOUNT);
  // PART 1
  p1fileDataReader(p1file, projgraph);
  print("\e[42mNumber of edges inserted: {}\e[m\n", projgraph.nofEdges);
  p2fileDataReader(p2file, projgraph);
  readIdNames(lablfile, projgraph);

  //*Enter-less key input, no echo settings
  termiEcho();termiOneKey();
  print("{0}V{1}iew node, or {0}C{1}ontinue program? (Any other key to exit)\t","\e[0;31m","\e[m");
  charInp=getchar();
  termiReset();print("\n");
  switch(charInp){
    case 'v': case 'V': projgraph.InspectNodes();
    case 'C': case 'c': print("\e[0;43m\nContinuing!\e[m\n"); break;
    default: exit(0);
  }//*/
  // P1.1 done
  int src, dest;
  src = 523, dest = 5229;
  //*
  print("\e[mEnter start and end for A* path:\t");
  while (!(cin >> src>>dest))
      {
        cin.clear(); cin.ignore();
        print("\e[101mAAA\e[m ");
      }
      getchar();//take blank*/
  print("Going from {} to {}!\n", src, dest);
  vector<int> shortestpath = projgraph.astra(src, dest);
  print("Shortest path via A★:{}\n", shortestpath);
  termiEcho(); termiOneKey();
  print("{0}V{1}iew node, or {0}C{1}ontinue program? (Any other key to exit)\t", "\e[0;31m", "\e[m");
  charInp = getchar();
  termiReset();
  print("\n");
  switch (charInp)
  {
  case 'v': case 'V': projgraph.InspectNodes();
  case 'C': case 'c': print("\e[0;43m\nContinuing!\e[m\n"); break;
  default: exit(0);
  }

  //PART 2
  print("Longest chain of influence \e[90m(from given start node, each\n\\tsuccessive \
  will have greater Influence score than previous nodes)\e[m:\n{} \n",projgraph.showChain());
  termiEcho();termiOneKey();
  print("{0}V{1}iew nodes? (Any other key to exit)\t","\e[0;31m","\e[m");
  charInp=getchar();
  termiReset();print("\n");
  switch(charInp)
  {
    case 'v': case 'V': projgraph.InspectNodes();
    default: print("\e[0;43m\nExitinuing!\e[m\n"); break;
  }//*/
  termiReset();
}

void readIdNames(char *filepath, Graph &proj)
{
  ifstream srcFile(filepath);
  if (!srcFile)
    print("\e[41mError opening labels file \"{}\"\n",filepath), exit(1);
  string line;
  while (getline(srcFile, line))
  {
    istringstream iss(line);
    int src;
    string nam;
    if (!(iss >> src))
    {
      print("Error reading src from line:{}\n", line);
      continue;
    }
    getline(iss >> std::ws, nam); // ignore whitesummer
    proj.setName(src, nam);
  }
}
void p1fileDataReader(char *filepath, Graph &proj)
{
  ifstream srcFile(filepath);
  if (!srcFile)
    print("\e[41mError opening edges file \"{}\"\n",filepath), exit(1);
  string line;
  int Ns=proj.nofNodes;
  while (getline(srcFile, line))
  {
    istringstream iss(line);
    int src, dst, weyt;
    if (!(iss >> src >> dst >> weyt))
    {
      print("Error reading integers from line:{}\n", line);
      continue;
    }
    proj.insertEdge(src, dst, weyt);
    if(proj.nofEdges*2 >= Ns*(Ns-1)) {print("No more nodes can be inserted (edges>=nodes*(nodes-1))\n");break;}
  }
  srcFile.close();
}
void p2fileDataReader(char *filepath, Graph &proj)
{
  ifstream srcFile(filepath);
  if (!srcFile)
    print("\e[41mError opening influences file \"{}\"\n",filepath), exit(1);
  string line;
  while (getline(srcFile, line))
  {
    istringstream iss(line);
    int src, influ;
    if (!(iss >> src >> influ))
    {
      print("Error reading integers from line:{}\n", line);
      continue;
    }
    proj.setInf(src, influ);
  }
  srcFile.close();
}
