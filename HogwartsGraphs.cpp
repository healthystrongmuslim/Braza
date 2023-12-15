// uncomment the printfs to show the cycle checking path
// also added removeEdge function for graph (and respectively for AdjLst)
//  insertEdge function first checks whether there are such students and whether the edge already exists or not, then inserts accordingly

#include "stdc++.h"
using namespace std;

class Student{
				string	name;int id;short magab, brav, cunni, intel, lol;
				Student(int i=0): id(i) {//magab=rand()%11,brav=rand()%11, cunni=rand()%11, intel=rand()%11, lol=rand()%11;
								cin>>name>>magab>>brav>>cunni>>intel>>lol;
								mod(magab,11);mod(brav,11);mod(cunni,11);mod(intel,11);mod(lol,11);
				}
				Student(string n,int i): name(n), id(i) {//magab=rand()%11,brav=rand()%11, cunni=rand()%11, intel=rand()%11, lol=rand()%11;
								cin>>magab>>brav>>cunni>>intel>>lol;
								mod(magab,11);mod(brav,11);mod(cunni,11);mod(intel,11);mod(lol,11);
				}
				Student(Student &s): name(s.name), id(s.id), magab(s.magab), brav(s.brav), cunni(s.cunni), intel(s.intel), lol(s.lol) {}
				bool operator==(Student &s){return id==s.id;}
				bool operator==(int &s){return id==s;}
				friend std::ostream& operator<<(std::ostream& os, const Student& obj){
								os << obj.name; printf("\t\33[33mID:%i\tMagic Abilities:%i\tBravery:%i\tCunningness:%i\tIntelligence:%i\tLoyalty:%i\n",obj.id,obj.magab,obj.brav,obj.cunni, obj.intel, obj.lol);
										return os;
				}
				
				friend std::istream &operator>>(std::istream& is, Student &s){cin>>s.name>>s.magab>>s.brav>>s.cunni>>s.intel>>s.lol;
								mod(s.magab,11);mod(s.brav,11);mod(s.cunni,11);mod(s.intel,11);mod(s.lol,11);
								return is;
				}
				friend class Node; friend class Graph; friend class AdjLst;
};

class Node{
				friend class Graph; friend class AdjLst;
				Student d; Node* x;
				Node(Student &s): d(s), x(0)  {;}
				~Node(){if (x) x=0;}
};
class AdjLst{
				Node *h,*l;
				void insert(Student &s){Node *temp=new Node(s);
								void((h)? l->x=temp: h=temp),l=temp;
				}
				void remove(Student &s){if(h) if (s.id!=h->d.id){Node *t=h,*temp,*pre=h;
								while((t)? t->d.id!=s.id:0) void(pre=t),t=t->x;
								if(t) temp=t->x,t->x=0, pre->x=temp; if(t) delete t;}
				}
				
				void Display(){Node *temp=h;
								for(;(temp)? temp->x: 0;temp=temp->x) cout<<"\33[33;1m"<<temp->x->d.name<<" "<<temp->x->d.id<< "\t\33[0m";}
				
				AdjLst(Node* strt=0): h(strt),l(strt) {}
				~AdjLst(){for(Node *t=h;t;t=h) {h=h->x; delete t;}}
				friend class Graph;
};

class Graph{
public:
				AdjLst* al;
				int* House;
				int Vs;
				Graph(int size=6):Vs((size>-1)?size:size*-1),al(new AdjLst[(size>-1)?size:size*-1]), House(0) {
								cout<<"\33[35mGryffindor 1: High bravery and loyalty\tHufflepuff 2: High loyalty and low intelligence\nRavenclaw 3: High intelligence and cunning\tSlytherin 4: High cunning and magical ability\nID:\tName:\tMagic Abilities:\tBravery:\tCunningness:\tIntelligence:\tLoyalty:\n";
								for(int i=0; i<Vs;  i++){
												cout<<i<<"\t";
												Student s(i);
												al[i].insert(s);
								}
								endc();
				}
				void SortingHat(){
								printf("\33[1;35mSorting into Houses\n\33[0;33m");
								if(!House){	House=new int[Vs];}
												for(int i=0;i<Vs;i++){
																/*0
																	Gryffindor 1: High bravery and loyalty
																	Hufflepuff 2: High loyalty and low intelligence
																	Ravenclaw 3: High intelligence and cunni
																	Slytherin 4: High cunning and magical ability*/
																if(al[i].h->d.brav > 5){
																				if(al[i].h->d.lol > 5)  House[i]=1;
																				else 	if(al[i].h->d.intel < 6)  House[i]=2;
																				else {cout<<"Which does "<<al[i].h->d.name<<" want to be? (Even for Gryffindor, Odd for Hufflepuff)\t";
																								int chc; cin>>chc; House[i]=chc%2+1;}
																}
																else if(al[i].h->d.cunni > 5){
																				if(al[i].h->d.intel > 5) House[i]=3;
																				else if(al[i].h->d.magab > 5) House[i]=4;
																				else {cout<<"Which does "<<al[i].h->d.name<<" want to be? (Even for Ravenclaw, Odd for Slytherin)\t"; int chc; cin>>chc; House[i]=chc%2+3; }
																}
																else{cout<<"Which does "<<al[i].h->d.name<<" want to be? (1-4)\t"; int chc; cin>>chc; House[i]=(chc-1)%4+1;}
												}
												for(int i=0;i<Vs;i++) cout<<al[i].h->d.name<<" is in "
																<<((House[i]>2)?(House[i]>3)?"Slytherin":"Ravenclaw" :(House[i]>1)?"Hufflepuff":"Gryffindor")
																<< "!\n";
												endc();
				}
				void insertEdge(string src, string dest){int i1=-1,i2=-1;
								bool m1=haveStudent(src, i1),m2=haveStudent(dest, i2);
								if(m1 && m2) if (i1!=i2) insertEdge(i1, i2);
				}
				void removeEdge(string src, string dest){int i1=-1,i2=-1;
								bool m1=haveStudent(src, i1),m2=haveStudent(dest, i2);
								if(m1 && m2) if (i1!=i2) removeEdge(i1, i2);
				}
				
				bool haveStudent(string src,int &j){ bool m=0;//store id in j
								for(int i=0;i<Vs && !m; i++){
												string s=al[i].h->d.name;
												if(s.length()==src.length()){
																bool smatch=1;
																for(int j=0; j<s.length() && smatch;j++) smatch=s[j]==src[j];
																if(smatch) m=1;
												}
												if(m) j=i;
								}
								return m;
				}
				
				void showGraphStructure(){for(int i=0;i<Vs;i++) {cout<<"\33[34;1m"<<al[i].h->d.name<<" "<<i<<":\33[0m"; void(al[i].Display()), printf("\n");}}

				bool cycle(){bool found=0, *visd=new bool[Vs];
								for(int i=0;i<Vs && !found;i++){
												for(Node *t=al[i].h;t && !found ;t=t->x){
																for(int j=0;j<Vs;visd[j++]=0)
																				; visd[i]=1;
																if (t->x) {/*printf("Main cycle sending to %i from:%i\n",t->x->d,i), */visd[t->x->d.id]=1, found=cycle(i,t->x->d.id,visd);
																				if(!found) visd[t->x->d.id]=0;}
												}
								}
								delete[] visd;
								printf("Cycle\33[3%s found%s",((found)? "2;1m":"1;1m NOT"),endcl);
								return found;
}
				~Graph(){delete[] al;if(House) delete[] House;}
private:
				void insertEdge(int src, int dest){
								if (src<Vs && dest<Vs && src>=0 && dest>=0 && src!=dest){Node* t=al[src].h; int n; bool match=0;//checks whether src and dest are within range
												for(;t && !match; n=t->d.id, match=(n==dest),t=t->x);// checks whether edge already exists
												if(!match) void(al[src].insert(al[dest].h->d)), al[dest].insert(al[src].h->d);}
				}
				void removeEdge(int src, int dest){
								if (src<Vs && dest<Vs && src>=0 && dest>=0 && src!=dest){
												Node* t=al[src].h; int n; bool match=0;//checks whether src and dest are within range
												for(;t && !match; n=t->d.id, match=(n==dest),t=t->x);// checks whether edge already exists
												if(match) void(al[src].remove(al[dest].h->d)), al[dest].remove(al[src].h->d);}
				}
				bool cycle(int from,int check, bool* visd){bool found=0;//from is the node called from, check is the current node to check, visd is the array of visited nodes
								for(Node *t=al[check].h; t  && !found; t=t->x)
												if(from!=t->d.id && check!=t->d.id){
																if(visd[t->d.id]) {/*printf("Found %i at %i from %i\n",t->d,check,from);*/ return 1;}
																else {void(visd[t->d.id]=1), /*printf("From:%i Check:%i To:%i\n",from,check,t->d),*/ found=cycle(check,t->d.id,visd);
																				if(!found) visd[t->d.id]=0;//to unvisit unsuccessful paths, while alternative path is tried
																				}
												}
								return found;
				}
};

int main(){
				Graph g(4);
				g.SortingHat();
}

