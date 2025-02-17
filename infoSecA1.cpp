//By  braza Musaab
//USE FLAG --std=c++23 WHEN COMPILING
#include <print>
#include <iostream>
#include <unordered_map>
#include <queue>
//#include <termios.h>
#include<sys/ioctl.h>
#define map std::unordered_map
#define maxHeap std::priority_queue
using std::print, std::string;

template <class moddingclass,typename t>
inline t mod(t n,moddingclass k){return (k>0)? n= n%k + k*(n%k<0): 0;}//Modulus corrects for negative numbers
char upOrLo(char c){return (c&32)?'a':'A';}

void caesarShift(char* plaintext, size_t dataLen, int key)
{
  for(size_t i=0; i<=dataLen; i++)
    if(plaintext[i] and isalpha(plaintext[i]))
      plaintext[i]=mod(plaintext[i]-(upOrLo(plaintext[i]))+key,26)+upOrLo(plaintext[i]);
}

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
  for(auto &[c,i]: freqs)
  {
    print("{0}:\e[93m{1}\e[32m, ", c, i);total+=i;
    if(i>1) max_heap.push(freqPair(c,i)); //avoid graphing singletons; singletons are not interesting
    else singles++;
  }
  print("\n\e[mGraphed \e[2m(relative to total number of {}graphs ({}), and excluding ({}) singletons)\e[m:\n",title, total, singles);
  for(; !max_heap.empty(); max_heap.pop())
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
void columnised(char userData[],size_t dataLen,size_t cols,size_t padLen)
{
  char padr='X';
  if(padLen) print("\t\e[2;33mMessage will be padded ({}) to match key…\e[m\n",padLen);
  print("Message Columnised:\n");
  for(size_t i=0;i<=dataLen;i++) print("{}\e[;3{}m{}", (i%cols==0)?'\n':'\0', i%((cols<=7)?cols:7)+1, userData[i]);
  for(size_t i=0;i<padLen;i++) print("{}",padr);
  print("\e[m\n");
}

void transposeInto(char userData[],char tpose[],size_t dataLen,size_t cols,size_t padLen)
{
  char padr='X';
  size_t rows=dataLen/cols +(padLen?1:0);
  /*/ //For debugging
  static struct termios oldt, newt;
  tcgetattr(0, &oldt);
  newt = oldt; newt.c_lflag &= ~264;
  tcsetattr(0, 0, &newt);
  getchar(), getchar(); //take blanks from enter key
  //*/
  struct winsize w;
  char garbage = ioctl(1, TIOCGWINSZ, &w);
  int termWidth = w.ws_col;//window width, for matching column colors while printing like 
  print("Transposing…\n");
  for(size_t i=0; i<dataLen+padLen; i++)
  {
    char cur=(i<dataLen)? userData[i]: padr;
    tpose[(i%cols)*rows+i/cols]=cur;
    print("\e[{}G\e[;3{}m{}{}",(i%cols)*rows+i/cols+1, i%((cols<=7)?cols:7)+1,cur, (i%rows==termWidth-1)?'\n':'\0');//
    //print("\e[1K\e[G{}\t{}->{}",tpose, i, (i%cols)*rows+i/cols), getchar(); tcsetattr(0, 0, &oldt);
  }
  print("\e[m\n");
}
void detransing(char tpose[], char dpose[], size_t len, size_t cols)
{
  size_t rows=len/cols;
  for(size_t i=0; i<len; i++)
    dpose[(i%rows)*cols+i/rows]=tpose[i];//==dpose[i]=tpose[(i%cols)*rows+i/cols];
}

int main(int argc, char** argv)
{
  int rno=0,rnoLimit=100;
  if (2 != argc) print("\e[41mEnter only one number (last {} digits of rno: 0 to {}) after object filename\e[m\n",
    floor(log10(rnoLimit)), rnoLimit-1), exit(1);
  try{rno = std::stoi(argv[1]); if((rno>=rnoLimit or rno<0)) throw (3);}
  catch (std::invalid_argument &e){print("Invalid arg\n");exit(2);}
  catch (std::out_of_range &e){print("Out of range of int\n");exit(4);}
  catch (int err){print("Out of limit\tCorrecting…\n"); rno=mod(rno,rnoLimit);}
  const size_t dataLen=500;
  char userData[dataLen+1]={0};
  print("Enter {}'s data ({} characters max):\t\e[33m", rno, dataLen);
  fgets(userData, dataLen+1, stdin);

  ////////////////////////////////////////
  //PART A
  ///////////////////////////////////////
  print("\e[mYou entered: \"\e[34m{}\e[m\"\n\
Fixed, Plaintext is:", userData);
  for (size_t i=0; i<=dataLen;i++)
    if('\n'==userData[i] or (i>0 and 0==userData[i-1])) userData[i]=0;
    else if(!isalpha(userData[i]) and userData[i]!=' ') userData[i]=' ';//mod(userData[i],26)+upOrLo(userData[i]);
  userData[dataLen]=0;
  int key=rno%26;
  print("\"\e[44m{}\e[m\"\nKey: {}\n", userData, key);
  freqAnalysis(userData,dataLen, "Original Data");
  //Caesar shifting
  caesarShift(userData, dataLen, key);
  print("Ciphertext:\"\e[46m{}\e[m\"", userData);
  freqAnalysis(userData,dataLen);
  
  //for(size_t i=0; i<dataLen; i++) print("{0:d}:\e[35m{0}\e[m ", userData[i]);
  ///////////////////////////////////////
  //PART A DONE
  ///////////////////////////////////////
  
  //////////////////////////////////////
  //PART B
  ///////////////////////////////////////
  print("\nColumnar Transposition Cipher");
  const size_t cols=6,// transpose keylength
  padLen=((dataLen%cols)?cols-dataLen%cols: 0),
  cipLen=dataLen+padLen;
  if(dataLen<=cols) print("\n\e[41mMessage too short -- exiting…\e[m\n"), exit(0);
  columnised(userData,dataLen,cols,padLen);
  char transpose[cipLen+1]={0}, detrans[cipLen+1]={0};
  transposeInto(userData,transpose,dataLen,cols,padLen);
  print("\nTransposed:\"\e[41m{}\e[m\"",transpose);
  freqAnalysis(transpose,dataLen+padLen);
  detransing(transpose,detrans,cipLen,cols);
  print("Detransposed:\"\e[46m{}\e[m\"\n",detrans);
  print("DeCaesed:\"\e[44m");
  caesarShift(userData, dataLen, -key);//Deciphering
  print("{}\e[m\"\n", userData);
  ///////////////////////////////////////
  //PART B DONE
  ///////////////////////////////////////
  
  //columnised(transposed,dataLen+padLen,row);
}
/*A tad over 500 chars:
Multiply saying void a make and fruit given own signs. Form fill hath every, i fowl were Have for living night multiply he their whales in had moved place had seas seasons you'll. Beginning day their make man earth You'll the image firmament. Were face god thing night firmament is heaven saying. Deep replenish, deep. Two were. Firmament moved shall fly man i one it, fly sea. Their fish us from created the, great divided saying brought she'd it face subdue man is creepeth together man were. Evening

Another tad from the book "Probability & Statistics for Engineers & Scientists (Ninth Edition)" by Walpole, pg 110:
As far as potential hazards with the use of material in this chapter, the warning to the reader is not to read more into the material than is evident. The general nature of the probability distribution for a specific scientific phenomenon is not obvious from what is learned in this chapter. The purpose of this chapter is for readers to learn how to manipulate a probability distribution, not to learn how to identify a specific type. Chapters 5 and 6 go a long way toward identification according to the general nature of the scientific system.

*/