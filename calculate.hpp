#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <math.h> //log

using namespace std;

struct WordInfo {
	string word;
	vector<wchar_t> leftchars;
	vector<wchar_t> rightchars;
	double alpha; //自由程度
	double beta; //内聚程度
	size_t freq; //词出现频率
	double prob; //词出现概率
    bool has_max_beta; //词没有分裂
	bool choosed;

	WordInfo():word(""), alpha(0.0), beta(0.0), freq(0), prob(0.0), has_max_beta(false), choosed(true){}
	void print_info() {
		//cout << word << '\t' << alpha << '\t' << beta << '\t' << freq << '\t' << prob << endl;
		cout << word << '\t' << alpha << '\t' << beta << '\t' << freq << endl;
	}
};

template<typename T>
double calc_entropy(const vector<T> &vec) {
	map<T,int> count_map;
	for(size_t i = 0; i < vec.size(); ++i) {
		if (count_map.find(vec[i]) != count_map.end())
			count_map[vec[i]] += 1;
		else
			count_map.insert(typename map<T,int>::value_type(vec[i],1));
	}
	size_t n = vec.size();
	double entropy = 0.0;
	double prob = 0.0;
	typename map<T,int>::iterator itr = count_map.begin();
	for( ;itr != count_map.end(); ++itr) {
		prob = double(itr->second)/n;
		entropy += -prob*log(prob)/log(2.0);
	}
	return entropy;
}

void calc_alpha(map<string, WordInfo> &result_map) {
	map<string, WordInfo>::iterator itr;
	double word_count = result_map.size();
	itr =  result_map.begin();
	for( ; itr != result_map.end(); ++itr) {
		WordInfo *cur_info = &(itr->second);
		cur_info->prob = cur_info->freq/word_count;	
		double x =  calc_entropy(cur_info->leftchars);
		double y =  calc_entropy(cur_info->rightchars);
		cur_info->alpha = x < y ? x:y; //左邻字熵和右邻字熵的较小者
	}
}

int get_char_length(const char* str)
{
	if (!str)
		return 0;
	unsigned char mask = 0x80;
	if (!(str[0] & mask)) //ASCII
		return 1;
	int len = 0;
	while (str[0] & mask) {
		len++;
		mask = mask >> 1;
	}
	return len;
}

void calc_beta(map<string, WordInfo> &result_map) {
    int EXPAND_FACTOR = 1e8;
	map<string, WordInfo>::iterator itr = result_map.begin();
	string s1,s2;
	for( ; itr != result_map.end(); ++itr) {
		if (!itr->second.choosed)
			continue;
		string cur_word = itr->second.word;
		vector<double> values;
		const char *pstr = cur_word.c_str();
		size_t len = 0;
		while(*(pstr+len)) {
			len += get_char_length(pstr+len);
            if (len == 0)
                continue;
			s1 = cur_word.substr(0,len);
			s2 = cur_word.substr(len);
			if (result_map.find(s1) != result_map.end() && 
                    result_map.find(s2) != result_map.end()) {
				values.push_back(result_map[s1].prob * EXPAND_FACTOR * result_map[s2].prob);
			}
		}
		if (values.size() > 0) {
			double max = -100;
			for (size_t j = 0; j < values.size(); ++j)
				if (values[j] > max)
					max = values[j];
			itr->second.beta =  itr->second.prob * EXPAND_FACTOR / max; //词内所有组合中互信息最小值
		}
		else {
			itr->second.beta = itr->second.prob;//TODO:reset a new value
            itr->second.has_max_beta = true;
        }
	}
}
