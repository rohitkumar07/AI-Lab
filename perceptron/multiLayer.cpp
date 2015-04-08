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

#define COMPUTATIONALLAYERS 2 			// No. of input + hidden layers
// #define HNODES 2						// 

const int n = 2;					// no. of inputs in a pattern / nodes per hidden layer
const int outputLength = 1;		// no. of outputs in a pattern

/* weight of edge of jth node in ith layer to 
 * kth node in (i+1)th layer
 * On the basis of fully feed forward network
 */

double edgeWeights[COMPUTATIONALLAYERS - 1][n][n];
double outputWeights[n][outputLength];		// last hidden layer to output layer

double learningRate = 0.7;

#define ITERATIONLIMIT 100

inline double sigmoid(double x)
{
	return 1.0/(1.0 + exp(-x));
}

int main(){
	// cout << "Enter no. of variables in truth table: ";
	// cin >> n >> outputLength;
	ll nInputs = (1LL<<n);
	VVI table(nInputs);
	VVI outputs(nInputs);
	for (ll i = 0; i < nInputs; i++){
		int temp;
		rep(j,n) {
			cin >> temp;
			assert(temp == 0 or temp == 1);
			table[i].pb(temp);
		}
		table[i].pb(-1);
		rep(j, outputLength) {
			cin >> temp;
			assert(temp == 0 or temp == 1);
			outputs[i].pb(temp);
		}
	}

	double intermediateValues[COMPUTATIONALLAYERS][n];
	double finalOutputs[outputLength];
	double outputLayerDeviation[outputLength];
	double innerLayerDeviation[n];

	int nIterations = 0;
	rep(iterNum, ITERATIONLIMIT){
		++nIterations;

		double dot = 0;
		// rep(i, n + 1){
		// 	dot += weight[i]*table[currentRow][i];
		// }	

		// double observed = sigmoid(dot);

		for (ll i = 0; i < nInputs; i++){
			
			/*
			 * Feeding inputs to input layer
			 */
			rep(j,n){
				intermediateValues[0][j] = table[i][j];
			}

			// Intermediate output calculation
			forn(j, 1, COMPUTATIONALLAYERS - 1){
				rep(k, n){
					double tempOutput = 0;
					rep(t, n){
						tempOutput += edgeWeights[j-1][t][k] * intermediateValues[j-1][t];
					}
					intermediateValues[j][k] = sigmoid(tempOutput);
				}
			}

			// Final output calculation
			rep(j, outputLength){
				double tempOutput = 0;
				rep(t, n){
					tempOutput += outputWeights[t][j] * intermediateValues[COMPUTATIONALLAYERS - 1][t];
				}
				finalOutputs[j] = sigmoid(tempOutput);
			}
			// Output calculation done


			// deviation (delta) for output layer
			rep(j, outputLength){
				outputLayerDeviation[j] = (outputs[i][j] - finalOutputs[j])*finalOutputs[j]*(1.0 - finalOutputs[j]);
			}


			// weight updation for output layer
			rep(j, outputLength){
				rep(t, n){
					outputWeights[t][j] += learningRate*outputLayerDeviation[j]*intermediateValues[COMPUTATIONALLAYERS - 1][t];
				}
			}

			// Backpropagation and deviation for inner layers

			forb(j, COMPUTATIONALLAYERS - 2, 0);



		// 	dot = 0;
		// 	rep(j, n + 1){
		// 		dot += weight[j]*table[i][j];
		// 	}
		// 	double observed = sigmoid(dot);

		// 	double coeff = learningRate * (outputs[i] - observed) * observed * (1.0 - observed); 
		// 	rep(j, n+1){
		// 		weight[j] += coeff * table[i][j];
		// 	}
		// }

		// double error = 0;

		// for (ll i = 0; i < nInputs; i++){
		// 	dot = 0;
		// 	rep(j, n+1){
		// 		dot += weight[j]*table[i][j];
		// 	}
		// 	double o = sigmoid(dot);
		// 	double t = outputs[i];
		// 	// debug3(i, o, t);
		// 	error += (t - o)*(t - o)/2.0;
		// }
		
		// if (error < 0.1) break;


		// if (dot > 0) currentRow++;
		// else{
		// 	rep(i, n+1){
		// 		weight[i] += table[currentRow][i];
		// 	}
		// 	currentRow = 0;
		}
		// if (currentRow == nInputs) break; // success
	}

	cout << "Printing weights:: \n";
	rep(i, n){
		// cout << weight[i] << " ";
	}
	cout << "\n";
	debug(nIterations);
	// cout << "Value of theta is: " << weight[n] << endl;
}
