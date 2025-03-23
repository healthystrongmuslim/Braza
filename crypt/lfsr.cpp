// Using an LFSR to generate a bitstream to use as key for encryption
#define B2bpat "%c%c%c%c%c%c%c%c" // inspired by https://stackoverflow.com/questions/111928/is-there-a-printf-converter-to-print-in-binary-format
#define B2bin(b)  '0'+(b>>7&1), '0'+(b>>6&1),'0'+(b>>5&1),'0'+(b>>4&1),'0'+(b>>3&1),'0'+(b>>2 &1),'0'+(b>>1 &1),'0'+(b&1) // much better than the stackoverflow one… perhaps more operations? 
//By  braza Musaab, for older C++
#include<stdio.h>
#include<stdlib.h>
#include<set>
bool exor(char bloc, char poly)//returns new in-bit; up to 8-bit polynomial supported
{
  bool val=0;//char bitcount=0;
  bloc&=poly;//only bits included in poly are included in xor
  while(bloc) val^=bloc&1, bloc>>=1;// while(poly) val^=(bloc&1)&(poly&1), bloc>>=1,poly>>=1, bitcount+=val;
  return val;// return bitcount%2; return bitcount&1;
}
void argParse(int& bs, int poLim, char* argv, char& dat,int deflt =1010, int min=11)
{
  int tmp=bs;
  bs = atoi(argv); if((bs>poLim or bs<min)) printf("Out of limit\tDefaulting…\n"), bs=deflt;
  //print("Polynomial in bits (rightmost is degree 1 i.e. x^1): \n");
  for(int i=0;i<8 && bs ;i++, bs/=10) dat|=((bs%10)&1)<<i/* , print(B2bpat "\n", B2bin(poly)) */;
  //return 0;
  bs=tmp;
}
int main(int argc, char** argv)
{
  bool outbit, inbit;
  char bloc=0, poly=0;
  int bs, poLim=11111;//binary string, polynomial limit
  if (3 != argc) {printf("\e[41mEnter two numbers after object filename:\e[;31m\n\
    1. polynomial in binary \e[2m(of degree 2..8)\e[;31m\n\
    2. seed/initial vector in binary \e[2m(non-zero, not wider than polynomial)\n\
    e.g:\n\
    \e[;33m./a 1001 110\n"); return 1;}
  argParse(bs, poLim, argv[1],poly);
  int degree=0;
  for(int i=0; i<8; i++) if(poly>>i) degree++;
  int iters=1<<degree;
  printf("\e[94mPolynomial: " B2bpat "\e[m\nDegree: %i\tIterations: %i\n",B2bin(poly),degree,iters);
  argParse(bs, poLim, argv[2], bloc,1,1);
  if((bloc>>degree)>(poly>>degree)) printf("\e[33mSeed/Initial vector must not be wider than polynomial\e[m\nEqualling…"), bloc=poly;
  printf("\e[94mSeed/Initial vector: " B2bpat "\e[m\n",B2bin(bloc));
  std::set<char> seq;
  bool repeat=false;
  for(int i=0; i<iters-1 && !repeat; i++)
  {
    if(seq.count(bloc)) repeat=true;
    else seq.insert(bloc);
    printf("\e[2;32m%i:\t\e[;2m%x=" B2bpat "\e[D",i,bloc,B2bin(bloc));//\e[D moves cursor back, outbit is highlighted as end/LSB of byte
    inbit=exor(bloc,poly);
    outbit=bloc&1;
    bloc>>=1;
    bloc|=(inbit)<<(degree-1);
    printf("\e[m%i\n",outbit);
  }
  printf("\e[%s\e[m\n",repeat?"41mRepeating":"42mNo repeats; primitive polynomial");
}