//By  braza Musaab
//USE FLAG --std=c++23 WHEN COMPILING
#include <print>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>
#define map std::unordered_map
#define maxHeap std::priority_queue
//utility
using std::print, std::string;
template <class moddingclass,typename t>
inline t mod(t n,moddingclass k){return (k>0)? n= n%k + k*(n%k<0): 0;}//Modulus corrects for negative numbers
char upOrLo(char c){return (c&32)?'a':'A';}
//analysis
struct freqPair
{
  string c;
  int i;
  freqPair(string c, int i): c(c), i(i){}
};
struct CompareFreq
{
  bool operator()(freqPair const& j1, freqPair const& j2)
  {
    return j1.i < j2.i;
  }
};
void graphFreqs(map<string, int> &freqs, string title)
{
  int total=0, precision=4,singles=0;
  maxHeap<freqPair, std::vector<freqPair>, CompareFreq> max_heap;
  print("\e[32m{}gram Frequencies:\n",title);
  int minFreq=2,//min freq to graph
   maxGrams=30, grams=0;//prints upto maxGrams number of lines
  for(auto &[c,i]: freqs)// prints all frequency counts and calculates total
  {
    //print("{0}:\e[93m{1}\e[32m,", c, i);
    total+=i;
    if(i>minFreq) max_heap.push(freqPair(c,i)); //avoid graphing rares; rares are not interesting
    else singles++;
  }
  print("\n\e[mGraphed \e[2m(relative to total number of {}graphs ({}), and excluding ({}) rares/singletons (freq <={}))\e[m:\n",title, total, singles, minFreq);
  for(; !max_heap.empty() && grams<maxGrams; max_heap.pop(), grams++)
  {
    freqPair fp=max_heap.top();
    print("{0}\e[4{5}m{1:>{2}.{4}f}\e[;2;36m {3}\e[m\n",
    fp.c, float(fp.i)/total, int(float(fp.i*500)/total)+precision+1, fp.i, precision,fp.i%7+1);
  }
  //c string, i occurences, i/total relative[-to-total] observed occurence rate, i*500/total graph width, precision
  print("\n");
}
void freqAnalysis(char* userData, const size_t dataLen,string title="")
{
  print("\n\e[93m{} Frequency Analysis\e[m\n",title);
  map<string, int> monoFreq,biFreq,triFreq;
  for(size_t i=0; i<=dataLen; i++)
  {
    if(!(userData[i] and isalpha(userData[i]))) continue;
    //Hello my name is Musab Muhammad I am very happy to be here
    char a=userData[i];
    a=a+32*(!(a&32));//Normalizing to lowercase
    monoFreq[string(1,a)]++;
    if(!(i+1<=dataLen and userData[i+1] and isalpha(userData[i+1]))) continue;
    char b=userData[i+1];
    b=b+32*(!(b&32));
    string bi=string(1,a)+b;
    biFreq[bi]++;
    if(!(i+2<=dataLen and userData[i+2] and isalpha(userData[i+2]))) continue;
    string tri=bi+char(userData[i+2]+32*(!(userData[i+2]&32)));
    triFreq[tri]++;
  }
  graphFreqs(monoFreq, "Mono");
  graphFreqs(biFreq, "Bi");
  graphFreqs(triFreq, "Tri");
}
//ciphers
int main(int argc, char** argv)
{
  int rno=0,rnoLimit=100;
  if (2 != argc) print("\e[41mEnter dir of unzipped Lab after object filename (a dot . if in same dir as it)\e[m\n"), exit(1);
  const size_t dataLen=4760;// ==Filesize
  char userData[dataLen+1]={0};
  string dir(argv[1]);dir+="/Labsetup-arm/Files/ciphertext.txt";
  FILE* cipht=fopen(dir.c_str(),"r");
  if(!cipht) print("\e[41mFile not found: \e[;31m{}\e[m\n",dir), exit(5);
  fread(userData, 1, dataLen, cipht);
  userData[dataLen]=0;
  fclose(cipht);
  print("\e[mYou entered: \"\e[34m{}\e[m", userData);
  freqAnalysis(userData,dataLen, "Original Data");
}
/*A tad over 500 chars:
Multiply saying void a make and fruit given own signs. Form fill hath every, i fowl were Have for living night multiply he their whales in had moved place had seas seasons you'll. Beginning day their make man earth You'll the image firmament. Were face god thing night firmament is heaven saying. Deep replenish, deep. Two were. Firmament moved shall fly man i one it, fly sea. Their fish us from created the, great divided saying brought she'd it face subdue man is creepeth together man were. Evening

Another tad from the book "Probability & Statistics for Engineers & Scientists (Ninth Edition)" by Walpole, pg 110:
As far as potential hazards with the use of material in this chapter, the warning to the reader is not to read more into the material than is evident. The general nature of the probability distribution for a specific scientific phenomenon is not obvious from what is learned in this chapter. The purpose of this chapter is for readers to learn how to manipulate a probability distribution, not to learn how to identify a specific type. Chapters 5 and 6 go a long way toward identification according to the general nature of the scientific system.

*/