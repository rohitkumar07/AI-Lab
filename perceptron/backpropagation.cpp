#include <bits/stdc++.h>

using namespace std;

#define DBG 1
#define debug(x) if (DBG) {cerr << #x << " : " << x << endl;}
#define debug2(x, y) if (DBG) {cerr << #x << " : " << x << " :: " << #y << " : " << y << endl;}
#define debug3(x, y, z) if (DBG) {cerr << #x << " : " << x << " :: " << #y << " : " << y << " :: " << #z << " : " << z << endl;}
#define rep(i,n) for(int i=0; i<(n); i++)
#define forn(i,a,b) for(int i=(a); i<=(b); i++)
#define forb(i,a,b) for(int i=(a); i>=(b); i--)
#define ll long long
#define pb push_back
#define VS vector<string> 
#define VI vector<int> 
#define VVS vector<VS > 
#define VVI vector<VI > 

// Print Macros



int main(){
	int n;
	cout << "Enter no. of variables in truth table: ";
	cin >> n;
	ll nInputs = (1LL<<n);
	VVI table(nInputs);
	for (ll i = 0; i < nInputs; i++){
		ll temp = i;
		stack<int> rowNum;
		while(temp > 0){
			rowNum.push(temp&1);
			temp>>=1;
		}

		cout << "Enter output for " ;		
		rep(j, n - rowNum.size()) {
			table[i].pb(0);
			cout << "0 ";
		}

		while(!rowNum.empty()){
			table[i].pb(rowNum.top());
			cout << rowNum.top() << " ";
			rowNum.pop();
		}
		cout << " : ";
		table[i].pb(-1);	// for theta

		while(1){
			int temp;
			cin >> temp;
			if (temp == 0 || temp == 1) {
				if (temp == 0){
					rep(t, table[i].size()){
						table[i][t] *= -1;
					}
				}
				break;
			}
			cout << "Invalid output of truth table. Enter again!\n";
		}
	}

	VI weight(n+1, 0);  // Initial weights

	int currentRow = 0, nIterations = 0;
	
	while(1){
		++nIterations;
		int dot = 0;
		rep(i, n+1){
			dot += weight[i]*table[currentRow][i];
		}
		if (dot > 0) currentRow++;
		else{
			rep(i, n+1){
				weight[i] += table[currentRow][i];
			}
			currentRow = 0;
		}
		if (currentRow == nInputs) break; // success
	}
	cout << "Printing weights:: \n";
	rep(i, n){
		cout << weight[i] << " ";
	}
	cout << "\n";
	debug(nIterations);
	cout << "Value of theta is: " << weight[n] << endl;
}