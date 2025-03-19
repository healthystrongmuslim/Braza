//By  braza Musaab
//USE FLAG --std=c++23 WHEN COMPILING
#if !defined(__cplusplus) || __cplusplus < 202302L
#error "This code requires C++23 or later. Please compile with --std=c++23."
#endif
#include <print>
using std::print;

bool exor(char bloc, char poly)//returns new in-bit; up to 8-bit polynomial supported
{
  bool val=0;//char bitcount=0;
  bloc&=poly;//only bits included in poly are included in xor
  while(bloc) val^=bloc&1, bloc>>=1;// while(poly) val^=(bloc&1)&(poly&1), bloc>>=1,poly>>=1, bitcount+=val;
  return val;// return bitcount%2; return bitcount&1;
}
void argParse(int& bs, int poLim, char* argv, char& dat,int deflt =1010)
{
  int tmp=bs;
  try{bs = std::stoi(argv); if((bs>poLim or bs<11)) print("Out of limit\tDefaulting…\n"), bs=deflt;}
  catch (std::invalid_argument &e){print("Invalid arg\tDefaulting…\n"); bs=deflt;}
  catch (std::out_of_range &e){print("Out of range of int\tDefaulting…\n"); bs=deflt;}
  //print("Polynomial in bits (rightmost is degree 1 i.e. x^1): \n");
  for(int i=0;i<8 && bs ;i++, bs/=10) dat|=((bs%10)&1)<<i/* , print("{:b}\n", poly) */;
  //return 0;
  bs=tmp;
}
int main(int argc, char** argv)
{
  bool outbit, inbit;
  char bloc=0, poly=0;
  int bs, poLim=1111;//binary string, polynomial limit
  if (3 != argc) print("\e[41mEnter two numbers after object filename:\e[;31m\n\
    1. polynomial in binary \e[2m(of degree 2..8)\e[;31m\n\
    2. seed/initial vector in binary \e[2m(non-zero, not wider than polynomial)\n\
    e.g:\n\
    \e[;33m./a 1001 110\n"), exit(1);
  argParse(bs, poLim, argv[1],poly);
  int degree=0;
  for(int i=0; i<8; i++) if(poly>>i) degree++;
  int iters=1<<degree;
  print("\e[94mPolynomial: {:0>8b}\e[m\nDegree: {}\tIterations: {}\n",poly,degree,iters);
  argParse(bs, poLim, argv[2], bloc,1);
  if((bloc>>degree)>(poly>>degree)) print("\e[33mSeed/Initial vector must not be wider than polynomial\e[m\nEqualling…"), bloc=poly;
  print("\e[94mSeed/Initial vector: {:0>8b}\e[m\n",bloc);

  for(int i=0; i<iters; i++)
  {
    print("\e[2;32m{0}:\t\e[;2m{1:x}={1:0>8b}\e[D",i,bloc);//\e[D moves cursor back, outbit is highlighted as end/LSB of byte
    inbit=exor(bloc,poly);
    outbit=bloc&1;
    bloc>>=1;
    bloc|=(inbit)<<(degree-1);
    print("\e[m{:b}\n",outbit);
  }
}
