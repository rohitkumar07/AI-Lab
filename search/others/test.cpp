#include <bits/stdc++.h>
#include <math.h>
using namespace std;

struct node
{
	int state[3][3];
	node* parent;
	int Gcost, Hcost;
	node(int mat[][3],int gcost, int hcost, node* p){
		/**
		inititialise g, h
		*/
		Gcost = gcost;
		Hcost = hcost;
		parent = p;
		for (int i=0; i<3; i++)
			for (int j=0; j<3; j++)
				state[i][j] = mat[i][j];
	}

};
// remind rohit that he needs to check that the adjacency list has its own parent


bool equal_nodes_tile(node* a,node* b){
	bool to_return = true;
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			to_return = to_return && ( a->state[i][j] == b->state[i][j]);
		}
	}
	return to_return;
}



int displaced_tiles(int current[][3],int goal[][3]){
	int count = 0;
	for(int x=0;x<3;x++){
		for(int y=0;y<3;y++){
			if(current[x][y] != goal[x][y] && current[x][y] != 0) count++;
		}
	}
	return count;
}


int manhattan_distance(int current[][3],int goal[][3]){
	int count = 0;
	for(int x=0;x<3;x++){
		for(int y=0;y<3;y++){
			int p = current[x][y];
			if(p == 0) continue;
			for(int i=0;i<3;i++){
				for(int j=0;j<3;j++){
					if(goal[i][j] == p){
						count = count + abs(i - x) + abs(j - y);
						break;					
					}
				}
			}
			
		}
	}
	return count;
}
//int manhattan_distance()


void giveAdjacencyList(node* &current,vector<node*> &adj, node* &goal){
	
	int presentx,presenty;
	bool found = false;
	for(presentx = 0;presentx < 3; presentx++){
		for(presenty = 0;presenty < 3; presenty++){
			found = (current->state[presentx][presenty] == 0);
			if(found) break;
		}
		if(found) break;
	}
	
	int gcost,hcost;
	
	// to move right
	if(presentx > 0){
		int mat_right[3][3];
		for(int x=0;x<3;x++){
			for(int y=0;y<3;y++){
				if(!(x == presentx && y == presenty) && !(x == (presentx - 1) && y == presenty)) mat_right[x][y] =  current->state[x][y];
				else if (x == presentx && y == presenty){
					//cout << "a" << endl;
					mat_right[x][y] =  current->state[x-1][y];
					mat_right[x-1][y] = 0;

				}
			}
		}
		gcost = current->Gcost + 1;						//for edge cost to be one  
		hcost = displaced_tiles(mat_right,goal->state);							// assuming heuristic to be displaced_tiles
		node* right = new node(mat_right,gcost,hcost,current);
		adj.push_back(right);

	}
	if(presentx < 2){
		int mat_left[3][3];
		for(int x=0;x<3;x++){
			for(int y=0;y<3;y++){
				if(!(x == presentx && y == presenty) && !(x == (presentx + 1) && y == presenty)) mat_left[x][y] =  current->state[x][y];
				else if (x == presentx && y == presenty){
					//cout << "b" <<endl;
					mat_left[x][y] =  current->state[x+1][y];
					mat_left[x+1][y] = 0;

				}
			}
		}
		gcost = current->Gcost + 1;						//for edge cost to be one  
		hcost = displaced_tiles(mat_left,goal->state);							// assuming heuristic to be displaced_tiles
		node* left = new node(mat_left,gcost,hcost,current);
		adj.push_back(left);
	}

	if(presenty > 0){
		int mat_bottom[3][3];
		for(int x=0;x<3;x++){
			for(int y=0;y<3;y++){
				if(!(x == presentx && y == presenty) && !(x == presentx && y == (presenty - 1))) mat_bottom[x][y] =  current->state[x][y];
				else if (x == presentx && y == presenty){
					//cout << "c" <<endl;
					mat_bottom[x][y] =  current->state[x][y - 1];
					mat_bottom[x][y - 1] = 0;

				}
			}
		}
		gcost = current->Gcost + 1;						//for edge cost to be one  
		hcost = displaced_tiles(mat_bottom,goal->state);							// assuming heuristic to be displaced_tiles
		node* bottom = new node(mat_bottom,gcost,hcost,current);
		adj.push_back(bottom);
	}

	if(presenty < 2){
		int mat_top[3][3];
		for(int x=0;x<3;x++){
			for(int y=0;y<3;y++){
				if(!(x == presentx && y == presenty) && !(x == presentx && y == (presenty + 1))) mat_top[x][y] =  current->state[x][y];
				else if (x == presentx && y == presenty){
					//cout << "d" <<endl;
					
					mat_top[x][y] =  current->state[x][y + 1];
					mat_top[x][y + 1] = 0;

				}
			}
		}
		gcost = current->Gcost + 1;						//for edge cost to be one  
		hcost = displaced_tiles(mat_top,goal->state);							// assuming heuristic to be displaced_tiles
		node* top = new node(mat_top,gcost,hcost,current);
		adj.push_back(top);
	}			
	return;
}

void print_matrix(int mat[][3]){
	cout << endl;
	for(int i=0; i< 3; i++){
		for(int j=0; j< 3; j++){
			cout << mat[i][j] << " " ;
		}
		cout << endl;
	}
	cout << endl;
}


int main(){

	int a[9] = {1,2,3,4,5,6,7,8,0};
		
	int t = 10;
	cout << t << endl;
	for (int u = 0; u < t; u++){
		random_shuffle(a, a+9);

		for (int i = 0; i < 9; i++) cout << a[i] << " ";
		cout << endl;
	}
	/*
	int mat[3][3],mat1[3][3];
	mat[0][0] = 0;
	mat[0][1] = 1;
	mat[0][2] = 2;
	mat[1][0] = 3;
	mat[1][1] = 4;
	mat[1][2] = 5;
	mat[2][0] = 6;
	mat[2][1] = 7;
	mat[2][2] = 8;
	print_matrix(mat);
	
	mat1[0][0] = 1;
	mat1[0][1] = 0;
	mat1[0][2] = 2;
	mat1[1][0] = 3;
	mat1[1][1] = 4;
	mat1[1][2] = 5;
	mat1[2][0] = 6;
	mat1[2][1] = 7;
	mat1[2][2] = 8;
	
	
	node* x = NULL;
	node* q = new node(mat,0,0,x);
	node* r = new node(mat1,0,0,x);
	
	vector<node*> adj;
	giveAdjacencyList(q,adj,r);
	for(int i = 0; i< adj.size(); i++){
		print_matrix(adj[i]->state);
	}
	*/
	
}
