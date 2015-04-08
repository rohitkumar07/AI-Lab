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
// #define HNODES 2						

const int n = 2;					// no. of inputs in a pattern / nodes per hidden layer
const int outputLength = 1;		// no. of outputs in a pattern
const int inputLength = n;


/* weight of edge of jth node in ith layer to 
 * kth node in (i+1)th layer
 * On the basis of fully feed forward network
 */

#define INIT 0.001

double edgeWeights[COMPUTATIONALLAYERS - 1][n][n] = {INIT};
double outputWeights[n][outputLength] = {INIT};		// last hidden layer to output layer

double biasWeights[COMPUTATIONALLAYERS][n] = {INIT}; // one for each hidden node
double biasOutputWeights[outputLength] = {INIT}; // one for each hidden node


double learningRate = 0.8;

#define ITERATIONLIMIT 1000000

inline double sigmoid(double x)
{
	return 1.0/(1.0 + exp(-x));
}

int main(){
	// cout << "Enter no. of variables in truth table: ";
	// cin >> n >> outputLength;
	ll nInputs = (1LL<<inputLength);
	VVI table(nInputs);
	VVI outputs(nInputs);
	for (ll i = 0; i < nInputs; i++){
		int temp;
		rep(j,inputLength) {
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

	double intermediateValues[COMPUTATIONALLAYERS][n];	// intermediiate outputs
	double finalOutputs[outputLength];
	double outputLayerDeviation[outputLength];
	double innerLayerDeviation[n][n];




	double error = 0.0;
	int nIterations = 0;

	rep(iterNum, ITERATIONLIMIT){
		++nIterations;

		double dot = 0;
		// rep(i, n + 1){
		// 	dot += weight[i]*table[currentRow][i];
		// }	

		// double observed = sigmoid(dot);

		error = 0.0;

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
					tempOutput += biasWeights[j][k];		// value is always 1
					intermediateValues[j][k] = sigmoid(tempOutput);
				}
			}

			// Final output calculation
			rep(j, outputLength){
				double tempOutput = 0;
				rep(t, n){
					tempOutput += outputWeights[t][j] * intermediateValues[COMPUTATIONALLAYERS - 1][t];
				}
				finalOutputs[j] = sigmoid(tempOutput + biasOutputWeights[j]);
			}
			// Output calculation done

			rep(j, outputLength){
				// debug3(i, outputs[i][j], finalOutputs[j]);
				error += (finalOutputs[j] - outputs[i][j])*(finalOutputs[j] - outputs[i][j])/2.0;
			}

			// ----------------------------------------------------------------------------------------------------

			// deviation (delta) for output layer
			rep(j, outputLength){
				outputLayerDeviation[j] = (outputs[i][j] - finalOutputs[j])*finalOutputs[j]*(1.0 - finalOutputs[j]);
			}


			// weight updation for output layer
			rep(j, outputLength){
				rep(t, n){
					outputWeights[t][j] += learningRate*outputLayerDeviation[j]*intermediateValues[COMPUTATIONALLAYERS - 1][t];
				}
				biasOutputWeights[j] += learningRate*outputLayerDeviation[j]; // value is 1 always
			}

			// Backpropagation and deviation for inner layers

			forb(j, COMPUTATIONALLAYERS - 1, 1){

				// Calculating deviations for the current layer
				rep(k, n){
					
					double tempDeviation = 0;
					if (j == COMPUTATIONALLAYERS - 1){
						rep(t, outputLength){
							tempDeviation += outputWeights[k][t]*outputLayerDeviation[t]*intermediateValues[j][t]*(1.0 - intermediateValues[j][t]);
						}
					}

					else{
						rep(t,n){
							tempDeviation += edgeWeights[j][k][t]*innerLayerDeviation[j+1][t]*intermediateValues[j][t]*(1.0 - intermediateValues[j][t]);
						}
					}

					innerLayerDeviation[j][k] = tempDeviation;

				}

				// Updating edge weights for the inner layers

				rep(k, n){
					rep(t, n){
						edgeWeights[j-1][k][t] += learningRate*innerLayerDeviation[j][t]*intermediateValues[j - 1][k];
					}
					biasWeights[j][k] += learningRate*innerLayerDeviation[j][k];
				}

			}
		}

		// debug(error);

		// if (currentRow == nInputs) break; // success
	}

	cout << "Printing weights:: \n";
	for (ll i = 0; i < nInputs; i++){
			// rep(j, inputLength) cout << table[i][j] << " " ;
			// cout << " :: " ;
			/*
			 * Feeding inputs to input layer
			 */
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
						debug3(t, k , edgeWeights[j-1][t][k]);
					}
					tempOutput += biasWeights[j][k];		// value is always 1
					debug3(j, k , biasWeights[j][k]);

					intermediateValues[j][k] = sigmoid(tempOutput);
				}
			}

			// Final output calculation
			rep(j, outputLength){
				double tempOutput = 0;
				rep(t, n){
					tempOutput += outputWeights[t][j] * intermediateValues[COMPUTATIONALLAYERS - 1][t];
					debug3(t, j , outputWeights[t][j]);

				}
				debug2(j, biasOutputWeights[j]);
				finalOutputs[j] = sigmoid(tempOutput + biasOutputWeights[j]);
			}
			// Output calculation done

			rep(j, outputLength) debug(finalOutputs[j]);
			// cout << endl;
	}

	cout << "\n";
	debug(nIterations);
	debug(error);
	// cout << "Value of theta is: " << weight[n] << endl;
}
