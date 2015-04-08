#include <bits/stdc++.h>

using namespace std;

#define DBG 0
#define debug(x) if (DBG) {cerr << #x << " : " << x << endl;}
#define debug2(x, y) if (DBG) {cerr << #x << " : " << x << " :: " << #y << " : " << y << endl;}
#define debug3(x, y, z) if (DBG) {cerr << #x << " : " << x << " :: " << #y << " : " << y << " :: " << #z << " : " << z << endl;}
#define rep(i,n) for(int i=0; i<(n); i++)
#define forn(i,a,b) for(int i=(a); i<=(b); i++)
#define forb(i,a,b) for(int i=(a); i>=(b); i--)
#define pb push_back
#define VS vector<string> 
#define VVS vector<VS > 

// Print Macros

#define PRINTTRANS 0
#define PRINTOBS 0

#define USERINPUT 0
#define CONFUSION 1

//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------


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

bool remove_extra(string& s){
	string temp = "";

	rep(i, s.size()) if(s[i] >= 'A' and s[i] <= 'Z') {
		if (s[i] == '\'') return true;
		temp.push_back(s[i]);
	}

	s = temp;
	return false;
}

//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------


struct phonemeOccurence{
	int lineNum;
	int phonemeNum;
	phonemeOccurence(int l,int p) : lineNum(l), phonemeNum(p){}
};
	
map<string, pair<int, vector<phonemeOccurence> > > phonemeOccurenceMap;
map<string, pair<int, vector<phonemeOccurence> > > :: iterator phonemeItr;

VVS data;



#define STATES 75
#define OBSERVATIONS 30

int occurenceCount[STATES][STATES][OBSERVATIONS];		// s[i] to s[j] on symbol obs[k]
int trigramCount[STATES][STATES][STATES][OBSERVATIONS];	// s[i] to s[j] on previous state s[k] on symbol obs[l] 
int transitionCount[STATES][STATES];					// s[i] to s[j] on any symbol
int observationCount[STATES][OBSERVATIONS];				// s[i] to any state on symbol obs[j]
int totalCount[STATES];									// s[i] to any state on any symbol


int confusionCount[STATES][STATES];						// s[i] confused with s[j]


int leftIndex = 0;										// to perform cross validation
string PHONEMES[STATES];
// S0 corresponds to phoneme ""(epsilon)

void buildOccurenceProbabilities()
{
	map<string, pair<int, vector<phonemeOccurence> > > :: iterator itr1, itr2, itr3;	

	// Building starts

	for (int i = 0; i < data.size(); i++){
		if(!USERINPUT) if (i%5 == leftIndex) continue;
		string OBS = data[i][0];
		itr1 = phonemeOccurenceMap.find(data[i][1]);

		occurenceCount[0][(itr1->second).first][int(OBS[0] - 'A')]++;
		trigramCount[0][(itr1->second).first][0][int(OBS[0] - 'A')]++;

		for (int j = 1; j < data[i].size() - 1; j++){
			itr1 = phonemeOccurenceMap.find(data[i][j]);
			itr2 = phonemeOccurenceMap.find(data[i][j+1]);
			if (itr1 == phonemeOccurenceMap.end() || itr2 == phonemeOccurenceMap.end()) {
				cerr << "Phoneme not found exception\n";
				return;
			}
			occurenceCount[(itr1->second).first][(itr2->second).first][int(OBS[j] - 'A')]++;
		}

		for (int j = 1; j < data[i].size() - 1; j++){
			itr1 = phonemeOccurenceMap.find(data[i][j]);
			itr2 = phonemeOccurenceMap.find(data[i][j+1]);
			itr3 = phonemeOccurenceMap.find(data[i][j-1]);
			if (itr1 == phonemeOccurenceMap.end() || itr2 == phonemeOccurenceMap.end()) {
				cerr << "Phoneme not found exception\n";
				return;
			}
			if (j == 1) trigramCount[(itr1->second).first][(itr2->second).first][0][int(OBS[j] - 'A')]++;
			else trigramCount[(itr1->second).first][(itr2->second).first][(itr3->second).first][int(OBS[j] - 'A')]++;
		}
	}
	// Building done

	rep(i, STATES){
		rep(j, STATES){
			int sum = 0;
			rep(k, OBSERVATIONS){
				sum += occurenceCount[i][j][k];
			}
			transitionCount[i][j] = sum;
		}
	}

	

	rep(i, STATES){
		rep(j, OBSERVATIONS){
			int sum = 0;
			rep(k, STATES){
				sum += occurenceCount[i][k][j];
			}
			observationCount[i][j] = sum;
		}
	}


	rep(i, STATES){
		int sum = 0;
		rep(j, STATES){
			sum += transitionCount[i][j];
		}
		totalCount[i] = sum;
		// debug3(i, PHONEMES[i], totalCount[i]);
	}

	if (PRINTTRANS){
		printf("\nPrinting Transition Table :: \n");
		cout << "\t\t\t";
		rep(i, 70) cout << PHONEMES[i+1] << "\t\t\t";
		cout << endl;
		rep(i, 70) {
			cout << PHONEMES[i+1] << "\t\t";
			rep(j, 70) cout << fixed << setprecision(4) << double(transitionCount[i+1][j+1])/double(totalCount[i+1]) << "\t\t"; 
			cout << "\n";
		}
	}

	if (PRINTOBS){
		printf("\nPrinting Observation Table ::\n");
		cout << "\t\t\t";
		rep(i, 26) cout << char(i + 'A') << "\t\t\t";
		cout << endl;
		rep(i, 70) {
			cout << PHONEMES[i+1] << "\t\t";
			rep(j, 26) cout << fixed << setprecision(4) << double(observationCount[i+1][j])/double(totalCount[i+1]) << "\t\t"; 
			cout << "\n";
		}	
	}
}

//--------------------------------------------------------------------------------------------------------


double SEQSCORE[STATES][OBSERVATIONS];
int BACKPTR[STATES][OBSERVATIONS];

int main()
{
	VS graphemes;
	// set<string> phonemes;

	string line;
	ifstream file;
	file.open("cmu.txt");
	int lineNum = 0, phonemeId = 0;
	
	PHONEMES[0] = "";

	//--------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------
	// Process input data
	while(getline(file,line)){
		
		graphemes = split_space(line);
		remove_extra(graphemes[0]);

		if(graphemes[0].size() != graphemes.size() - 1)	continue;	// Inconsistent data
		if(graphemes[0].size() > 4)	continue;	// Inconsistent data

		++lineNum;
		data.push_back(graphemes);

		// for (int i = 0; i < graphemes.size(); i++) cout << graphemes[i] << " ";
		// cout << "\n";

		for(int state = 1; state < graphemes.size(); state++){

			phonemeItr = phonemeOccurenceMap.find(graphemes[state]);

			if (phonemeItr == phonemeOccurenceMap.end()){
				++phonemeId;
				PHONEMES[phonemeId] = graphemes[state];
				vector<phonemeOccurence> newOccurenceVector;
				newOccurenceVector.push_back(phonemeOccurence(lineNum - 1, state));
				phonemeOccurenceMap.insert(map<string, pair<int, vector<phonemeOccurence> > > :: value_type(graphemes[state], make_pair(phonemeId, newOccurenceVector)));
			}

			else{
				(phonemeItr->second).second.push_back(phonemeOccurence(lineNum - 1, state));
			}
		}
	}

	// debug2(data.size(), phonemeOccurenceMap.size());

	// Reading data done
	//--------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------
	// Building SEQSCORE dynamically

	double match[5];
	for (leftIndex = 0; leftIndex < 5; leftIndex++){

		buildOccurenceProbabilities();

		int res = 0, total = 0;
		debug(leftIndex);
		for (int t = leftIndex + 1; t < data.size(); t += 5){
			int dataLine = t;
			debug(dataLine);
			string input = data[dataLine][0];
			if (USERINPUT) cin >> input;

			SEQSCORE[0][0] = 1.0;	// epsilon on state 0
			BACKPTR[0][0] = 0;	// epsilon on state 0

			int totalStates = phonemeOccurenceMap.size();

			rep(i, totalStates + 1)
				SEQSCORE[i][1] = double(occurenceCount[0][i][int(input[0] - 'A')])/double(totalCount[0]);	// 1.0;	// epsilon on state 0
			BACKPTR[0][0] = 0;	// epsilon on state 0


			//--------------------------------------------------------------------------------------------------------
			// DP : Viterbi Algorithm
			int observationLength = input.size();
			for (int observationNum = 2; observationNum <= observationLength; observationNum++){
				for (int state = 1; state <= totalStates; state++){
					double maxState = 0;
					double maxVal = 0;
					if (observationNum == 1){
						for (int j = 0; j <= totalStates; j++){
							double tempProb = double(occurenceCount[j][state][int(input[observationNum - 1] - 'A')])/double(totalCount[j]);
							// double tempProb = double(transitionCount[j][state]*observationCount[j][int(input[observationNum - 1] - 'A')])/double(totalCount[j]*totalCount[j]);

							if (SEQSCORE[j][observationNum - 1]*tempProb > maxVal){
								maxVal = SEQSCORE[j][observationNum - 1]*tempProb;
								maxState = j;
								// (double(transitionCount[j][state]*observationCount[j][int(input[observationNum - 1] - 'A')])/double(totalCount[j]*totalCount[j]))
							}
						}	
					}
					else{
						for (int j = 1; j <= totalStates; j++){
							// for (int k = 0; k <= totalStates; k++){
								int k = BACKPTR[j][observationNum-1];
								double tempProb1 = double(occurenceCount[k][j][int(input[observationNum - 2] - 'A')])/double(totalCount[k]);
								// double tempProb1 = double(transitionCount[k][j]*observationCount[k][int(input[observationNum - 2] - 'A')])/double(totalCount[k]*totalCount[k]);

								double tempProb2 = double(trigramCount[j][state][k][int(input[observationNum - 1] - 'A')])/double(totalCount[j]);
								if (SEQSCORE[k][observationNum - 2]*tempProb1*tempProb2 > maxVal){
									maxVal = SEQSCORE[k][observationNum - 2]*tempProb1*tempProb2;
									maxState = j;
									// (double(transitionCount[j][state]*observationCount[j][int(input[observationNum - 1] - 'A')])/double(totalCount[j]*totalCount[j]))
								}
							// }
						}
					}
					
					SEQSCORE[state][observationNum] = maxVal;
					BACKPTR[state][observationNum] = maxState;
				}
			}

			//--------------------------------------------------------------------------------------------------------

			int indexThatMaximizes = 0;
			double maxValue = 0;
			for (int i = 0; i <= totalStates; i++){
				if (maxValue < SEQSCORE[i][observationLength]){
					indexThatMaximizes = i;
					maxValue = SEQSCORE[i][observationLength];
				}
			}

			debug2(indexThatMaximizes, PHONEMES[indexThatMaximizes]);

			//--------------------------------------------------------------------------------------------------------
			stack<int> answer;
			answer.push(indexThatMaximizes);
			for (int i = observationLength - 1; i > 0; i--){
				indexThatMaximizes = BACKPTR[indexThatMaximizes][i+1];
				answer.push(indexThatMaximizes);
			}

			if (USERINPUT){
				while(!answer.empty()){
					cout << PHONEMES[answer.top()] << " ";
					answer.pop();
				}
				cout << endl;
			} else{
				for (int k = 1; k < data[dataLine].size(); k++){
					string s1 = data[dataLine][k];
					string s2 = PHONEMES[answer.top()];
					remove_extra(s1);
					remove_extra(s2);
					if (s1 == s2) res++;
					// if (data[dataLine][k] == PHONEMES[answer.top()]) res++;
					phonemeItr = phonemeOccurenceMap.find(data[dataLine][k]);
					confusionCount[(phonemeItr->second).first][answer.top()]++;

					total++;
					answer.pop();
				}
			}
		}
		match[leftIndex] = double(res)/double(total);
		cout << "Fraction matched for iteration " << leftIndex << " : " << match[leftIndex] << endl;



		if (CONFUSION){
			// printf("\nPrinting Confusion Matrix :: \n");
			// cout << "\t\t";
			// rep(i, 70) cout << PHONEMES[i+1] << "\t\t\t";
			// cout << endl;
			rep(i, STATES) {
				// cout << PHONEMES[i+1] << "\t\t";
				rep(j, STATES) //cout << confusionCount[i+1][j+1] << "\t\t\t"; 
				rep(k, STATES) //cout << confusionCount[i+1][j+1] << "\t\t\t"; 
				rep(h, STATES) //cout << confusionCount[i+1][j+1] << "\t\t\t"; 
				rep(l, OBSERVATIONS) //cout << confusionCount[i+1][j+1] << "\t\t\t"; 
				int x = confusionCount[i+1][j+1]*transitionCount[i+1][j+1];
				// cout << "\n";
			}
		}
	}
	
	double fracSum = 0;
	rep(i,5) fracSum += match[i];
	cout << "\nAverage Matched Percentage : " << (fracSum/5.0)*100.0 << endl;

}
