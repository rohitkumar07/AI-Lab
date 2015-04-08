#include <bits/stdc++.h>

using namespace std;

#define DBG 1
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

#define USERINPUT 1
#define CONFUSION 0
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

void remove_extra(string& s){
	string temp = "";

	rep(i, s.size()) if(s[i] >= 'A' and s[i] <= 'Z') temp.push_back(s[i]);

	s = temp;
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



#define STATES 100
#define OBSERVATIONS 30

int occurenceCount[STATES][STATES][OBSERVATIONS];	// s[i] to s[j] on symbol obs[k]
int transitionCount[STATES][STATES];				// s[i] to s[j] on any symbol
int confusionCount[STATES][STATES];					// s[i] confused with s[j]
int observationCount[STATES][OBSERVATIONS];			// s[i] to any state on symbol obs[j]
int totalCount[STATES];								// s[i] to any state on any symbol

int leftIndex = 0;									// to perform cross validation
string PHONEMES[STATES];
// S0 corresponds to phoneme ""(epsilon)

void buildOccurenceProbabilities()
{
	map<string, pair<int, vector<phonemeOccurence> > > :: iterator itr1, itr2;	

	// Building starts
	for (int i = 0; i < data.size(); i++){
		// if(!USERINPUT) if (i%5 == leftIndex) continue;
		string OBS = data[i][0];
		itr1 = phonemeOccurenceMap.find(data[i][1]);

		occurenceCount[0][(itr1->second).first][int(OBS[0] - 'A')]++;

		for (int j = 1; j < data[i].size() - 1; j++){
			itr1 = phonemeOccurenceMap.find(data[i][j]);
			itr2 = phonemeOccurenceMap.find(data[i][j+1]);
			if (itr1 == phonemeOccurenceMap.end() || itr2 == phonemeOccurenceMap.end()) {
				cerr << "Phoneme not found exception\n";
				return;
			}
			occurenceCount[(itr1->second).first][(itr2->second).first][int(OBS[j] - 'A')]++;
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
	file.open("cmu7a.txt");
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
		// if(graphemes[0].size() > 5)	continue;	// Inconsistent data

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

	// double match[5];
	// for (leftIndex = 0; leftIndex < 5; leftIndex++){

		buildOccurenceProbabilities();

		// int res = 0, total = 0;
		// for (int dataLine = leftIndex; dataLine < data.size(); dataLine += 5){
			string input ;//= data[dataLine][0];
			if (USERINPUT) cin >> input;

			remove_extra(input);

			SEQSCORE[0][0] = 1.0;	// epsilon on state 0
			BACKPTR[0][0] = 0;	// epsilon on state 0

			int totalStates = phonemeOccurenceMap.size();

			// Not needed loop :
			// for (int i = 1; i <= totalStates; i++){
			// 	SEQSCORE[i][0] = 0.0;
			// }

			//--------------------------------------------------------------------------------------------------------
			// DP : Viterbi Algorithm

			int observationLength = input.size();
			for (int observationNum = 1; observationNum <= observationLength; observationNum++){
				for (int state = 0; state <= totalStates; state++){
					double maxState = 0;
					double maxVal = 0;
					for (int j = 0; j <= totalStates; j++){
						double tempProb = double(occurenceCount[j][state][int(input[observationNum - 1] - 'A')])/double(totalCount[j]);
						if (SEQSCORE[j][observationNum - 1]*tempProb > maxVal){
							maxVal = SEQSCORE[j][observationNum - 1]*tempProb;
							maxState = j;
							// (double(transitionCount[j][state]*observationCount[j][int(input[observationNum - 1] - 'A')])/double(totalCount[j]*totalCount[j]))
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

			// debug2(indexThatMaximizes, PHONEMES[indexThatMaximizes]);

			//--------------------------------------------------------------------------------------------------------
			stack<int> answer;
			answer.push(indexThatMaximizes);
			for (int i = observationLength - 1; i > 0; i--){
				indexThatMaximizes = BACKPTR[indexThatMaximizes][i+1];
				answer.push(indexThatMaximizes);
			}

			if (USERINPUT){
				while(!answer.empty()){
					string s = PHONEMES[answer.top()];
					remove_extra(s);
					cout << s;
					answer.pop();
				}
				cout << endl;
			} 
			
			
		
		// match[leftIndex] = double(res)/double(total);
	
}
