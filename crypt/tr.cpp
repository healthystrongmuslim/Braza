//By  braza Musaab
//USE FLAG --std=c++23 WHEN COMPILING
#include <print>
#include <fstream>
#include <unordered_map>
#define map std::unordered_map
//utility
using std::string;
map<char,char> subMapper(string orig, string sub)
{
  for(char& c: orig) c=c+32*(!(c&32));//normalizing to lowercase
  for(char& c: sub) c=c+32*(!(c&32));
  map<char,char> subMap;
  for(size_t i=0; i<orig.size(); i++) subMap[orig[i]]=sub[i], subMap[orig[i]&95]=sub[i]&95;
  return subMap;
}

int main(int argc, char** argv)
{
  int rno=0,rnoLimit=100;
  if (4 != argc) printf("\e[41mEnter dir of unzipped Lab after object filename (a dot . if in same dir as it),\
    then original-letters-string and substitute-letters-string (case insensitive, same length), like so:\e[m\
    ./a . ytn THE\n"), exit(1);
  const size_t dataLen=4760;// ==Filesize
  char userData[dataLen+1]={0};
  string dir(argv[1]);dir+="/Labsetup-arm/Files/ciphertext.txt";
  FILE* cipht=fopen(dir.c_str(),"r");
  if(!cipht) printf("\e[41mFile not found: \e[;31m%s\e[m\n",dir.c_str()), exit(5);
  fread(userData, 1, dataLen, cipht);
  userData[dataLen]=0;
  fclose(cipht);
  string orig(argv[2]),sub(argv[3]);
  if(orig.size()!=sub.size()) printf("\e[41mOriginal and Substitute strings must be of equal length\e[m\n"), exit(2);
  auto submap=subMapper(orig,sub);
  for(char& c: userData)
  {
    if(submap.find(c)!=submap.end()) printf("\e[33m%c\e[m",submap[c]);
    else printf("\e[;2m%c\e[m",c);
  }
  printf("\n");
}