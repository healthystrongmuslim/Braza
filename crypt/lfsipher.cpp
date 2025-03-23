#define B2bpat "%c%c%c%c%c%c%c%c" // inspired by https://stackoverflow.com/questions/111928/is-there-a-printf-converter-to-print-in-binary-format
#define B2bin(b)  '0'+(b>>7&1), '0'+(b>>6&1),'0'+(b>>5&1),'0'+(b>>4&1),'0'+(b>>3&1),'0'+(b>>2 &1),'0'+(b>>1 &1),'0'+(b&1) // much better than the stackoverflow one… perhaps more operations? 
//By  braza Musaab, for older C++
#include<stdio.h>
#include<stdlib.h>
#include<string>
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
void cipher(char* message, int key,size_t siz,FILE* ciphFile=0)
{
  if(ciphFile)
  {
    for(size_t i=0;i<siz; i++)message[i]=0;
    fread(message, 1, siz, ciphFile);
    printf("\nFrom file:\n\e[33m%s\e[m\n",message);
  }
  //printf("Message size:%lu\n",sizeof(message)); //for some reason, a C string of size 14 in caller function has size 8 here?
  for(size_t i=0; i<siz/4; i++)
  {
    *((int*)(message+i*4))^=key;
  }
  switch(siz%4)
  {
    case 0: break;
    case 1: message[siz-1]^=char(key&0xff); break;
    case 2: *((short*)(message+siz-2))^=short(key&0xffff); break;
    case 3: *((short*)(message+siz-3))^=short((key>>8)&0xffff), message[siz-1]^=char(key&0xff); break;
  }
}
void hexDump(char* message, size_t siz)
{
  printf("\e[31m%s\e[m in hex view:\n\e[2;32m",message);
  for(int i=0;i<16;i++) printf("  %2x",i);
  for(size_t i=0;i<siz;i++) (i%16)?printf("  %2x",message[i]&0xff): printf("\n\e[2;32m%2lx\e[m%2x",i/16, message[i]&0xff);
  printf("\n");
}
int main(int argc, char** argv)
{
  bool outbit, inbit;
  char bloc=0, poly=0;
  int bs, poLim=11111;//binary string, polynomial limit
  if (4 != argc) {printf("\e[41mEnter two numbers after object filename:\e[;31m\n\
    1. polynomial in binary \e[2m(of degree 2..8)\e[;31m\n\
    2. seed/initial vector in binary \e[2m(non-zero, not wider than polynomial)\n\
    3. message file path\n\
    e.g:\n\
    \e[;33m./a 1001 110 msg.txt\n"); return 1;}
  argParse(bs, poLim, argv[1],poly);
  int degree=0;
  for(int i=0; i<8; i++) if(poly>>i) degree++;
  int iters=1<<degree;
  printf("\e[94mPolynomial: " B2bpat "\e[m\nDegree: %i\tIterations: %i\n",B2bin(poly),degree,iters);
  argParse(bs, poLim, argv[2], bloc,1,1);
  FILE* msgFile = fopen(argv[3],"r");
  if(!msgFile) {printf("\e[41mFile not found: \e[;31m%s\e[m\n",argv[3]); return 5;}
  fseek(msgFile, 0, SEEK_END);
  size_t siz=ftell(msgFile);
  rewind(msgFile);
  printf("File size:%lu\n",siz);
  char* message=new char[siz+1];
  fread(message, 1, siz, msgFile);
  fclose(msgFile);
  if((bloc>>degree)>(poly>>degree)) printf("\e[33mSeed/Initial vector must not be wider than polynomial\e[m\nEqualling…"), bloc=poly;
  printf("\e[94mSeed/Initial vector: " B2bpat "\e[m\n",B2bin(bloc));
  int seq=bloc;
  hexDump(message, siz);
  for(int i=0; i<iters; i++)
  {
    printf("\e[;2m%x=" B2bpat "\e[D",bloc,B2bin(bloc));//\e[D moves cursor back, outbit is highlighted as end/LSB of byte
    inbit=exor(bloc,poly);
    outbit=bloc&1;
    bloc>>=1;
    seq<<=1, seq|=outbit;
    bloc|=(inbit)<<(degree-1);
    printf("\e[m%i ",outbit);
  }
  printf("\nKey:\e[33m%i\e[m i.e %x\nOriginal message:\e[32m%s\e[m\n", seq, seq, message);
  //printf("Message size:%lu\n",siz);
  cipher(message, seq, siz);
  printf("Encrypted (XORed) message:\n\e[33m%s\e[m\n", message);
  hexDump(message, siz);
  std::string ciphFile(argv[3]); ciphFile+="cph";
  msgFile=fopen(ciphFile.c_str(),"w");
  fprintf(msgFile,"%s",message);
  fclose(msgFile);//hopefully resets the file pointer
  msgFile=fopen(ciphFile.c_str(),"r");
  //printf("Buffer:\e[33m%s\e[m\n", message);
  //printf("Message size:%lu\n",sizeof(message));
  cipher(message, seq,siz, msgFile);
  printf("Deciphered:\e[92m%s\e[m\n",message);
  fclose(msgFile);
  delete[] message;
}