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
#include "convert.hpp"
#include "calculate.hpp"

using namespace std;

int extract(const string &input,const int max_combine_length, const int least_frequency, const double freedom_degree, const double cohesion_degree) {
	map<string, WordInfo> result_map;
	size_t size = 0;
	size_t len = 0;
	size_t pos = 0;
	const char* pstr = input.c_str();
	vector<pair<size_t,size_t> > charVec; //position and length
	while(*(pstr+pos)) {
		len = get_char_length(pstr+pos);
		charVec.push_back(pair<size_t,size_t>(pos,len));
		pos += len;
		size++;	
	}
	//cout << "size:" << size << endl;
	//get word statistic info
	for(size_t i = 0; i < size - max_combine_length; ++i) {
		string cur_word;
		for(size_t j = 1; j <= max_combine_length; ++j) {
			cur_word = input.substr(charVec[i].first, charVec[i+j].first-charVec[i].first);
			//cout << i << '\t' << j << '\t' <<  cur_word << endl;
			wstring c; //only one character
			wchar_t left_neighbor;
			wchar_t right_neighbor;
			if (i > 0) {
				convert_to_wstring(input.substr(charVec[i-1].first,charVec[i-1].second).c_str(), c);
				left_neighbor = c[0];
			}
			convert_to_wstring(input.substr(charVec[i+j].first,charVec[i+j].second).c_str(), c);
			right_neighbor = c[0];  
			if (result_map.find(cur_word) == result_map.end()) {
				WordInfo wf;
				wf.word = cur_word;
				wf.freq = 1;
				result_map.insert(map<string,WordInfo>::value_type(cur_word, wf));
			}
			else {
				result_map[cur_word].freq += 1;
			}
			if(i >0) {
				result_map[cur_word].leftchars.push_back(left_neighbor);
			}
			result_map[cur_word].rightchars.push_back(right_neighbor);
		}
	}
	//calculate 
	calc_alpha(result_map);
	map<string, WordInfo>::iterator itr = result_map.begin();
	for( ; itr != result_map.end(); ++itr) {
		if (itr->second.alpha < freedom_degree or itr->second.freq < least_frequency ) //自由程度
			itr->second.choosed = false;
	}

	calc_beta(result_map);
	itr = result_map.begin();
	for( ; itr != result_map.end(); ++itr) {
		if (!itr->second.has_max_beta and itr->second.beta < cohesion_degree) //内聚程度
			itr->second.choosed = false;
	}
	itr = result_map.begin();
	for( ; itr != result_map.end(); ++itr) {
        if (itr->second.choosed)
			itr->second.print_info();
    }
	return 0;	
}

//bool cmp(const char *s1, const char *s2) {
//    if (strncmp(s1, s2, 6) < 0) //TODO:how to set n
//        return true;
//    return false;
//}
//
//int extract_by_suffix(const string &input,const int max_combine_length, const int least_frequency, const double freedom_degree, const double cohesion_degree) {
//	map<string, WordInfo> result_map;
//	size_t size = 0;
//	size_t len = 0;
//	size_t pos = 0;
//	const char* pstr = input.c_str();
//    vector<const char*> suffix_vec;
//    suffix_vec.reserve(input.size());
//	while(*(pstr+pos)) {
//		len = get_char_length(pstr+pos);
//        suffix_vec.push_back(pstr+pos);
//		pos += len;
//		size++;	
//	}
//    sort(suffix_vec.begin(), suffix_vec.end(), cmp);
//    
//	//cout << "size:" << size << endl;
//	//get word statistic info
//    vector<const char *>::iterator sfx_itr = suffix_vec.begin();
//    char tmp_str[100];
//	wstring c; //only one character
//    for ( ; sfx_itr != suffix_vec.end(); ++sfx_itr) { //统计词频和右邻字
//        pstr =  *sfx_itr;
//        size_t offset = 0;
//        size_t i = 0;
//        while ( i < max_combine_length or *pstr != '\0') {
//            i++;
//		    len = get_char_length(pstr+offset);
//            offset += len;
//            strncpy(tmp_str, pstr, offset);
//            tmp_str[offset] = '\0';
//		    string cur_word(tmp_str); 
//			if (result_map.find(cur_word) == result_map.end()) {
//				WordInfo wf;
//				wf.word = cur_word;
//				wf.freq = 1;
//				result_map.insert(map<string,WordInfo>::value_type(cur_word, wf));
//			}
//			else {
//				result_map[cur_word].freq += 1;
//			}
//            if (*(pstr+offset)) {
//		        len = get_char_length(pstr+offset);
//                strncpy(tmp_str,pstr+offset, len);
//                tmp_str[len] = '\0';
//			    convert_to_wstring(tmp_str, c);
//			    wchar_t right_neighbor = c[0];  
//			    result_map[cur_word].rightchars.push_back(right_neighbor);
//            }
//        }
//    }
//    //TODO:计算左邻字
//	//calculate 
//	calc_alpha(result_map);
//	map<string, WordInfo>::iterator itr = result_map.begin();
//	for( ; itr != result_map.end(); ++itr) {
//		if (itr->second.alpha < freedom_degree) //自由程度
//			itr->second.choosed = false;
//	}
//
//	calc_beta(result_map);
//	itr = result_map.begin();
//	for( ; itr != result_map.end(); ++itr) {
//		if (itr->second.beta > cohesion_degree) //内聚程度
//			itr->second.print_info();
//	}
//	return 0;	
//}

int main(int argc, char* argv[]) {
	if (argc != 6){
		cout << "Usage: " << argv[0] << " [infile] max_combine_length least_frequency freedom_degree cohesion_degree" << endl;
		cout << "e.g.: " << argv[0] << " 1.txt 6 5 0.9 200" << endl;
		return 0;
	}
	clock_t start = clock();
	ifstream infile(argv[1]);
	if (!infile) {
		cout << "open file error:" << argv[1] << endl;
		return 0;
	}
	string line;	
	string input;
	while (getline(infile, line)) {
		input += line + '$';
	}
	infile.close();
	int max_combine_length = atoi(argv[2]); 
	input += string(max_combine_length,'$'); //for convenience
	extract(input, max_combine_length, atoi(argv[3]), atof(argv[4]), atof(argv[5]));
	clock_t end = clock();
	cout<<"Run time: "<<(double)(end - start) / CLOCKS_PER_SEC<<"s"<<endl;
	return 0;
}
