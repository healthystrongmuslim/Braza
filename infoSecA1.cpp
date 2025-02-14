//By  braza Musaab
//USE FLAG --std=c++23 WHEN COMPILING
#include <print>
#include <iostream>
#include <unordered_map>
//#include <termios.h>
#define map std::unordered_map
using std::print,std::string;
template <class moddingclass,typename t>
inline t mod(t n,moddingclass k){return (k>0)? n= n%k + k*(n%k<0): 0;}//Modulus corrects for negative numbers
char upOrLo(char c){return (c&32)?'a':'A';}

void caesarShift(char* plaintext, size_t dataLen, int key)
{
  for(size_t i=0; i<=dataLen; i++)
    if(plaintext[i] and isalpha(plaintext[i]))
      plaintext[i]=mod(plaintext[i]-(upOrLo(plaintext[i]))+key,26)+upOrLo(plaintext[i]);
}
void graphFreqs(map<string, int> &freqs, string title)
{
  print("\e[32m{}gram Frequencies:\n",title);
  int total=0;
  for(auto &[c,i]: freqs) print("{0}:\e[93m{1}\e[32m, ", c, i), total+=i;
  print("\n\e[mGraphed \e[2m(relative to total number of {}graphs)\e[m:\n",title);
  for(auto &[c,i]: freqs) print("{0}\e[36m{1:->{2}}\e[m\n", c, i, int(i*100/total));
  print("\n");
}

void freqAnalysis(char* userData, const size_t dataLen)
{
  print("\n\e[93mFrequency Analysis\e[m\n");
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
  print("Message Columnised:\n\e[46m");
  for(size_t i=0;i<=dataLen;) print("{}{}",userData[i++], (i%cols==0)?'\n':'\0');
  for(size_t i=0;i<padLen;i++) print("{}",padr);
  print("\e[m\n");
}

void transposeInto(char userData[],char tpose[],size_t dataLen,size_t cols,size_t padLen)
{
  char padr='X';
  size_t rows=dataLen/cols +(padLen?1:0);
  /*/
  static struct termios oldt, newt;
  tcgetattr(0, &oldt);
  newt = oldt; newt.c_lflag &= ~264;
  tcsetattr(0, 0, &newt);
  getchar(), getchar();
  //*/
  for(size_t i=0; i<dataLen+padLen; i++)
    tpose[(i%cols)*rows+i/cols]=(i<dataLen)? userData[i]: padr;
    //,print("\e[1K\e[G{}\t{}->{}",tpose, i, (i%cols)*rows+i/cols), getchar(); tcsetattr(0, 0, &oldt);
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
  const size_t dataLen=50;
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
    else if(!isalpha(userData[i]) and userData[i]!=' ') userData[i]=mod(userData[i],26)+upOrLo(userData[i]);
  userData[dataLen]=0;
  int key=rno%26;
  print("\"\e[44m{}\e[m\"\nKey: {}\n\
Ciphertext:\"\e[46m", userData, key);
  //Caesar shifting
  caesarShift(userData, dataLen, key);
  print("{}\e[m\"", userData);
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