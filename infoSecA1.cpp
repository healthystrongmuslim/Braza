//By  braza Musaab
//USE FLAG --std=c++23 WHEN COMPILING
#include <print>
#include <iostream>
#include <unordered_map>
#define map std::unordered_map
using std::print,std::string;
template <class moddingclass,typename t>
inline t mod(t n,moddingclass k){return (k>0)? n= n%k + k*(n%k<0): 0;}
char upOrLow(char c){return (c&32)?'a':'A';}
void caesarShift(char* plaintext, size_t buffLen, int key)
{
  for(size_t i=0; i<buffLen; i++)
    if(plaintext[i] and isalpha(plaintext[i]))
      plaintext[i]=mod(plaintext[i]-(upOrLow(plaintext[i]))+key,26)+upOrLow(plaintext[i]);
}
void graphFreqs(map<string, int> &freqs, string title)
{
  print("\e[32m{}gram Frequencies:\n",title);
  int total=0;
  for(auto &[c,i]: freqs) print("{0}:\e[33m{1}\e[32m, ", c, i), total+=i;
  print("\n\e[mGraphed \e[2m(relative to total number of {}graphs)\e[m:\n",title);
  for(auto &[c,i]: freqs) print("{0}\e[36m{1:->{2}}\e[m\n", c, i, int(i*100/total));
  print("\n");
}
int main(int argc, char** argv)
{
  int rno=0,rnoLimit=100;
  if (2 != argc) print("\e[41mEnter only one number (last {} digits of rno: 0 to {}) after object filename\e[m\n", floor(log10(rnoLimit)), rnoLimit-1), exit(1);
  try{rno = std::stoi(argv[1]); if((rno>=rnoLimit or rno<0)) throw (3);}
  catch (std::invalid_argument &e){print("Invalid arg\n");exit(2);}
  catch (std::out_of_range &e){print("Out of range of int\n");exit(4);}
  catch (int err){print("Out of limit\tCorrecting…\n"); rno=mod(rno,rnoLimit);}
  const size_t buffLen=50;
  char userData[buffLen]={0};
  print("Enter {}'s data:\t\e[33m", rno);
  fgets(userData, buffLen, stdin);

  ////////////////////////////////////////
  //PART A
  ///////////////////////////////////////
  print("\e[mYou entered: \"\e[34m{}\e[m\"\n\
Fixed, Plaintext is:", userData);
  for (size_t i=0; i<buffLen;i++)
    if('\n'==userData[i] or (i>0 and 0==userData[i-1])) userData[i]=0;
    else if(!isalpha(userData[i]) and userData[i]!=' ') userData[i]=mod(userData[i],26)+upOrLow(userData[i]);
  userData[buffLen-1]=0;
  int key=rno%26;
  print("\"\e[104m{}\e[m\"\nKey: {}\n\
Ciphertext:\"\e[31m", userData, key);
  //Caesar shifting
  caesarShift(userData, buffLen, key);
  print("{}\e[m\"\n", userData);

  map<string, int> monoFreq,biFreq,triFreq;
  for(size_t i=0; i<buffLen; i++)
  {
    if(userData[i] and isalpha(userData[i]))
    {//Hello my name is Musab Muhammad I am very happy to be here
      char a=userData[i];
      a=a+32*(!(a&32));//Normalizing to lowercase
      monoFreq[string(1,a)]++;
      if(i+1<buffLen and userData[i+1] and isalpha(userData[i+1]))
      {
        char b=userData[i+1];
        b=b+32*(!(b&32));
        string bi=string(1,a)+b;
        biFreq[bi]++;
        if(i+2<buffLen and userData[i+2] and isalpha(userData[i+2]))
        {
          string tri=bi+char(userData[i+2]+32*(!(userData[i+2]&32)));
          triFreq[tri]++;
        }
      }
    }
  }
  graphFreqs(monoFreq, "Mono");
  graphFreqs(biFreq, "Bi");
  graphFreqs(triFreq, "Tri");
  print("Decryption:\"\e[44m");
  //Caesar unshifting
  caesarShift(userData, buffLen, -key);
  print("{}\e[m\"\n", userData);

  //for(size_t i=0; i<buffLen; i++) print("{0:d}:\e[35m{0}\e[m ", userData[i]);
  
  print("\n");
  ///////////////////////////////////////
  //PART A DONE
  ///////////////////////////////////////
}