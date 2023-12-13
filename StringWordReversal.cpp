/*	Made this because I vaguely recalled there was a question in a written
	Programming Fundamentals exam (1st semester, either the 2nd Sessional or the Final)
	that asked us to make a function that would operate on a given string as follows:
	each word would get reversed BUT would stay in the location in the string.
	
	During the exam and even long after it, I thought "it would be too long and complicated to design and write on paper, especially within exam time."
	2 semesters later I decided to finally try making it...
	
	There are two ways to do this:
	a) set of letters, without symbols, is a word
	b) symbols are part of words
	
	"The people're direly, truly waiting for you to rise!"
				a:"ehT elpoep'er ylerid, ylurt gnitiaw rof uoy ot esir!"
				b:"ehT er'elpoep ,ylerid ylurt gnitiaw rof uoy ot !esir"
	"What are your thoughts onMBS? (pls be nice)"
				a:"tahW era ruoy sthguoht SBMno? (slp eb ecin)"
				b:"tahW era ruoy sthguoht ?SBMno slp( eb )ecin"
*/
#include "stdc++.h"
using namespace std;

bool isletr(char c){return (c>='a' && c<='z')|| (c>='A' && c<='Z');}
void revword(string &s,int i,int j){for(;i<j;swap(s[i++], s[j--]));}
void reva(string &s){
				for(int i=0,j=0;i<s.length();i++)
								if(isletr(s[i])) {for(j=i+1;j<s.length() && isletr(s[j]);j++)
												; revword(s, i, j-1),i=j-1;}
}
void revb(string &s){
				for(int i=0,j=0;i<s.length();i++)
								if(s[i]!=' ' and s[i]!=0) {for(j=i+1;j<s.length() && (s[j]!=' ' and s[j]!=0);j++)
												; revword(s, i, j-1),i=j-1;}
}
int main(){
				string s;cout<<"\33[36m";
				getline(cin, s); reva(s); cout<<"\33[32m"<<s<<"\n";
				reva(s);// restoring to original
				revb(s); cout<<"\33[33m"<<s<<endc();
}	//Pretty short.
