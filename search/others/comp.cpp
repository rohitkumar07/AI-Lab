#include <bits/stdc++.h>
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
	bool operator<(const node* &x)const{
		for (int i=0; i<3; i++)
			for (int j=0; j<3; j++){
				if (state[i][j] == x->state[i][j]) continue;
				else return (state[i][j] < x->state[i][j]);
			}
		return true;
	}
	bool operator==(const node* &x)const{
		for (int i=0; i<3; i++)
			for (int j=0; j<3; j++)
				if (state[i][j] != x->state[i][j]) return false;
		return true;
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

int main(){
	//int mat[3][3],mat1[3][3];
	int mat[3][3] = {	{0,1,2},
						{3,4,5},
						{6,7,8}};
	
	
	int mat1[3][3] = {	{3,1,2},
						{0,4,5},
						{6,7,8}};

	int mat2[3][3] = {	{3,1,2},
						{0,4,5},
						{6,7,8}};

	node* x = new node(mat, 0, 0, NULL);
	node* y = new node(mat1, 0, 0, NULL);
	node* z = new node(mat2, 0, 0, NULL);
	set<node*, compare> Q;
	Q.insert(x);
	Q.insert(z);
	Q.insert(y);
	if (Q.find(z) != Q.end()) cout << "mil gaya\n";
	else cout << "nahi mila\n";
}