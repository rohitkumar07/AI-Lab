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
	bool operator==(const node* &x)const{
		for (int i=0; i<3; i++)
			for (int j=0; j<3; j++)
				if (state[i][j] != x->state[i][j]) return false;
		return true;
	}

	bool operator<(const node* &x)const{
		for (int i=0; i<3; i++)
			for (int j=0; j<3; j++){
				if (state[i][j] == x->state[i][j]) continue;
				else return (state[i][j] < x->state[i][j]);
			}
			//cout << "a"<<endl;
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
	for(int i=0; i< 3; i++){
		for(int j=0; j< 3; j++){
			cout << mat[i][j] << " " ;
		}
		cout << endl;
	}
	cout << endl;
}

bool aStar(node &start, node &goal){

	multimap<int, node> openlist;
	set<node> closedlist;// all the nodes which have been expanded
	map<node, map<int,node>:: iterator> openlistMap;


	map<int, node> :: iterator itr;
	map<node, multimap<int,node>:: iterator>::iterator searchItr;

	itr = openlist.insert(multimap<int, node>::value_type(start.Gcost + start.Hcost,  start));
	openlistMap.insert(map<node*\, multimap<int,node*>::iterator >::value_type(start, itr));



	//while(!openlist.empty()){
	while(openlist.size() < 10) {
		//cout << openlist.size() <<  " here1\n";
		//cout << openlistMap.size() <<  " here2\n";
		itr = openlist.begin();

		node* current = (*itr).second;
		cout << "printing preferred matrix:\n";
		print_matrix(current->state);

		if (equal_node(current, goal)){
			goal = current;
			return true;
		}

		openlist.erase(itr);
		searchItr = openlistMap.find(current);
		openlistMap.erase(searchItr);

		//cout << openlist.size() <<  " here3\n";
		//cout << openlistMap.size() <<  " here4\n";

		closedlist.insert(current);
		cout << closedlist.size() <<  " here5\n";

		vector<node*> adj;
		giveAdjacencyList(current, adj, goal);
		// void giveAdjacencyList(node* &current,vector<node*> &adj)

		

		for (int i = 0; i < adj.size(); i++){ // expanding the current node 
			node* next = adj[i];
			cout << "printing Adjacency List:\n";
			print_matrix(next->state);
			cout << "printing closed List:\n";
			for (set<node*>:: iterator t = closedlist.begin(); t!=closedlist.end(); t++)
				print_matrix((*t)->state);

			if(closedlist.find(next) != closedlist.end()) {
				cout << "present in closed list\n";
				continue; // continue if present in closedlist
			}
			else cout << "hello\n";

			searchItr = openlistMap.find(next);
			if (searchItr != openlistMap.end()){
				cout << "present in open list\n";
				if (searchItr->first->Gcost + searchItr->first->Hcost > next->Gcost + next->Hcost)
				{
					cout << "updated\n";
					openlist.erase(searchItr->second);
					openlistMap.erase(searchItr);
					itr = openlist.insert(multimap<int, node*>::value_type(next->Gcost + next->Hcost,  next));
					openlistMap.insert(map<node*, multimap<int,node*>::iterator >::value_type(next, itr));
				}
			}
			else{
				cout << "not in open list\n" ;
				itr = openlist.insert(multimap<int, node*>::value_type(next->Gcost + next->Hcost,  next));
				openlistMap.insert(map<node*, multimap<int,node*>::iterator >::value_type(next, itr));
			}
		}
	}
	return false;
}

int main(){
	int mat[3][3] = {	{0,1,2},
						{3,4,5},
						{6,7,8}};
	print_matrix(mat);
	
	int mat1[3][3] = {	{8,7,6},
						{5,4,3},
						{2,1,0}};
	
	
	node* q = new node(mat,0, manhattan_distance(mat, mat1),NULL);
	node* r = new node(mat1,0, 0,NULL);
	if (aStar(q, r)) {
		cout << "mil gaya\n";
		cout << "Optimal cost: " << r->Gcost << endl;
	}
	else cout << "nahi mila\n";
	/*
	vector<node*> adj;
	giveAdjacencyList(q,adj,r);
	for(int i = 0; i< adj.size(); i++){
		print_matrix(adj[i]->state);
	}
	*/
}





