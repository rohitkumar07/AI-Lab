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
#define VS vector<string> 
#define VD vector<double> 
#define VVI vector<VI > 

// Print Macros
#define USERINPUT 0

#define MAXGRAPHEMELENGTH 5
#define INPUTBITS 5*MAXGRAPHEMELENGTH
#define OUTPUTBITS 7*MAXGRAPHEMELENGTH

#define COMPUTATIONALLAYERS 2 			// No. of output + hidden layers
#define HNODES 5*OUTPUTBITS //(INPUTBITS + OUTPUTBITS)/2						// 

#define PADDINGBIT 0		// -1 maybe?

/* weight of edge of jth node in ith layer to 
 * kth node in (i+1)th layer
 * On the basis of fully feed forward network
 */

double inputWeights[INPUTBITS][HNODES] = {-0.01};
double outputWeights[HNODES][OUTPUTBITS] = {0.01};		// last hidden layer to output layer
double biasHiddenWeights[HNODES] = {-0.01};			// thresholds of hidden neurons
double biasOutputWeights[OUTPUTBITS] = {0.01};		// thresholds of output neurons


int inputData[40000][INPUTBITS];
int outputData[40000][OUTPUTBITS];

double learningRate = 0.7;

#define ITERATIONLIMIT 50

inline double sigmoid(double x)
{
	return 1.0/(1.0 + exp(-x));
}


VS split_space(string s){
	VS ans;
	int index = 0;
	s = s + " ";
	while(s[index] == ' ') index++;

	string temp = "";
	while(1){
		if(s[index] == ' ') {
			ans.pb(temp);
			temp = "";
			while(s[index] == ' ') index++;
		}
		if (index == s.size()) break;
		temp = temp + s[index++];
	}
	return ans;
}

void remove_extra(string& s){
	string temp = "";

	rep(i, s.size()) if(s[i] >= 'A' and s[i] <= 'Z') temp.push_back(s[i]);

	s = temp;
}

bool capsOnly(string s){
	rep(i, s.size()) if(s[i] < 'A' or s[i] > 'Z') return false;
	return true;
}

#define STATES 75
#define OBSERVATIONS 30
string PHONEMES[STATES];
map<string, int> phonemeMap;

int sizeOfInput[40000];

int main(){
	VS graphemes;
	int lineNum = 0, phonemeNum = 0;

	string line;
	ifstream file;

	// file.open("phonemes.txt");
	// while(getline(file, line)){
	// 	remove_extra(line);
	// 	if (line != ""){
	// 		phonemeMap[line] = phonemeNum;
	// 		PHONEMES[phonemeNum] = line;
	// 		phonemeNum++;
	// 	}
	// }
	// file.close();
	file.open("con.txt");

	while(getline(file,line)){
		
		graphemes = split_space(line);

		if (!capsOnly(graphemes[0])) continue;
		// remove_extra(graphemes[0]);

		if(graphemes[0].size() != graphemes.size() - 1)	continue;	// Inconsistent data
		if(graphemes[0].size() > MAXGRAPHEMELENGTH)	continue;	// Inconsistent data

		// data.push_back(graphemes);

		int gLength = graphemes[0].size();
		sizeOfInput[lineNum] = gLength;
		rep(i, MAXGRAPHEMELENGTH){
			if (i < gLength){
				int index = int(graphemes[0][i] - 'A') + 1;
				rep(j, 5){
					inputData[lineNum][(i*gLength + j)] = (index&1);
					index>>=1;
				}
			}
			else{
				rep(j, 5){
					inputData[lineNum][(i*gLength + j)] = PADDINGBIT;
				}
			}
		}

		int pLength = graphemes.size() - 1;

		for (int i = 1; i < graphemes.size(); i++){
			map<string, int> :: iterator itr;
			itr = phonemeMap.find(graphemes[i]);
			if (itr == phonemeMap.end()) {
				phonemeMap[graphemes[i]] = phonemeNum;
				PHONEMES[phonemeNum] = graphemes[i];
				phonemeNum++;
			}
		}

		rep(i, MAXGRAPHEMELENGTH){
			if (i < pLength){
				map<string, int> :: iterator itr;
				itr = phonemeMap.find(graphemes[i+1]);
				// if (itr == phonemeMap.end()) {
				// 	cerr << "ABORT\n";
				// }
				int index = itr->second + 1;
				// if (lineNum < 10){
				// 	cout << graphemes[i+1] << endl;
				// 	cout << phonemeMap[graphemes[i+1]] << endl;
				// 	cout << PHONEMES[phonemeMap[graphemes[i+1]]] << endl;
				// 	cout << endl;
				// }
				rep(j, 7){
					outputData[lineNum][(i*pLength + j)] = (index&1);
					index>>=1;
				}
			}
			else{
				rep(j, 7){
					outputData[lineNum][(i*pLength + j)] = PADDINGBIT;
				}
			}	
		}

		++lineNum;
	}
	file.close();
	debug(lineNum);
	double inputValues[INPUTBITS];
	double intermediateValues[HNODES];
	double finalOutputs[OUTPUTBITS];

	double outputLayerDeviation[OUTPUTBITS];
	double hiddenLayerDeviation[HNODES];

	int nIterations = 0;
	
	rep(iterNum, ITERATIONLIMIT){
		if (iterNum%10 == 0) debug(iterNum);
		++nIterations;

		double dot = 0;
		// rep(i, n + 1){
		// 	dot += weight[i]*table[currentRow][i];
		// }	

		// double observed = sigmoid(dot);

		for (ll i = 0; i < lineNum; i++){
			// if (i%1000 == 0) debug2(iterNum, i);	
			
			// Feeding inputs to input layer
			
			rep(j,INPUTBITS){
				inputValues[j] = inputData[i][j];
			}

			// Intermediate output calculation

			rep(j, HNODES){
				double tempOutput = 0;
				rep(t, INPUTBITS){
					tempOutput += inputWeights[t][j] * inputValues[t];
				}
				tempOutput -= biasHiddenWeights[j];
				intermediateValues[j] = sigmoid(tempOutput);
			}

			
			// Final output calculation
			rep(j, OUTPUTBITS){
				double tempOutput = 0;
				rep(t, HNODES){
					tempOutput += outputWeights[t][j] * intermediateValues[t];
				}
				tempOutput -= biasOutputWeights[j];
				finalOutputs[j] = sigmoid(tempOutput);
			}
			// Output calculation done

			
			// deviation (delta) for output layer
			rep(j, OUTPUTBITS){
				outputLayerDeviation[j] = (outputData[i][j] - finalOutputs[j])*finalOutputs[j]*(1.0 - finalOutputs[j]);
			}

			// weight updation for output layer
			rep(j, OUTPUTBITS){
				rep(t, HNODES){
					outputWeights[t][j] += learningRate*outputLayerDeviation[j]*intermediateValues[t];
				}
				biasOutputWeights[j] += -learningRate*outputLayerDeviation[j];
			}

			/*
			// Backpropagation and deviation for inner layers

			forb(j, COMPUTATIONALLAYERS - 2, 0);
			*/


				// Calculating deviations for the current layer
				
			rep(j, HNODES){
				
				double tempDeviation = 0;
				
				rep(t, OUTPUTBITS){
					tempDeviation += outputWeights[j][t]*outputLayerDeviation[t]*intermediateValues[j]*(1.0 - intermediateValues[j]);
				}

				hiddenLayerDeviation[j] = tempDeviation;

			}

			// Updating edge weights for the inner layers

			rep(j, HNODES){
				rep(t, INPUTBITS){
					inputWeights[t][j] += learningRate*hiddenLayerDeviation[j]*inputValues[j];
				}
				biasHiddenWeights[j] += -learningRate*hiddenLayerDeviation[j];
			}

		}
	}
	cout << "Training done!\n";
	/*
	cout << "Printing weights:: \n";
	rep(i, n){
		// cout << weight[i] << " ";
	}
	cout << "\n";
	debug(nIterations);
	// cout << "Value of theta is: " << weight[n] << endl;
	*/	

	if (USERINPUT){
		while(1){
			string inputGrapheme;
			cin >> inputGrapheme;


			int gLength = inputGrapheme.size();
			rep(i, MAXGRAPHEMELENGTH){
				if (i < gLength){
					int index = int(inputGrapheme[i] - 'A') + 1;
					rep(j, 5){
						inputValues[(i*gLength + j)] = (index&1);
						index>>=1;
					}
				}
				else{
					rep(j, 5){
						inputValues[(i*gLength + j)] = PADDINGBIT;
					}
				}
			}

			rep(j, HNODES){
				double tempOutput = 0;
				rep(t, INPUTBITS){
					tempOutput += inputWeights[t][j] * inputValues[t];
				}
				tempOutput -= biasHiddenWeights[j];
				intermediateValues[j] = sigmoid(tempOutput);
			}

			
			// Final output calculation
			rep(j, OUTPUTBITS){
				double tempOutput = 0;
				rep(t, HNODES){
					tempOutput += outputWeights[t][j] * intermediateValues[t];
				}
				tempOutput -= biasOutputWeights[j];
				finalOutputs[j] = sigmoid(tempOutput);
			}
			// Output calc
			debug("here");

			rep(i, gLength){
				double index = 0;
				rep(j, 7){
					// if (finalOutputs[i*gLength + j] > 0.5)
					// 	finalOutputs[i*gLength + j] = 1;
					// else finalOutputs[i*gLength + j] = 0;

					index = index*2.0 + finalOutputs[i*gLength + j];
				}
				int phonemeIndex = index;
				if (phonemeIndex - 1 < 71 and phonemeIndex > 0){
					cout << PHONEMES[phonemeIndex - 1] << " ";
				}
				else{
					cout << PHONEMES[1] << " ";
				}
			}
			cout << endl;
		}
	}

	else{
		int matches[5], total[5];
		rep(i,5) {matches[i] = 0; total[i] = 0;}
		rep(iter,lineNum){
			int curr = iter%5;
			int gLength = sizeOfInput[iter];

			rep(i, MAXGRAPHEMELENGTH){
				if (i < gLength){
					int index = int(inputData[iter][i] - 'A') + 1;
					rep(j, 5){
						inputValues[(i*gLength + j)] = (index&1);
						index>>=1;
					}
				}
				else{
					rep(j, 5){
						inputValues[(i*gLength + j)] = PADDINGBIT;
					}
				}
			}

			rep(j, HNODES){
				double tempOutput = 0;
				rep(t, INPUTBITS){
					tempOutput += inputWeights[t][j] * inputValues[t];
				}
				tempOutput -= biasHiddenWeights[j];
				intermediateValues[j] = sigmoid(tempOutput);
			}

			
			// Final output calculation
			rep(j, OUTPUTBITS){
				double tempOutput = 0;
				rep(t, HNODES){
					tempOutput += outputWeights[t][j] * intermediateValues[t];
				}
				tempOutput -= biasOutputWeights[j];
				finalOutputs[j] = sigmoid(tempOutput);
			}
			// Output calc
			// debug("here");

			rep(i, gLength){
				double index = 0;
				int idealIndex = 0;
				
				rep(j, 7){
					// if (finalOutputs[i*gLength + j] > 0.5)
					// 	finalOutputs[i*gLength + j] = 1;
					// else finalOutputs[i*gLength + j] = 0;

					index = index*2.0 + finalOutputs[i*gLength + j];
					idealIndex = idealIndex*2 + outputData[iter][i*gLength + j];
				}

				int phonemeIndex = index;
				// if (phonemeIndex - 1 < 71 and phonemeIndex > 0){
					// cout << PHONEMES[phonemeIndex - 1] << " ";
					if (abs(index - idealIndex) < 1) matches[curr]++;
				// }
					total[curr]++;

			}
		}
		rep(i,5){
			cout << "Match percentage for iteration  " << i << " : " << 100*double(matches[i])/double(total[i]) << endl;
		 }
	}

}
