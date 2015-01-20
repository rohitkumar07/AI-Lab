#include <bits/stdc++.h>
using namespace std;

struct node
{
	int state[3][3];
	node* parent;
	int Gcost, Hcost;
	node(int mat[][3],int gcost, int hcost, node* p){
		Gcost = gcost;
		Hcost = hcost;
		parent = p;
		for (int i=0; i<3; i++)
			for (int j=0; j<3; j++)
				state[i][j] = mat[i][j];
	}
};

struct compare {
    bool operator() (const node* x, const node* y) const{
        for (int i=0; i<3; i++){
			for (int j=0; j<3; j++){
				if (y->state[i][j] == x->state[i][j]) continue;
				else return (x->state[i][j] < y->state[i][j]);
			}
		}
		return false;
    }
};

bool equal_node(node* &x, node* &y){
		for (int i=0; i<3; i++)
			for (int j=0; j<3; j++)
				if (y->state[i][j] != x->state[i][j]) return false;
		return true;
}

int displaced_tiles(int current[][3],int goal[][3]){
	int count = 0;
	for(int x=0;x<3;x++){
		for(int y=0;y<3;y++){
			if(current[x][y] != goal[x][y] and current[x][y] != 0) count++;
		}
	}
	return count;
}

int invct(vector<int> &arr, int start, int end){
    if (start >= end) return 0;
    int mid = start + (end - start)/2;
    int a = invct(arr,start, mid) + invct(arr, mid +1, end);
    int res = 0;
    int index1 = start, index2 = mid+1;
    while(true){
        if (index1 > mid || index2 > end) break;
        if (arr[index1] <= arr[index2])
            index1++;
        else {
            index2++;
            res+=(mid-index1+1);
            }
    }
    sort(arr.begin()+start, arr.begin() + end + 1);
    return res + a;
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

int linear_conflict(int current[][3], int goal[][3]){
	int count = 0;

	for (int i=0; i<3; i++){
		for (int j=0; j<2; j++){
			for(int k=j+1; k<3; k++){
				int p,l, m,n;
				bool found = false;
				if (current[i][j] == 0 || current[i][k] == 0) continue;
				for (p = 0; p < 3; p++){
					for ( l = 0; l < 3; l++){
						if (goal[p][l] == current[i][j]){
							found = true;
							break;
						}
					}
					if (found) break;
				}
				found = false;
				for (m = 0; m < 3; m++){
					for ( n = 0; n < 3; n++){
						if (goal[m][n] == current[i][k]){
							found = true;
							break;
						}
					}
					if (found) break;
				}
				if (p == m && l > n) count++;
			}
		}
	}
	return count;
}


int heuristic_distance(int current[][3],int goal[][3]){
	//return 0;

	return manhattan_distance(current, goal) ;
	//return  manhattan_distance(current, goal) + linear_conflict(current, goal);;
	//return displaced_tiles(current,goal);
}

void giveAdjacencyList(node* &current,vector<node*> &adj, node* &goal){
	adj.clear();
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
					mat_right[x][y] =  current->state[x-1][y];
					mat_right[x-1][y] = 0;

				}
			}
		}
		gcost = current->Gcost + 1;						//for edge cost to be one  
		hcost = heuristic_distance(mat_right,goal->state);							// assuming heuristic to be displaced_tiles
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
		hcost = heuristic_distance(mat_left,goal->state);							// assuming heuristic to be displaced_tiles
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
		hcost = heuristic_distance(mat_bottom,goal->state);							// assuming heuristic to be displaced_tiles
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
		hcost = heuristic_distance(mat_top,goal->state);							// assuming heuristic to be displaced_tiles
		node* top = new node(mat_top,gcost,hcost,current);
		adj.push_back(top);
	}
	
	
				
	return;
}

void print_matrix(int mat[][3]){
	for(int i=0; i< 3; i++){
		for(int j=0; j< 3; j++){
			cout << mat[i][j] << " " ;
		}
		cout << endl;
	}
	cout << endl;
}

int expandedNodes = 0;

int aStar(node* &start, node* &goal){

	// Data Structures
	multimap<int, node*> openlistStart, openlistGoal;
	set<node*, compare> closedlistStart, closedlistGoal;// all the nodes which have been expanded
	map<node*, map<int,node*>:: iterator, compare> openlistMapStart, openlistMapGoal;

	// Iterators
	map<int, node*> :: iterator itr;
	map<node*, multimap<int,node*>:: iterator>::iterator searchItr;
	set<node*>::iterator setItr;

	// Inserting Start Node
	itr = openlistStart.insert(multimap<int, node*>::value_type(start->Gcost + start->Hcost,  start));
	openlistMapStart.insert(map<node*, multimap<int,node*>::iterator >::value_type(start, itr));

	// Inserting Goal Node
	itr = openlistGoal.insert(multimap<int, node*>::value_type(goal->Gcost + goal->Hcost,  goal));
	openlistMapGoal.insert(map<node*, multimap<int,node*>::iterator >::value_type(goal, itr));

	node* current; 
	vector<node*> adj;

	while(!openlistStart.empty() && !openlistGoal.empty()){
		//cout << openlist.size() <<  " here1\n";

		// Forward Search

		itr = openlistStart.begin();
		current = (*itr).second;

		setItr = closedlistGoal.find(current);

		if (setItr != closedlistGoal.end()){
			expandedNodes = closedlistGoal.size() + closedlistStart.size();
			return current->Gcost + (*setItr)->Gcost;
		}

		openlistStart.erase(itr);
		searchItr = openlistMapStart.find(current);
		openlistMapStart.erase(searchItr);

		closedlistStart.insert(current);

		//cout <<  adj.size() <<" here1\n";
		//vector<node*> adj;
		giveAdjacencyList(current, adj, goal);
		// void giveAdjacencyList(node* &current,vector<node*> &adj, node* &goal)

		for (int i = 0; i < adj.size(); i++){ // expanding the current node 
			node* next = adj[i];

			if(closedlistStart.find(next) != closedlistStart.end()) continue; // continue if present in closedlist

			searchItr = openlistMapStart.find(next);
			if (searchItr != openlistMapStart.end()){
		//		cout << "present in open list\n";
				if (searchItr->first->Gcost + searchItr->first->Hcost > next->Gcost + next->Hcost)
				{
		//			cout << "updated\n";
					openlistStart.erase(searchItr->second);
					openlistMapStart.erase(searchItr);
					itr = openlistStart.insert(multimap<int, node*>::value_type(next->Gcost + next->Hcost,  next));
					openlistMapStart.insert(map<node*, multimap<int,node*>::iterator >::value_type(next, itr));
				}
			}
			else{
		//		cout << "not in open list\n" ;
				itr = openlistStart.insert(multimap<int, node*>::value_type(next->Gcost + next->Hcost,  next));
				openlistMapStart.insert(map<node*, multimap<int,node*>::iterator >::value_type(next, itr));
			}
		}

	//---------------------------------------------------------------------------------------
	// Backward Search

		itr = openlistGoal.begin();
		current = (*itr).second;

		setItr = closedlistStart.find(current);

		if (setItr != closedlistStart.end()){
			expandedNodes = closedlistGoal.size() + closedlistStart.size();
			return current->Gcost + (*setItr)->Gcost;
		}

		openlistGoal.erase(itr);
		searchItr = openlistMapGoal.find(current);
		openlistMapGoal.erase(searchItr);

		closedlistGoal.insert(current);

		giveAdjacencyList(current, adj, start);
		// void giveAdjacencyList(node* &current,vector<node*> &adj)

		for (int i = 0; i < adj.size(); i++){ // expanding the current node 
			node* next = adj[i];

			if(closedlistGoal.find(next) != closedlistGoal.end()) continue; // continue if present in closedlist

			searchItr = openlistMapGoal.find(next);
			if (searchItr != openlistMapGoal.end()){
		//		cout << "present in open list\n";
				if (searchItr->first->Gcost + searchItr->first->Hcost > next->Gcost + next->Hcost)
				{
		//			cout << "updated\n";
					openlistGoal.erase(searchItr->second);
					openlistMapGoal.erase(searchItr);
					itr = openlistGoal.insert(multimap<int, node*>::value_type(next->Gcost + next->Hcost,  next));
					openlistMapGoal.insert(map<node*, multimap<int,node*>::iterator >::value_type(next, itr));
				}
			}
			else{
		//		cout << "not in open list\n" ;
				itr = openlistGoal.insert(multimap<int, node*>::value_type(next->Gcost + next->Hcost,  next));
				openlistMapGoal.insert(map<node*, multimap<int,node*>::iterator >::value_type(next, itr));
			}
		}

	}
	return -1;
}



int main(){
	/*
	int mat[3][3] = {	{1,2,3},
						{0,5,6},
						{4,7,8}};
	
	int mat1[3][3] = {	{1,2,3},
						{4,5,6},
						{7,8,0}};
	//cout << manhattan_distance(mat, mat1);
	*/
	

	// Bidirectional Gives a suboptimal path :::
	int mat[3][3] = {	{8,7,6},
						{1,0,5},
						{2,3,4}};

	int mat1[3][3] = {	{1,2,3},
						{8,0,4},
						{7,6,5}};

	
	
	node* start = new node(mat,0, heuristic_distance(mat, mat1),NULL);
	node* goal = new node(mat1,0, heuristic_distance(mat1, mat),NULL);
	int optimalCost = aStar(start, goal);
	if (optimalCost != -1) {
		cout << "Path Found!\n";
		cout << "Optimal cost: " << optimalCost << endl;
		cout << "No. of expanded nodes: " << expandedNodes << endl;
	}
	else {
		cout << "Path Not Found!\n";
		cout << "No. of expanded nodes: " << expandedNodes << endl;
	}
	/*
	vector<node*> adj;
	giveAdjacencyList(q,adj,r);
	for(int i = 0; i< adj.size(); i++){
		print_matrix(adj[i]->state);
	}
	*/
}
