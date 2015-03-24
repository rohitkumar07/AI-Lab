#include <iostream>
#include <fstream>
#include <vector>
#include <set>
using namespace std;
vector<string> split_space(string s){
	int i =0;
	vector<string> ans;
	int space=0;
	for(int j=0;j<s.size();j++){
		if(space==1){
			if(s[j]==' '){
				i++;
				continue;
			}
			else{
				space = 0;
				}
			}
		if(s[j] == ' '){
			ans.push_back(s.substr(i,j-i));
			i = j+1;
			space = 1;
		}
	}
	ans.push_back(s.substr(i,s.size()-i));
	return ans;
}

void remove_extra(string& s){
	string res = "";
	for (int i = 0; i < s.size(); i++){
		if (s[i] >= 'A' and s[i] <= 'Z')
			res = res + s[i];
	}
	s = res;
}
	
struct pai{
	int line_no;
	int phoneme_no;
	pai(int one,int two){
		line_no = one;
		phoneme_no = two;
		}
	};
	
struct phoneme_list{
	char phoneme;
	vector<pai> decode_vector;  
	};

struct datstr{
	float prob;
	vector<int> sequence;
};

struct onephonemes
{
	char phoneme;
	int sum[100];
	int v[100][30];
	int totalsum;
	onephonemes(phoneme_list *vec , vector<vector<string> > &data, char phoneme, vector<string> &V){
		for (int i = 0; i < 100; ++i)
		{	
			for (int j = 0; j < 30; ++j)
			{ 
				v[i][j]=0;
			}
		}

		int j;
		// for(j = 0; j < vec.size(); j++){
		// 	if(vec[j].phoneme == phoneme){
		// 		break;
		// 	}
		// }

		j = int(phoneme - 'A');

		for (int i = 0; i < vec[j].decode_vector.size(); ++i)
		{
			int line_no = vec[j].decode_vector[i].line_no;
			int phoneme_no = vec[j].decode_vector[i].phoneme_no;
			//cout<<line_no<<":"<<phoneme_no<<":"<<data[line_no][0]<<endl;
			if(phoneme_no == data[line_no][0].size() - 1) continue;
			char nextphoneme = data[line_no][0][phoneme_no+1];
			//cout<<nextphoneme<<endl;
			int k = int(nextphoneme - 'A');
			{
				// if(vec[k].phoneme == nextphoneme){
					string charno = data[line_no][phoneme_no+1];
					int index = 0;
					for (index = 0; index < V.size(); index++)
						if (V[index] == charno) break;
					v[index][k]++;
				// }
			}
		}
		calculate_sum(V.size());
	}
	
	void calculate_sum(int S){
		totalsum = 0;
		for(int i=0;i<S;i++){
			sum[i] = 0;
			for(int j=0;j<30;j++){
				sum[i] += v[i][j];
				}
			totalsum+=sum[i];
			}
		}
	
	// onephonemes(){}

	onephonemes(vector<vector<string> > &data, vector<string> &V){
		for (int i = 0; i < 100; ++i)
		{	
			for (int j = 0; j < 30; ++j)
			{ 
				v[i][j]=0;
			}
		}
		
		for (int i = 0; i < data.size(); ++i)
		{
			char nextphoneme = data[i][0][0];
			// for (int k = 0; k < vec.size(); k++)
			// {
				int k = int(nextphoneme - 'A');
				{
					// if(vec[k].phoneme == nextphoneme){
						string charno = data[i][1];
						int index = 0;
						for (index = 0; index < V.size(); index++)
							if (V[index] == charno) break;
						v[index][k]++;
					// }
				}
			// }
		}

		calculate_sum(V.size());
	}
	
// 	void print(vector<phoneme_list> &vec){
// 		for (int i = 0; i < vec.size(); ++i)
// 		{	cout<<endl;
// 			cout<<'A'+i<<":\n";
// 			for (int j = 0; j < 100; ++j)
// 			{
// 				if(j < vec.size()){
// 					cout<<vec[j].phoneme<<": "<<v[i][j]<<endl;
// 				}
// 				else continue;
// 			}
// 			cout<<endl;
// 		}
// 	}
};

int main(){
	vector<vector<string> > data;
	vector<string> graphemes;
	set<string> phonemes;
	phoneme_list inverse_map[100];
	string line;
	ifstream file;
	file.open("cmu.txt");
	int line_no=0;
	while(getline(file,line)){
		line_no++;
		graphemes = split_space(line);
		remove_extra(graphemes[0]);
		if(graphemes[0].size() +1 != graphemes.size()){
			line_no--;
			continue;
		}
		else{
			data.push_back(graphemes);
			for(int i = 0 ; i < graphemes[0].size(); i++){
				// bool found = false;
				// for(int j=0;j<inverse_map.size();j++){
				// 	if(inverse_map[j].phoneme == graphemes[0][i]){
					// if (graphemes[0][i] >= 'A' and graphemes[0][i] <= 'Z')
						inverse_map[int(graphemes[0][i] - 'A')].decode_vector.push_back(pai(data.size()-1,i));
					// else cout << graphemes[0][i] << endl;
						// found = true;
					// 	break;
					// 	}
					// }
				// if(!found){
				// 	phoneme_list p;
				// 	p.phoneme = graphemes[0][i];
				// 	p.decode_vector.push_back(pai(data.size()-1,i));
				// 	inverse_map.push_back(p);
				// 	}
			for(int j = 1 ; j < graphemes.size(); j++)
				phonemes.insert(graphemes[j]);
			}
		}
	}

	vector<string> V;
	set<string> :: iterator itr = phonemes.begin();
	while(itr != phonemes.end()){
		// cout << *itr << " ";
		V.push_back(*itr);// << " ";
		itr++;
	}

	cout << "\n" << data.size() << "::" << phonemes.size()<<endl;
	vector<onephonemes> op;
	onephonemes op_epsilon =  onephonemes(data, V);

	for(int i=0;i<26; i++){
		op.push_back(onephonemes(inverse_map, data, i + 'A', V));
	}

	// op.push_back(op_epsilon);
	
	string input;
	getline(cin, input);
	// cin>>input;
		
	vector<string> P = split_space(input);

	
	
	// int n = inverse_map.size();
	// datstr states[n];
	// for(int i =0;i<n;i++){
	// 	states[i].prob = (float)op_epsilon.v[input[0]-'A'][i]/(float)op_epsilon.totalsum;
	// 	states[i].sequence.push_back(i);
	// }
	// for(int i=1;i<input.size();i++){
		
	// 	char x = input[i];
	// 	datstr temp[n];
	// for(int i=0;i<n;i++){
	// 	float max;
	// 	int maxnum;
	// 	if(op[0].sum[x-'A'] == 0) {
	// 		max = 0;
	// 	}
	// 	else max = (states[0].prob)*((float)op[0].v[x-'A'][i]/op[0].totalsum);
	// 	//cout << max << ' ' << 0 << ' ' << i << endl;
	// 	maxnum = 0;
	// 	for(int j = 1;j<n;j++){
	// 		if(op[j].sum[x-'A'] == 0) continue;//cout << "ayyo";
	// 		float t = (states[j].prob)*(op[j].v[x-'A'][i]/(float)op[j].totalsum);
	// 		//cout << t << ' ' << j << ' ' << i << endl;
	// 		if(max < t){
	// 			max = t;
	// 			maxnum = j;
	// 		}
	// 	}
	// 	temp[i].sequence = states[maxnum].sequence;
	// 	//cout << maxnum << endl;
	// 	temp[i].sequence.push_back(i); 
	// 	temp[i].prob = max;
	// }
	// for(int i=0;i<n;i++){
	// 	states[i].prob = temp[i].prob ;
	// 	states[i].sequence = temp[i].sequence;
	// }
	// 	}
	// 	int max_num = 0;
	// 	for(int i=1;i<n;i++){
	// 		if(states[i].prob > states[max_num].prob) max_num = i;
	// 	}
	// 	//cout << "max_num" << max_num << endl;
	// 	//cout << "Answer" << endl;
	// for(int i=0;i<states[max_num].sequence.size();i++){
	// 	cout << inverse_map[states[max_num].sequence[i]].phoneme << ' ';
	// } 
	// cout<<endl;
	// //op[0].print(inverse_map);

}
