/*
	main
	Data Structures Project
	*/
#include "stdc++.h"
using namespace std;
unsigned long st=time(0);
time_t result=time(nullptr);
ifstream datain("/Users/healthystrongmuslim/Documents/DataStructuresProject/DataStructuresProject/DataProjectData.txt");//need absolute file path of DataProjectData.txt here for some reason
ofstream outt("File55.txt");
int login(){string s,p; cout<<"\33[30;107m||Login||\nEnter C for Controller, truck id# (0-4) for Truck drivers, then enter password: ";
				cin>>s;
				if(s[0]=='C'){cin>>p;
								if(p.length()>=5) if(p[0]=='a' && p[1]=='d' && p[2]=='m' && p[3]=='i' && p[4]=='n') {
												cout<< "Logged in as controller"<<endc();return 100;}
				}
				else if(s[0]<='4' && s[0]>='0'){
								cin>>p; if(p.length()>=5) if(p[0]=='t' &&p[1]=='r' &&p[2]=='u' &&p[3]=='c' &&p[4]==s[0]) {
												cout<< "Logged in as truck driver"<<endc();return s[0]-'0';}
				}
				cout<<"Invalid entry"; endc();
				return -1;
				
}
class Place{
public:
				string	name; int id;
				Place(string n,int i):name(n),id(i) {}
				friend std::ostream& operator<<(std::ostream& os, Place& l){
								os<<"\33[47;30mPlace ID:"<<l.id
								<<"\33[0m\t\33[103;31mPlace name:"<<l.name;
								endc();
								return os;
				}
				Place(){}
				Place& operator=(Place b){void(name=b.name), void(id=b.id);
								return *this;
				}
				friend std::istream &operator>>(std::istream& is, Place &s){
								return is;}
				
				friend class AdjMat;
};

class Bin: public Place{
				int cap; int lvl;
				friend class AdjMat;
				Bin():Place(), cap(100),lvl(0) {}
				Bin(string n,int i, int c):Place(n,i), cap(c),lvl(0) {
				}
				bool full(){return (float(lvl)/cap)>0.75;}
				
				void fill(){if (cap!=lvl) lvl+=rand()%(cap+1-lvl);}//randoms fills bin, always increasing from (or at least maintaining) the previous level, and always stays within capacity.
				
				Bin& operator=(Bin b){void(name=b.name), void(id=b.id), cap=b.cap;
								return *this;
				}
				friend std::ostream& operator<<(std::ostream& os, Bin& l){
								os<<
								"\33[47;30mBin ID:"<<l.id<<
								"\33[0;36m\tCapacity:"<<l.cap<<"\tPercentage full:\33["<<((l.full())?"91":"32")<<";1m"<<(100*float(l.lvl)/l.cap)<<'%'
								<<"\t\33[103;31mBin name:"<<l.name;
								endc();
								return os;
				}
				friend class Graph;friend class ShortPath;
};

class AdjMat{
				float** times;const int s;
				AdjMat(int size=12): s(size), times(0){
								times=new float*[s];
								for(int i=0;i<s;times[i++]=new float[s]);
				}
				void Display(){cout<<"\33[36;1m\tTo Location ID\nFrom Location ID-> ^\n\n\t";
								for(int i=0;i<s;printf("%i\t",i++))
								; endc();
								for(int i=0;i<s;i++) {printf("\33[36;1m%i\33[33;1m\t",i);
												for(int j=0;j<s;j++) cout<<fixed<<setprecision(bool(times[i][j]))<<
																((!times[i][j])?"\33[93;1m":"")<<times[i][j] /**<((!times[i][j])?"\33[33;1m":"")*/<<"\t";
												endc();
								}
				}

				~AdjMat(){for(int i=0;i<s;delete[] times[i++]);
								delete[] times;}
				friend class Graph;friend class ShortPath;
};








class ShortPath{//Shortest path -- Dijkstra's algo
public:
				AdjMat *am; Bin* bins;
				const int Vs;
				bool* visd;
				float* mintimes;
				int* pars;
				ShortPath(Bin *b,Place *candf,AdjMat *a): am(a), pars(0),visd(0),
				mintimes(0),Vs(a->s), bins(b) {}
				
				
				int* OrderofTravel(int start,int dest){int* order=0;
								//gives an array with the order in which the truck should travel between two points
								
								if(start!=dest && dest!=Vs-1 && start!=Vs-1)//Garbage collection: center to bins
								{
												visd= new bool[Vs];
												mintimes=new float[Vs];
												pars= new int[Vs];
												for(int i=0;i<Vs; visd[i]=((i>Vs-3)? 0: !bins[i].lvl), mintimes[i]=100000, pars[i++]=-1);//only EMPTY bins are marked visited here
												pars[start]=start; mintimes[start]=0;
												//initialization done
												for(int checkvert=start;!visd[dest];){
																visd[checkvert]=1;
																for(int i=0;i<Vs;i++){//minimum distance updating for each unvisited node
																				if(!visd[i])
																								if((am->times[checkvert][i]+mintimes[checkvert]) < mintimes[i]) mintimes[i]=am->times[pars[i]=checkvert][i]+mintimes[checkvert];
																}
																//going to minimum vertex to check next
																for(int i=0,o=checkvert;i<Vs-1 && o==checkvert;i++) if(!visd[i]) checkvert=i;
																for(int i=0;i<Vs-1;i++) if(!visd[i] && i!=checkvert) if(mintimes[checkvert]>mintimes[i]) checkvert=i;
												}
												int k=1;
												for(int i=dest;
																k<Vs && i!=start;
																k++,i=pars[i]);
												order=new int[k];
												for(int i=dest,j=k; j>0 && !(i==start && pars[i]==start); j--,i=pars[i])
																order[j-1]=i;//reverse-tracing the path of travel
												delete [] mintimes; mintimes=0;
												delete [] visd; visd=0;
												delete [] pars; pars=0;
								}
								else if(start!=dest && dest==Vs-1)//Garbage disposal: bin to landfill
								{
												visd= new bool[Vs];
												mintimes=new float[Vs];
												pars= new int[Vs];
												for(int i=0;i<Vs; visd[i]=((i>Vs-3)? 0: !bins[i].lvl), mintimes[i]=100000, pars[i++]=-1);//only EMPTY bins are marked visited here
												pars[start]=start; mintimes[start]=0;
												//initialization done
												for(int checkvert=start;!visd[dest];){
																visd[checkvert]=1;
																for(int i=0;i<Vs;i++){//minimum distance updating for each unvisited node
																				if(!visd[i])
																								if((am->times[checkvert][i]+mintimes[checkvert])<mintimes[i]) mintimes[i]=am->times[pars[i]=checkvert][i]+mintimes[checkvert];
																}
																//going to minimum vertex to check next
																for(int i=0,o=checkvert;i<Vs && o==checkvert;i++) if(!visd[i]) checkvert=i;
																for(int i=0;i<Vs;i++) if(!visd[i] && i!=checkvert) if(mintimes[checkvert]>mintimes[i]) checkvert=i;
												}
												int k=1;
												for(int i=dest;k<Vs && i!=start;k++,i=pars[i]) //going to the parent and increasing the size
												; order=new int[k];
												for(int i=dest,j=k; j>0 && !(i==start && pars[i]==start);j--,i=pars[i]) order[j-1]=i;//reverse-tracing the path of travel
												delete [] mintimes; mintimes=0;
												delete [] visd; visd=0;
												delete [] pars; pars=0;
								}
								
								else if(start==Vs-1 && dest==Vs-2)//landfill to center
								{
												/*visd= new bool[Vs];
												mintimes=new float[Vs];
												pars= new int[Vs];
												for(int i=0;i<Vs; visd[i]=((i>Vs-3)? 0: !bins[i].full()), mintimes[i]=100000, pars[i++]=-1);//bins above threshhold are univisited
												pars[start]=start; mintimes[start]=0;
												//initialization done
												for(int checkvert=start;!visd[dest];){
																visd[checkvert]=1;
																for(int i=0;i<Vs;i++){//minimum distance updating for each unvisited node
																				if(!visd[i])
																								if((am->times[checkvert][i]+mintimes[checkvert])<mintimes[i]) mintimes[i]=am->times[pars[i]=checkvert][i]+mintimes[checkvert];
																}
																//going to minimum vertex to check next
																for(int i=0,o=checkvert;i<Vs && o==checkvert;i++) if(!visd[i]) checkvert=i;
																for(int i=0;i<Vs;i++) if(!visd[i] && i!=checkvert) if(mintimes[checkvert]>mintimes[i]) checkvert=i;
												}
												int k=1;
												for(int i=dest;k<Vs && i!=start;k++,i=pars[i]);*/
												order=new int[2]; order[0]=start; order[1]=dest;
												//for(int i=dest,j=k-1;j && !(i==start && pars[i]==start);order[j--]=i,i=pars[i]);//reverse-tracing the path of travel
//												delete [] mintimes; mintimes=0;
//												delete [] visd; visd=0;
//												delete [] pars; pars=0;
								}
								return order;
				}
				~ShortPath(){
								if(mintimes) delete [] mintimes; mintimes=0;
								if(visd) delete [] visd; visd=0;
								if(pars) delete [] pars; pars=0;
				}
};






class Graph{
public:
				AdjMat am;//last 2 indices of adjacency matrix are truck departure Center and landFill
				Bin* bins;
				Place candf[2];//last 2 indices of adjacency matrix are truck departure Center and landFill
				const int Vs;
				bool binsfilled,coll;
				static char gs;
				Graph(int size=12):Vs((size>3)?size:3),am((size>3)?size:3), binsfilled(0), coll(0){
								bins=new Bin[Vs-2];
								printf("\33[36mINITIALISING DATA FOR SET %i\n\n\33[35mName of place\ntime0 time1 time2... time9 centertime landfilltime\n",int(gs));
								gs++; //bool gg=0;
								if(!datain.is_open()){cout<<"BRUHHHHHH\t\n";}
								else{
												string line; int row=0,col=0;
												while (row<Vs) {
																std::getline(datain, line, ',');
																//cout<<"new stringD:\t"<<row<<' '<<col<<'\t'<<line<<"\n";
																// Remove leading and trailing whitespaces
																line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));
																line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);
																// Convert the string to a float
																if(col<Vs){float value = std::stof(line);
																				am.times[row][col++] = value;
																}
																else{	// If string
																				if(row<Vs-2) bins[row] = Bin(line, row, rand()%70+190);
																				else candf[row+2-Vs]=Place(line, row);
																				col=0;row++;
																}
												}
												
												endc();
												display();
								}
				}
				void simulaterun(int i=1){
								for(int j=0;j<i;j++){
												result=time(nullptr);
												bool clct= binsfilled;//fillallbins();
												float tottime=0; int start=Vs-2;
												if(clct){collect(start, tottime);
																if((clct=fillallbins())) collect(start,tottime);
																binsfilled=0,coll=0;
																dispose(start,tottime);
																retrntocntr();// collect returns false if no bins were above threshhold.
																
												}
												else {cout<<"Nothing to collect\n";outt<<"Nothing to collect\n";}
												display();}
				}
				
				void retrntocntr(){
								ShortPath sp(bins, candf, &am);
								int* order =sp.OrderofTravel(Vs-1,Vs-2);//landfill to center
								float tottime=0;
								cout<<"\n\33[42;30mTruck returning from \33[42;37m"<<candf[1].name;
								outt<<"\nTruck returning from "<<candf[1].name;
								if(order){order[0]=Vs-1;
												for(int k=0;order[k]!=Vs-2;k++){
																int from=order[k],
																to=order[k+1];
																float curtime=am.times[from][to];
																tottime+=curtime;
																//printf("Rfrom ID:%i to ID:%i time:%.2f tottime:%.2f",from,to,curtime,tottime);
																cout<<",\33[42;30m then to \33[42;37m"<<((to>Vs-3)?candf[to+2-Vs].name:bins[to].name);
																outt<<", then to "<<((to>Vs-3)?candf[to+2-Vs].name:bins[to].name);
												}
												delete [] order; order=0;
								}
								printf(".\33[0;32m\nTruck returned to center! Total time taken:%.2f mins, at ",tottime);
								outt<<".\nTruck returned to center! Total time taken:"<<fixed<<setprecision(2)<<tottime<<" mins, at ";
								time_t result1=result+tottime*60;
								cout<<asctime(localtime(&result1))<<endc(); outt<<asctime(localtime(&result1))<<"\n\n";
				}
				
				void collect(int& start, float &tottime){//runtime change of start location i.e if bins are filled after there was already a collection done, the collection will continue from the last bin
								bool allvisd=!bins[0].full();
								for(int i=0;i<Vs-2 && allvisd;i++) allvisd=!bins[i].full();
								if (allvisd) return;
								ShortPath sp(bins, candf, &am);
								bool *visd= new bool[Vs];
								if(start==Vs-2) {cout<<"\n\33[42;30mGarbage collection starting from \33[42;37m"<<candf[0].name;
												outt<<"\nGarbage collection starting from "<<candf[0].name;}
								else if(start<Vs-2) {cout<<"\n\33[0;31;1mBIN(S) FILLED DURING PREVIOUS COLLECTION\n\33[42;30mGarbage collection continuing from \33[42;37m"<<bins[start].name;
												outt<<"\nBIN(S) FILLED DURING PREVIOUS COLLECTION\nGarbage collection continuing from "<<bins[start].name;}
								for(int i=0;i<Vs;visd[i++]=(i>Vs-3)? 0: !bins[i].full())//bins below threshhold are marked univisited
												
												//totaltime
											
								for(int i=start;!allvisd;)//visits all visit-needing bins
								{	int closest=0;for(int j=0;j<Vs-2 && visd[j];(!visd[j])? closest=j++:j++)
												;float closestplength=1000000;
												for(int j=closest;j<Vs-2;j++) if(!visd[j] && j!=i) // checking the closest needed-to-visit bin and the shortest path
												{int *order=sp.OrderofTravel(i, j);
																if(order){order[0]=i;
																				float len=0;
																				for(int k=0;order[k]!=j;k++)
																				{int from=order[k], to=order[k+1]; len+=am.times[from][to];} //printf("\33[0m\nTesting from ID:%i to ID:%i tottime:%.2f\n",from,to,len);//adding the time for the path
																				if(len<closestplength) void(closestplength=len), closest=j;
																				delete[] order; order=0;
																}
												}
												int *order=sp.OrderofTravel(i, closest);
												if(order) {order[0]=i;
																for(int k=0;order[k]!=closest;k++) {
																				int from=order[k],
																				to=order[k+1];
																				float curtime=am.times[from][to];
																				tottime+=curtime;//printf("\33[0mAfrom ID:%i to ID:%i time:%.2f tottime:%.2f\n",from,to,curtime,tottime);
																				cout<<",\33[42;30m then to \33[42;37m"<<((to<Vs-2)?bins[to].name:candf[to+2-Vs].name);
																				outt<<", then to "<<((to<Vs-2)?bins[to].name:candf[to+2-Vs].name);
																				if(to<Vs-2) bins[to].lvl=0; visd[to]=1;//emptying all bins on the path
																}
																delete [] order; order=0;
												}
												i=closest;
												allvisd=visd[0];
												
												for(int j=0;j<Vs-2 && allvisd; allvisd=(!visd[j])? !bins[j++].full():++j)
																;
												start=i;
								}
								
								time_t result1=result+tottime*60;
								printf(".\33[0;32;1m\nTime taken so far: %.2f mins, at ", tottime);
								outt<<".\nTime taken so far: "<<fixed<<setprecision(2)<<tottime<<" mins, at "<<asctime(localtime(&result1))<<endl;
								cout<<asctime(localtime(&result1))<<endc();
								delete [] visd;
				}
				bool dispose(int &start, float& tottime){
								cout<<"\n\33[42;30mGarbage Disposal starting from \33[42;37m"<<
								((start<Vs-2)? bins[start].name:candf[start+2-Vs].name);
								outt<<"\nGarbage Disposal starting from "<<
								((start<Vs-2)? bins[start].name:candf[start+2-Vs].name);
								ShortPath sp(bins, candf, &am);
								int* order =sp.OrderofTravel(start, Vs-1);//start currently has the last visited bin's id; truck goes from there to the landfill, taking the shortest path and picking up any level of bins on the way.
								if(order){order[0]=start;
												for(int k=0;k<Vs && order[k]!=Vs-1;k++){
																int from=order[k], to=order[k+1]; float curtime=am.times[from][to];
																tottime+=curtime;
//																printf("Bfrom ID:%i to ID:%i time:%.2f tottime:%.2f",from,to,curtime,tottime);
																cout<<",\33[42;30m then to \33[42;37m"<<((to>Vs-3)?candf[to+2-Vs].name:bins[to].name);
																outt<<", then to "<<((to>Vs-3)?candf[to+2-Vs].name:bins[to].name);
																if(to<Vs-2) if(bins[to].lvl) bins[to].lvl=0;//emptying all bins on the path
												}
												delete [] order; order=0;
								}
								printf("\33[0;32m\nGarbage collected and disposed! Total time taken:%.2f mins, at ",tottime);
								result+=tottime*60;
								outt<<"\nGarbage collected and disposed! Total time taken:"<<fixed<<setprecision(2)<<tottime<<" mins, at "<<asctime(localtime(&result))<<endl;
								cout<<asctime(localtime(&result))<<endc();
								return 1;
								
				}
				
				bool fillallbins(){
								printf("%sfilling all bins\n",endcl); bool anybinfull=0;
								for(int i=0;i<Vs-2;cout<<bins[i++]) bins[i].fill(),void((bins[i].full())? anybinfull=bins[i].full():0);
								binsfilled=anybinfull;
								return anybinfull;
				}
				
				
				void display(){
								for(int i=0;i<Vs;i++) {if(i>Vs-3) cout<<candf[i+2-Vs]; else cout<<bins[i];}
								am.Display();
				}
				~Graph(){;if(bins) delete[] bins;}
};
char Graph::gs=1;


int main() {srand(st);
				Graph g[5];//5 graphs with 12 vertices by default. default constructor inputs the data for the vertices.
				for(int i=0;i<5;g[i++].fillallbins());//argument is the number of hour simulations to run on each graph
				string c="n"; int j=0;
				do{if(c[0]=='y'){
								int user=login();
								
								if(user==100){int coll=0;
												cout<<"Bins full in sectors:"; for(int i=0;i<5;i++) if(g[i].binsfilled){ cout<<i<<", "; coll++;}
												if(coll) {cout<<"Which to collect from? (if sectors 3,2,1 need collection:\"1 2 3\")";
																for(int i;coll--;){cin>>i; if(i<5 && i>=0) if(g[i].binsfilled) g[i].coll=1;}}
												else{cout<<"-";}
												endc();
								}
								if(user<=4 && user>=0){ if(g[user].coll) {string gogo="Controller said go.\n Leave now?(y/other key)";
												cout<<gogo;cin>>gogo;
												if(gogo[0]=='y') g[user].simulaterun();
												else cout<<"Naafarmaan\n";}
								}
								}
								printf("Do you want to login%s? (y/n)", (j++)?" again":"");cin>>c;
								
				}while(c[0]=='y');
				cout<<"Thanks for playing\n";
				datain.close();
				
}
