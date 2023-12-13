//  Graphs.cpp
//	uncomment the printfs to show the cycle checking, BFS and DFS paths
//	also added removeEdge function for graph (and respectively for AdjLst)
//  Created by Muhammad Musab on 23/11/2023.
//	Queue was used for BFS, and to track DFS visiting/popping sequence.
//	Stack class is defined, but no Stack object was made here because the recursive calling in the DFS function means it uses a stack.

#include "stdc++.h"
using namespace std;

class Node{
				friend class Graph; friend class AdjLst; friend class Stack; friend class Queue;
				int d; Node* x;
				Node(int n=0): d(n), x(0)  {;}
				~Node(){if (x) x=0;}
};
class Stack{
				friend class Graph; friend class AdjLst;
				unsigned n; Node *h;// n is number of items in stack, h is head.
public:
				Stack(): n(0), h(NULL){;}
				
				int push(int c){Node *t=h;
								void(n++), void(h=new Node(c)), h->x=t;
								return c;}
				int pop(){if(h){int c;Node *t;
								t=h->x; void(c=h->d), h->x=NULL;
								delete h; void(h=t), n--;
								return c;}
								else cout<<"\33[1;31mStack empty\n\33[0m";return -1;}
				int peek(){cout<<"\33[1;33mpeak: "<<h->d<<"\33[0m\n"; return h->d;}
				bool mt(){return !h;}
				int getn(){return n;}
				void print(){for(Node *t=h;t;t=t->x) printf("\33[1;34m%i\t\33[0m",t->d); cout<<endl;}
				void clear(){while(n) pop();}
				~Stack(){clear();}
};
class Queue{
				friend class Graph; friend class AdjLst;
				Node *h,*l; unsigned short s=0; //h=head, l=last, s=size
				Queue():h(NULL), l(NULL), s(0) {;}
				
				void print(){Node* p= h;
								for(int i=0;p; i++, p=p->x) printf(" %i",p->d);
								cout<<endl;}
				
				void push(int i){s++;
								if (!h) {void(h=new Node(i)),l=h; return;}
								Node* p=l;
								l->x=new Node(i);
								l=l->x;}
				
				void pop(){if(h){
								Node *p=h->x;
								h->x=NULL; delete h;
								h=p; s--;}}
				
				bool mt(){return !h;}
				void clear(){while(h){pop();}}
				~Queue(){if (!h) { clear();}}
};

class AdjLst{
				Node *h,*l;
				void insert(int val){Node *temp=new Node(val);
								void((h)? l->x=temp: h=temp),l=temp;
				}
				void remove(int val){if(h) if (val!=h->d){Node *t=h,*temp,*pre=h;
								while((t)? t->d!=val :0) void(pre=t),t=t->x;
								if(t) void(temp=t->x),void(t->x=0), pre->x=temp; if(t) delete t;}
				}
				void Display(){Node *temp=h;
								for(;(temp)? temp->x: 0;temp=temp->x) printf("\33[33;1m %i \33[0m",temp->x->d);}
				AdjLst(Node* strt=0): h(strt),l(strt) {;}
				~AdjLst(){for(Node *t=h;t;t=h) {h=h->x; delete t;}}
				friend class Graph;
};
class Graph{
public:
				AdjLst* al;
				int Vs;
				Graph(int size=6):Vs(size),al(new AdjLst[size]) {for(int i=0; i<Vs; al[i].insert(i), i++);}
				void insertEdge(int src, int dest){if (src<Vs && dest<Vs && src>=0 && dest>=0 && src!=dest){Node* t=al[src].h; int n; bool match=0;//checks whether src and dest are within range
								for(;t && !match; n=t->d, match=(n==dest),t=t->x);// checks whether edge already exists
								if(!match) void(al[src].insert(dest)), al[dest].insert(src);}
				}

				void removeEdge(int src, int dest){
								if (src<Vs && dest<Vs && src>=0 && dest>=0 && src!=dest){
												Node* t=al[src].h; int n; bool match=0;//checks whether src and dest are within range
												for(;t && !match; n=t->d, match=(n==dest),t=t->x);// checks whether edge already exists
												if(match) void(al[src].remove(dest)), al[dest].remove(src);}
				}
				void showGraphStructure(){for(int i=0;i<Vs;i++) void(printf("\33[34;1m%i:\33[0m",i)), void(al[i].Display()), printf("\n");}
				
				
				void DFS(int ss=0, bool *visd=0,Queue *ordpop=0,Queue *ordvis=0){//ss determines the starting vertex
								mod(ss, Vs); bool starter=0;
								if(visd==0){starter=1;
												printf("\33[33mDFS traversal\n");
												visd=new bool[Vs]; ordpop=new Queue; ordvis=new Queue;for(int i=0;i<Vs;visd[i++]=0);
												visd[ss]=1;	ordvis->push(ss);}
								Node* t=al[ss].h->x;
								//printf("\n\33[42mEntered %i\n\33[0m",ss);
												while(t){
																if(!visd[t->d]){//printf("%i",t->d);
																				visd[t->d]=1; ordvis->push(t->d);
																				DFS(t->d,visd,ordpop,ordvis);//stack is used by recursive call
																				ordpop->push(t->d);}
																t=t->x;
												}
								//printf("\n\33[43mExited %i\n\33[0m",ss);
								if(starter) {ordpop->push(ss);
												printf("\33[0mOrder of Poppy:\33[96m");ordpop->print();
												printf("\33[0mOrder of Visit:\33[96m");ordvis->print();
												printf("\33[0m");
												delete[] visd; delete ordpop;delete ordvis;}
				}
				void BFS(int ss=0){//ss determines the starting vertex
								mod(ss, Vs);printf("\33[32mBFS traversal");
								Queue q; q.push(ss);
								bool *visd=new bool[Vs];int *ord=new int[Vs]; for(int i=0;i<Vs;visd[i++]=0, ord[i]=-1);
								visd[ss]=1;
								for(int i=0;i<Vs and not q.mt(); q.pop()){
												int cur=q.h->d;Node* t=al[cur].h->x; ord[i++]=cur;
												//printf("\n\33[32mfrom %i:\t\33[33m",cur);
												while(t){
																if(!visd[t->d]){//printf("%i ",t->d);
																																				visd[t->d]=1; q.push(t->d);}
																t=t->x;
												}
								}
								printf("%sOrder of Traversal (from first-popped):\33[96m",endcl);
								for(int i=0;i<Vs and ord[i]!=-1;printf(" %i",ord[i++]));
								endc(); delete[] visd;delete [] ord;
				}
				bool cycle(){
								bool found=0, *visd=new bool[Vs];
								for(int i=0;i<Vs && !found;i++){
												for(Node *t=al[i].h;t && !found ;t=t->x){
																for(int j=0;j<Vs;visd[j++]=0)
																				; visd[i]=1;
																if (t->x) {/*printf("Main cycle sending to %i from:%i\n",t->x->d,i),*/void(visd[t->x->d]=1), found=cycle(i,t->x->d,visd);
																				if(!found) visd[t->x->d]=0;}
												}
								}
								delete[] visd; printf("Cycle\33[3%s found\33[0m\n",((found)? "2;1m":"1;1m NOT")); return found;
				}
				~Graph(){delete[] al;}
private:
				bool cycle(int from,int check, bool* visd){bool found=0;//from is the node called from, check is the current node to check, visd is the array of visited nodes
								for(Node *t=al[check].h; t  && !found; t=t->x)
												if(from!=t->d && check!=t->d){
																if(visd[t->d]) {/*printf("Found %i at %i from %i\n",t->d,check,from);*/ return 1;}
																else {void(visd[t->d]=1), /*printf("From:%i Check:%i To:%i\n",from,check,t->d),*/ found=cycle(check,t->d,visd);
																				if(!found) visd[t->d]=0;//to unvisit unsuccessful paths, while alternative path is tried
																				}
												}
								return found;
				}
};


int main(){int Vs=6;
				Graph g(Vs);
				g.insertEdge(1,0), g.showGraphStructure(), g.cycle(); g.BFS(2), g.DFS(2);// not cycle
				g.insertEdge(1,2), g.insertEdge(2,3), g.insertEdge(4,3), g.insertEdge(5,4), g.showGraphStructure(), g.cycle(); g.BFS(2),g.DFS(2);//not cycle
				g.insertEdge(1,5), 	g.showGraphStructure(), g.cycle();g.BFS(2), g.DFS(2);//cycle
}
