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

	double w11 = 0.0, w12 = 0.0, w21 = 0.0, w22 = 0.0;
	double wo1 = 0.0, wo2 = 0.0;
	double bias1 = 0.0, bias2 = 0.0;
	double biasOutput = 0.0;

	rep(y, ITERATIONLIMIT){
		++nIterations;
		error = 0.0;

		rep(i, nInputs){
			double temp1, temp2, finalOutput;
			temp1 = sigmoid(w11*table[i][0] + w21*table[i][1] + bias1);
			temp2 = sigmoid(w12*table[i][0] + w22*table[i][1] + bias2);
			finalOutput = sigmoid(wo1*temp1 + wo2*temp2 + biasOutput);
			error += 0.5*(finalOutput - outputs[i])*(finalOutput - outputs[i]);

			double outDelta = (outputs[i] - finalOutput) * finalOutput * (1.0 - finalOutput);
			biasOutput += outDelta*learningRate;
			wo1 += learningRate*outDelta*temp1;
			wo2 += learningRate*outDelta*temp2;

			double delta1 = wo1*outDelta*temp1*(1.0 - temp1);
			double delta2 = wo2*outDelta*temp2*(1.0 - temp2);

			bias1 += learningRate*delta1;
			bias2 += learningRate*delta2;

			w11 += learningRate*delta1*table[i][0];
			w12 += learningRate*delta2*table[i][0];

			w22 += learningRate*delta2*table[i][1];
			w21 += learningRate*delta1*table[i][1];
		}
	}

	debug(error);
	rep(i, nInputs){
		double temp1, temp2, finalOutput;
		temp1 = sigmoid(w11*table[i][0] + w21*table[i][1] + bias1);
		temp2 = sigmoid(w12*table[i][0] + w22*table[i][1] + bias2);
		finalOutput = sigmoid(wo1*temp1 + wo2*temp2 + biasOutput);
		debug3(i, outputs[i], finalOutput);
	}

}