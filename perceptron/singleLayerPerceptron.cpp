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
#define VI vector<int> 
#define VD vector<double> 
#define VVI vector<VI > 

// Print Macros

double sigmoid(double x)
{
	return 1.0/(1.0 + exp(-x));
}

int main(){
	int n;
	cout << "Enter no. of variables in truth table: ";
	cin >> n;
	ll nInputs = (1LL<<n);
	VVI table(nInputs);
	VI outputs(nInputs);
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
		table[i].pb(-1);
		while(1){
			int temp;
			cin >> temp;
			if (temp == 0 || temp == 1) {
				outputs[i] = temp;
				// if (temp == 0){
				// 	rep(t, table[i].size()){
				// 		table[i][t] *= -1;
				// 	}
				// }
				break;
			}
			cout << "Invalid output of truth table. Enter again!\n";
		}
	}

	VD weight(n + 1, 0);  // Initial weights

	int nIterations = 0;

	double learningRate = 0.9;


	rep(y,100000){
		++nIterations;

		// double 

		double dot = 0;
		// rep(i, n + 1){
		// 	dot += weight[i]*table[currentRow][i];
		// }	

		// double observed = sigmoid(dot);

		for (ll i = 0; i < nInputs; i++){
			dot = 0;
			rep(j, n + 1){
				dot += weight[j]*table[i][j];
			}
			double observed = sigmoid(dot);

			double coeff = learningRate * (outputs[i] - observed) * observed * (1.0 - observed); 
			rep(j, n+1){
				weight[j] += coeff * table[i][j];
			}
		}

		double error = 0;

		for (ll i = 0; i < nInputs; i++){
			dot = 0;
			rep(j, n+1){
				dot += weight[j]*table[i][j];
			}
			double o = sigmoid(dot);
			double t = outputs[i];
			// debug3(i, o, t);
			error += (t - o)*(t - o)/2.0;
		}
		
		if (error < 0.1) break;


		// if (dot > 0) currentRow++;
		// else{
		// 	rep(i, n+1){
		// 		weight[i] += table[currentRow][i];
		// 	}
		// 	currentRow = 0;
		// }
		// if (currentRow == nInputs) break; // success
	}

	cout << "Printing weights:: \n";
	rep(i, n){
		cout << weight[i] << " ";
	}
	cout << "\n";
	debug(nIterations);
	cout << "Value of theta is: " << weight[n] << endl;
}
