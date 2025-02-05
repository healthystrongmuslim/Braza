/*/ Find unused rnos; input and print with names
// compile with "--std=c++23" flag
// sample input in the end comment
#include <print>
#include <iostream>
#include <string>
#include <unordered_map>
using std::print, std::cin, std::string, std::unordered_map;
int main()
{
  string c="t";//buffer
  const  char delim = '-';
  unordered_map<int,string> allRnos;
  while(c[0]!=delim)
  {
    cin>>c;
    if(c[0]!=delim and atoi(c.c_str())) {
      int rno= stoi(c);
      cin>> c;//inputting names
      allRnos[rno]=c;
    }
  }
  c="t";
  while(c[0]!=delim)
  {
    cin>>c;
    if(c[0]!=delim and
    atoi(c.c_str()) and
    allRnos.find(stoi(c))!=allRnos.end()){
      allRnos.erase(stoi(c));
    }
  }
  for (auto& [rno,n]: allRnos) print("\e[34m{} {}\n\e[m", rno, n);
}
// 154370	"MusabAlBraza" 335982	"AbuBakarAlKuwaiti" 321476	"MuhammadAhmad" 160497	"MuhammadIbnAbdulWahhab" 135982	"UmarFaruq"  277431	"Abdullah" 865121	"YoooooooHussain"  386510	"AbdullahIbnUmar"  577832	"YoAngelo"  -----  154370    160497  135982  321571  -----

//*/
//*
// Find unused rnos
// compile with "--std=c++23" flag
// sample input in the end comment
#include <print>
#include <iostream>
#include <string>
#include <unordered_set>
using std::print, std::cin, std::string, std::unordered_set;
int main()
{
  string c="t";//buffer
  const  char delim = '-';
  unordered_set<int> allRnos;
  while(c[0]!=delim)
  {
    cin>>c;
    if(c[0]!=delim and atoi(c.c_str()))
      allRnos.insert(stoi(c));
  }
  c="t";
  while(c[0]!=delim)
  {
    cin>>c;
    if(c[0]!=delim and
    atoi(c.c_str()) and
    allRnos.find(stoi(c))!=allRnos.end())
      allRnos.erase(stoi(c));
  }
  for (auto& rno: allRnos) print("\e[34m{} \n\e[m", rno);
}
// 154370 335982 321476 160497	135982 277431 865121 386510  577832	 -----  154370    160497  135982  321571  -----
//*/
