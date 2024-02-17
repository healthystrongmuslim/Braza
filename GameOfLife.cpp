//  Home Task 2
//  Created by Muhammad Musab on 27/08/2023.
/*
Game of Life
	Game of Life (or just “Life”) is not really a game. There’s no winning or losing or destroying your opponent mentally and spiritually. Life is a “cellular automaton” - a system of cells that live on a grid, where they live, die and evolve according to the rules that govern their world.
	Life’s simple, elegant rules give rise to astonishingly complex emergent behavior. It is played on a 2-D grid Each square in the grid contains a cell, and each cell starts the game as either “alive” or “dead”. Play proceeds in rounds. During each round, each cell looks at its 8 immediate neighbors and counts up the number of them that are currently alive. Make a type char 30 x 30 2D grid. Randomly assign active and dead cells. Active cells will have value ‘*’ and dead cell will have value ‘ ’.
	In Above diagram Blue cell is the current cell whereas Red cells are its neighboring cells
	The cell then updates its own liveness according to 4 rules:
	1. Any live cell with 0 or 1 live neighbors becomes dead, because of underpopulation
	2. Any live cell with 2 or 3 live neighbors stays alive, because its neighborhood is just right
	3. Any live cell with more than 3 live neighbors becomes dead, because of overpopulation
	4. Any dead cell with exactly 3 live neighbors becomes alive, by reproduction
	Run your code for infinite rounds and observe the pattern changing.
	And that’s all there is to Life. These 4 rules give rise to some unbelievably complex and beautiful patterns, and an equally unbelievable quantity of analysis by Life devotees intent on discovering new ones.
 */
/* i did that as well as make an adjustable radius
change the dead-alive to 1-0
*/
#include "stdc++.h"
using namespace std;
unsigned short x; //board size
void disp(bool**);
bool rondchkr(int, int, bool**);
void update(bool**);
void dltarr(bool**);

int main(){long double delay;char cont='y';
				cout<<"Enter board size and delay (seconds) :\t\n(default to 9 and 0.06;\tEnter 'y's to continue from default.)";
				try{cin>>x;if(cin.fail() || x<1) throw 1;cin>>delay; if(cin.fail() || delay<0) throw 0;}
				catch(int n){if(n)x=9; delay=0.04; cin.clear();}
				int i=0, j=0; unsigned st=time(0); srand(st);
				bool** bord =new bool* [x];
				for(i=0;i<x; bord[i++]=new bool[x]);
				// assign values to allocated memory
								for (i=0;i<x;i++)
												for (j=0;j<x; bord[i][j++]=rand()%2);
				//				disp(bord);
				cls(); std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
			while(cont=='y'){
								plswait(delay);
								disp(bord); cout<<"Enter 'y' n times for n iterations, Enter other key to quit:\t";
								cin>>cont;
								update(bord);
				}
				std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
				std::chrono::duration<long double, milli> time_put=t2-t1;
				cout << "\n\33[1;44mRun took:\t\t" << time_put.count() << " milliseconds.\33[0m\n";
//				disp(bord);
				//if line 41 (full animation print) is commented, uncomment line 37 and 48 (print initial and final state only), and vice versa
				dltarr(bord);
}


void disp(bool** A){
				printf("\e[H");
				for(int i=0; i<x; cout<<i++<<endl)
								for(int j=0; j<x; cout<<"\33["<<((A[i][j++])?41:104)<<"m   \33[0m");//41 for red dead cells, 104 for bright blue living cells
				for(int i=0; i<x; cout<<((i>9)? " ":"  ")<<i++);
				cout<<endl;
}

bool rondchkr(int i, int j, bool** A){
				short livn=0, y=0,z=0,r=1,chk;//y and z check offset around cell, r is "radius" to neighbour,
				chk=4*r*(r+1);///chk to check the same *proportion* of neighbours are alive as in the original game of life but with this radius
				for(y=-r;y< r+1;y++)
								for(z=-r;z< r+1;z++) ((y==0) && (z==0))? i: livn += !(A[x*((i+y)<0)+ i+y -((i+y)>(x-1))*x][((j+z)<0)*x+j+z-((j+z)>(x-1))*x]); //cells everywhere have same number of neighbours; the board infinitely neighbours itself
				/*{(((i+y) <0) || ((i+y) > (x-1)) || ((j+z) <0) || ((j+z) >(x-1)) || ((y==0) && (z==0)))? i: livn += (A[i+y][j+z]==' ');}*/ ///cells on walls of board have fewer neighbours
				
				return (!A[i][j]) ? !((livn<= (3*chk/8)) && (livn>=(2*chk/8))) : !((livn<=(3.25*chk/8)) && (livn >= (2.75*chk/8)));
}


void update(bool** A){
				short i,j;
				bool** bord =new bool* [x];
				for(i=0;i<x; bord[i++]=new bool[x]); //temporary array to store updated values, so original array doesn't change itself while being checked
				for(i=0;i<x;i++)
								for(j=0;j<x; bord[i][j++]=rondchkr(i,j,A));
				for(i=0;i<x;i++)
								for(j=0;j<x; A[i][j++]= bord[i][j]);
				dltarr(bord);
}

void dltarr(bool**A){
				unsigned short i;//deallocate memory
				for (i=0;i<x;delete[] A[i++]);
				delete[] A;
				A=NULL;
}
