#include <iostream>
#include <stack>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <fstream>
#include <time.h> //clock
#include <string.h> //memset, strncmp
#include <math.h> //log
#include <stdlib.h> //atof, atoi
#include "suffix.hpp"
#include "convert.hpp"
#include "calculate.hpp"

using namespace std;

int extract_by_suffix(wstring &input,const int max_combine_length, const int least_frequency, const double freedom_degree, const double cohesion_degree) {
	map<string, WordInfo> result_map;
	size_t n = input.size(); //input size
	size_t  *lcp = new size_t[n+1];
	lcp[n] = 0;
	SuffixInfo *sa = new SuffixInfo[n];
	build_suffix_array(input.c_str(), n, lcp, sa);
    
	//get word statistic info
    for (size_t i = 0; i < n; ++i) { //统计词频和右邻字
        size_t offset = 0;
        size_t start = sa[i].i;
        while ( offset < max_combine_length && start + offset < n) {
            offset++;
		    string cur_word("");
            convert_to_string(input.substr(start, offset).c_str(), cur_word);
			if (result_map.find(cur_word) == result_map.end()) {
				WordInfo wf;
				wf.word = cur_word;
				wf.freq = 1;
				result_map.insert(map<string,WordInfo>::value_type(cur_word, wf));
			}
			else {
				result_map[cur_word].freq += 1;
			}
            if (offset > lcp[i+1]) {//判断是否需要剪枝
				if (result_map[cur_word].freq < least_frequency && 
                        calc_entropy(result_map[cur_word].rightchars) < freedom_degree) {
                    result_map.erase(cur_word);
                    continue;
                }
            }
            if (start+offset < n) {
			    result_map[cur_word].rightchars.push_back(input[start+offset]);
            }
        }
    }
    //逆转输入求左邻字
	reverse(input.begin(),input.end()); 
	build_suffix_array(input.c_str(), n, lcp, sa);
    for (size_t i = 0; i < n; ++i) {
        size_t offset = 0;
        size_t start = sa[i].i;
        while ( offset < max_combine_length && start + offset < n) {
            offset++;
            wstring tmp_wstr = input.substr(start, offset);
            reverse(tmp_wstr.begin(), tmp_wstr.end());
		    string cur_word("");
            convert_to_string(tmp_wstr.c_str(), cur_word);
            if (start+offset < n) {
                //wchar_t x[2];
                //x[0] = input[start+offset];
                //x[1] = 0;
                //string w("");
                //convert_to_string(x, w);
                //cout << "left:" << cur_word << '\t' << w << endl;
			    result_map[cur_word].leftchars.push_back(input[start+offset]);
            }
        }
    }

    //calculate
	calc_alpha(result_map);
	map<string, WordInfo>::iterator itr = result_map.begin();
	for( ; itr != result_map.end(); ++itr) {
		if (itr->second.alpha < freedom_degree) //自由程度
			itr->second.choosed = false;
	}

	calc_beta(result_map);
	itr = result_map.begin();
	for( ; itr != result_map.end(); ++itr) {
		if (itr->second.beta > cohesion_degree) //内聚程度
			itr->second.print_info();
	}
	return 0;	
}

int main(int argc, char* argv[]) {
	if (argc != 6){
		cout << "Usage: " << argv[0] << " [infile] max_combine_length least_frequency freedom_degree cohesion_degree" << endl;
		cout << "e.g.: " << argv[0] << " 1.txt 5 6 0.9 200" << endl;
		return 0;
	}
	clock_t start = clock();
	ifstream infile(argv[1]);
	if (!infile) {
		cout << "open file error:" << argv[1] << endl;
		return 0;
	}
	wstring input;
	string line;	
	string s;
	while (getline(infile, line)) {
		s += line+"$";
	}
	infile.close();
	convert_to_wstring(s.c_str(),input);
	int max_combine_length = atoi(argv[2]); 
	extract_by_suffix(input, max_combine_length, atoi(argv[3]), atof(argv[4]), atof(argv[5]));
	clock_t end = clock();
	cout<<"Run time: "<<(double)(end - start) / CLOCKS_PER_SEC<<"s"<<endl;
	return 0;
}
