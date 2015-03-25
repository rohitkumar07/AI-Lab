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

#define USERINPUT 0

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


struct alphabetOccurence{
	int lineNum;
	int phonemeNum;
	alphabetOccurence(int l,int p) : lineNum(l), phonemeNum(p){}
};



vector<alphabetOccurence> alphabetOccurenceMap[30];
map<string, int> phonemeHashMap;
map<string, int> :: iterator phonemeItr;

VVS data;



#define STATES 30
#define OBSERVATIONS 100

int occurenceCount[STATES][STATES][OBSERVATIONS];	// s[i] to s[j] on symbol obs[k]
int transitionCount[STATES][STATES];				// s[i] to s[j] on any symbol
int observationCount[STATES][OBSERVATIONS];			// s[i] to any state on symbol obs[j]
int totalCount[STATES];								// s[i] to any state on any symbol

int leftIndex = 0;									// to perform cross validation
string PHONEMES[OBSERVATIONS];
// S0 corresponds to phoneme ""(epsilon)

void buildOccurenceProbabilities()
{

	// Building starts
	for (int i = 0; i < data.size(); i++)
	{
		if(!USERINPUT) if (i%5 == leftIndex) continue;

		phonemeItr = phonemeHashMap.find(data[i][1]);
		if (phonemeItr == phonemeHashMap.end()) {cerr << "Exception\n"; return;}

		occurenceCount[0][int(data[i][0][0] - 'A') + 1][phonemeItr->second]++;	// count from start state

		for (int j = 1; j < data[i].size() - 1; j++)
		{	
			phonemeItr = phonemeHashMap.find(data[i][j+1]);
			if (phonemeItr == phonemeHashMap.end()) {cerr << "Exception\n"; return;}

			occurenceCount[int(data[i][0][j-1] - 'A') + 1][int(data[i][0][j] - 'A') + 1][phonemeItr->second]++;
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
		// debug3(i, char(i + 'A' - 1), totalCount[i]);
	}
	
	if (PRINTTRANS){
		printf("\nPrinting Transition Table :: \n");
		cout << "\t\t\t";
		rep(i, 26) cout << char(i + 'A') << "\t\t\t";
		cout << endl;
		rep(i, 26) {
			cout << char(i + 'A') << "\t\t";
			rep(j, 26) cout << fixed << setprecision(4) << double(transitionCount[i+1][j+1])/double(totalCount[i+1])<< "\t\t"; 
			cout << "\n";
		}
	}

	
	if (PRINTOBS){
		printf("\nPrinting Observation Table ::\n");
		cout << "\t\t";
		rep(i, 70) cout << PHONEMES[i] << "\t\t\t";
		cout << endl;
		rep(i, 26) {
			cout << char(i + 'A') << "\t\t";
			++phonemeItr;
			rep(j, 70) cout << fixed << setprecision(4) << double(observationCount[i+1][j])/double(totalCount[i+1]) << "\t\t"; 
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
	
	// PHONEMES[0] = '';

	//--------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------
	// Reading data
	while(getline(file,line)){
		
		graphemes = split_space(line);
		remove_extra(graphemes[0]);

		if(graphemes[0].size() != graphemes.size() - 1)	continue;	// Inconsistent data

		++lineNum;
		data.push_back(graphemes);


		for(int state = 0; state < graphemes[0].size(); state++){

			phonemeItr = phonemeHashMap.find(graphemes[state + 1]);

			alphabetOccurenceMap[int(graphemes[0][state] - 'A')].push_back(alphabetOccurence(lineNum - 1, state));

			if (phonemeItr == phonemeHashMap.end()){
				phonemeHashMap.insert(map<string, int> :: value_type(graphemes[state + 1], phonemeId));
				PHONEMES[phonemeId] = graphemes[state + 1];
				++phonemeId;
			}
		}
	}

	// debug2(data.size(), phonemeHashMap.size());// phonemeOccurenceMap.size());

	// Reading data done
	//--------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------
	// Building SEQSCORE dynamically

	// buildOccurenceProbabilities();

	double match[5];
	for (leftIndex = 0; leftIndex < 5; leftIndex++){

		buildOccurenceProbabilities();

		int res = 0, total = 0;
		for (int dataLine = leftIndex; dataLine < data.size(); dataLine += 5){
			VS input;

			if (USERINPUT){
				string inputLine;
				getline(cin, inputLine);
				input = split_space(inputLine);
			} else{
				input = data[dataLine];// + 1, data[dataLine] + data[dataLine].size());
				input.erase(input.begin());
			}

			SEQSCORE[0][0] = 1.0;	// epsilon on state 0
			BACKPTR[0][0] = 0;	// epsilon on state 0

			int totalStates = 26;

			// Not needed loop :
			// for (int i = 1; i <= totalStates; i++){
			// 	SEQSCORE[i][0] = 0.0;
			// }

			//--------------------------------------------------------------------------------------------------------
			// DP : Viterbi Algorithm

			int observationLength = input.size();
			for (int observationNum = 1; observationNum <= observationLength; observationNum++){
				for (int state = 0; state <= totalStates; state++){
					int maxState = 0;
					double maxVal = 0;
					for (int j = 0; j <= totalStates; j++){
						phonemeItr = phonemeHashMap.find(input[observationNum - 1]);
						if (phonemeItr == phonemeHashMap.end()){
							cerr << "Phoneme : " << input[observationNum - 1] << " not found in data\n";
							return 0;
						}
						if (SEQSCORE[j][observationNum - 1]*occurenceCount[j][state][phonemeItr->second] > maxVal){
							maxVal = SEQSCORE[j][observationNum - 1]*occurenceCount[j][state][phonemeItr->second];
							maxState = j;
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

			// debug2(indexThatMaximizes, char(indexThatMaximizes + 'A'));

			// //--------------------------------------------------------------------------------------------------------
			stack<int> answer;
			answer.push(indexThatMaximizes);
			for (int i = observationLength - 1; i > 0; i--){
				indexThatMaximizes = BACKPTR[indexThatMaximizes][i+1];
				answer.push(indexThatMaximizes);
			}

			if (USERINPUT){
				while(!answer.empty()){
					cout << char(answer.top() + 'A' - 1);
					// cout << answer.top() - 1 << endl;
 					answer.pop();
				}
				cout << endl;
			} else{
				for (int k = 0; k < data[dataLine][0].size(); k++){
					if (data[dataLine][0][k] == char(answer.top() + 'A' - 1)) res++;
					total++;
					answer.pop();
				}
			}
		}

		match[leftIndex] = double(res)/double(total);
		cout << "Fraction matched for iteration " << leftIndex << " : " << match[leftIndex] << endl;
	}
	
	double fracSum = 0;
	rep(i,5) fracSum += match[i];
	cout << "\nAverage Matched Percentage : " << (fracSum/5.0)*100.0 << endl;

}
