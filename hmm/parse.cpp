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
	string phoneme;
	vector<pai> decode_vector;  
	};

struct datstr{
	float prob;
	vector<int> sequence;
};

struct onephonemes
{
	string phoneme;
	int sum[26];
	int v[26][100];
	int totalsum;
	onephonemes(vector<phoneme_list> &vec ,vector<vector<string> > &data,string pho){
		phoneme = pho;
		for (int i = 0; i < 26; ++i)
		{	
			for (int j = 0; j < 100; ++j)
			{ v[i][j]=0;
			}
		}
		int j;
		for(j=0;j<vec.size();j++){
			if(vec[j].phoneme==phoneme){
				break;
			}
		}
		for (int i = 0; i < vec[j].decode_vector.size(); ++i)
		{
			int line_no = vec[j].decode_vector[i].line_no;
			int phoneme_no = vec[j].decode_vector[i].phoneme_no;
			//cout<<line_no<<":"<<phoneme_no<<":"<<data[line_no][0]<<endl;
			if(phoneme_no == data[line_no][0].size())continue;
			string nextphoneme = data[line_no][phoneme_no+1];
			//cout<<nextphoneme<<endl;
			for (int k = 0; k < vec.size(); k++)
			{
				if(vec[k].phoneme==nextphoneme){
					int charno = (data[line_no][0][phoneme_no] - 'A');
					v[charno][k]++;
					break;
				}
			}
		}
		calculate_sum();
	}
	
	void calculate_sum(){
		totalsum = 0;
		for(int i=0;i<26;i++){
			sum[i] = 0;
			for(int j=0;j<100;j++){
				sum[i] += v[i][j];
				}
			totalsum+=sum[i];
			}
		}
	
	onephonemes(vector<phoneme_list> &vec ,vector<vector<string> > &data){
		for (int i = 0; i < 26; ++i)
		{	
			for (int j = 0; j < 100; ++j)
			{ v[i][j]=0;
			}
		}
		
		for (int i = 0; i < data.size(); ++i)
		{
			int line_no = i;
			int phoneme_no = 0;
			string nextphoneme = data[line_no][phoneme_no+1];
			for (int k = 0; k < vec.size(); k++)
			{
				if(vec[k].phoneme==nextphoneme){
					int charno = (data[line_no][0][phoneme_no] - 'A');
					v[charno][k]++;
					break;
				}
			}
		}
		calculate_sum();
	}
	
	void print(vector<phoneme_list> &vec){
		cout << "Initial State: " << phoneme << endl;
		for (int i = 0; i < vec.size(); ++i)
		{	cout<<endl;
			cout<<char('A'+i)<<":\n";

			for (int j = 0; j < 100; ++j)
			{
				if(j<vec.size()){
					cout<<vec[j].phoneme<<": "<<v[i][j]/ double(totalsum)<<endl;
				}
				else continue;
			}
			cout<<endl;
		}
	}
};

int main(){


	double match[5];
	for (int leftIndex = 0; leftIndex < 5; leftIndex++)
	{
		vector<string> unseen;
		vector<vector<string> > unseenData;
		int take = 0;

		vector<vector<string> > data;
		vector<string> graphemes;
		set<string> phonemes;
		vector<phoneme_list> inverse_map;
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
				take++;

				if (take %5 == leftIndex) {
					unseen.push_back(graphemes[0]);
					unseenData.push_back(graphemes);
					continue;
				}
				data.push_back(graphemes);
				for(int i =1;i<graphemes.size();i++){
					bool found = false;
					for(int j=0;j<inverse_map.size();j++){
						if(inverse_map[j].phoneme == graphemes[i]){
							inverse_map[j].decode_vector.push_back(pai(data.size()-1,i));
							found = true;
							break;
							}
						}
					if(!found){
						phoneme_list p;
						p.phoneme = graphemes[i];
						p.decode_vector.push_back(pai(data.size()-1,i));
						inverse_map.push_back(p);
						}
					phonemes.insert(graphemes[i]);
				}
			}
		}

		// set<string> :: iterator itr = phonemes.begin();
		// while(itr != phonemes.end()){
		// 	cout << *itr << " ";
		// 	itr++;
		// }
		// cout << endl;

		// cout<<data.size()<<"::"<<phonemes.size()<<endl;


		vector<onephonemes> op;
		onephonemes op_epsilon =  onephonemes(inverse_map,data);
		for(int i=0;i<inverse_map.size();i++)
		{
			op.push_back(onephonemes(inverse_map,data,inverse_map[i].phoneme));
		}
		op.push_back(op_epsilon);

		// for (int i = 0; i < op.size(); i++)
		// 	op[i].print(inverse_map);	

		int cnt = 0, sat = 0;
		for (int ind  = 0; ind < unseen.size(); ind++)
		{
			string input = unseen[ind];
			int n = inverse_map.size();
			datstr states[n];
			for(int i =0;i<n;i++){
				states[i].prob = (float)op_epsilon.v[input[0]-'A'][i]/(float)op_epsilon.totalsum;
				states[i].sequence.push_back(i);
			}
			for(int i=1;i<input.size();i++){
				
				char x = input[i];
				datstr temp[n];
				
				for(int i=0;i<n;i++){
					float max;
					int maxnum;
					if(op[0].sum[x-'A'] == 0) {
						max = 0;
					}
					else max = (states[0].prob)*((float)op[0].v[x-'A'][i]/op[0].totalsum);
					//cout << max << ' ' << 0 << ' ' << i << endl;
					maxnum = 0;
					for(int j = 1;j<n;j++){
						if(op[j].sum[x-'A'] == 0) continue;//cout << "ayyo";
						float t = (states[j].prob)*(op[j].v[x-'A'][i]/(float)op[j].totalsum);
						//cout << t << ' ' << j << ' ' << i << endl;
						if(max < t){
							max = t;
							maxnum = j;
						}
					}
					temp[i].sequence = states[maxnum].sequence;
				//cout << maxnum << endl;
					temp[i].sequence.push_back(i); 
					temp[i].prob = max;
				}
				for(int i=0;i<n;i++){
					states[i].prob = temp[i].prob ;
					states[i].sequence = temp[i].sequence;
				}
			}

			int max_num = 0;
			for(int i=1;i<n;i++){
				if(states[i].prob > states[max_num].prob) max_num = i;
			}
			for(int i=0; i< states[max_num].sequence.size();i++){
				// cout << inverse_map[states[max_num].sequence[i]].phoneme << ' ';
				if (unseenData[ind][i+1] == inverse_map[states[max_num].sequence[i]].phoneme)
					sat++;
				cnt++;
			} 

		}
		match[leftIndex] = double(sat)/double(cnt);
		cout << "Fraction matched for iteration " << leftIndex << " : " << match[leftIndex] << endl;
	}
	
	double fracSum = 0;
	for(int i = 0; i < 5; i++) fracSum += match[i];
	cout << "\nAverage Matched Percentage : " << (fracSum/5.0)*100.0 << endl;

}
