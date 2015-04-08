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

const int n = 2;					// no. of inputs in a pattern / nodes per hidden layer

double learningRate = 0.8;

#define ITERATIONLIMIT 100000

inline double sigmoid(double x)
{
	return 1.0/(1.0 + exp(-x));
}

int main(){
	ll nInputs = (1LL<<n);
	VVI table(nInputs);
	VI outputs;
	for (ll i = 0; i < nInputs; i++){
		int temp;
		rep(j,n) {
			cin >> temp;
			assert(temp == 0 or temp == 1);
			table[i].pb(temp);
		}
		// rep(j, outputLength) {
			cin >> temp;
			assert(temp == 0 or temp == 1);
			outputs.pb(temp);
		// }
	}


	double error = 0.0;
	int nIterations = 0;

	double w12 = 0.0, w22 = 0.0, w13 = 0.0, w23 = 0.0, wHidden = 0.0;
	double biasHidden = 0.0, biasOutput = 0.0;

	rep(y, ITERATIONLIMIT){
		++nIterations;
		error = 0.0;

		rep(i, nInputs){
			double temp, finalOutput;
			temp = sigmoid(w12*table[i][0] + w22*table[i][1] + biasHidden);
			finalOutput = sigmoid(w13*table[i][0] + w23*table[i][1] + wHidden*temp + biasOutput);
			error += 0.5*(finalOutput - outputs[i])*(finalOutput - outputs[i]);

			double outDelta = (outputs[i] - finalOutput) * finalOutput * (1.0 - finalOutput);
			biasOutput += outDelta*learningRate;

			wHidden += learningRate*outDelta*temp;

			double hiddenDelta = w23*outDelta*temp*(1.0 - temp);
			biasHidden += learningRate*hiddenDelta;

			w13 += learningRate*table[i][0]*outDelta;
			w23 += learningRate*table[i][1]*outDelta;

			w12 += learningRate*table[i][0]*hiddenDelta;
			w22 += learningRate*table[i][1]*hiddenDelta;

		}
	}

	debug(error);
	rep(i, nInputs){
		double temp, finalOutput;
		temp = sigmoid(w12*table[i][0] + w22*table[i][1] + biasHidden);
		finalOutput = sigmoid(w13*table[i][0] + w23*table[i][1] + wHidden*temp + biasOutput);
		debug3(i, outputs[i], finalOutput);
	}

}