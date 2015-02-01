#include <iostream>
#include <fstream>
#include "suffix.hpp"

using namespace std;

int convert_to_wstring(const char* str, wstring &out_wstr) { 
	if (!str)
		return 1;
	wchar_t* wcs = NULL; 
	int size = 0; 
	string loc = setlocale(LC_ALL,NULL);
	setlocale(LC_ALL, "zh_CN.utf8"); 
	size = mbstowcs(NULL,str,0); 
	wcs = new wchar_t[size+1]; 
	size = mbstowcs(wcs, str, size+1); 
	wcs[size] = 0; 
	out_wstr = wcs;
	delete[] wcs;
	setlocale(LC_ALL, loc.c_str()); 
	return 0;
}
int convert_to_string(const wchar_t * wstr, string &out_str) { 
	if (!wstr)
		return 1;
	char* str = NULL; 
	size_t size = 0; 
	string loc = setlocale(LC_ALL,NULL);
	setlocale(LC_ALL, "zh_CN.utf8"); 
	size = wcstombs( NULL, wstr, 0); 
	str = new char[size + 1]; 
	wcstombs(str, wstr, size); 
	str[size] = '\0'; 
	out_str = str;
	delete[] str;
	setlocale(LC_ALL, loc.c_str()); 
	return 0; 
}
int main(int argc, char* argv[]) {
	//wchar_t str[] = L"aabbaa\0ababab";
	wchar_t str[] = L"to_be`or`not_to_be`";
	//wchar_t str[] = L"嘻嘻哈哈嘻嘻\0嘻哈嘻哈嘻哈";
	//wstring wstr(str);
	ifstream infile(argv[1]);
	if (!infile) {
		cout << "open file error:" << argv[1] << endl;
		return 0;
	}
	string line;	
	string s;
	while (getline(infile, line)) {
		s += line;
	}
	infile.close();
	wstring input;
	convert_to_wstring(s.c_str(),input);
	size_t n = input.size();
    cout << "n=" << n << endl;
	SuffixInfo *sa = new SuffixInfo[n];
	size_t *lcp = new size_t[n+1];
	lcp[n] = 0;
	build_suffix_array(input.c_str(), n, lcp, sa);
	string output;
	for (int i=0; i<n; i++)
	{ 
		convert_to_string(input.substr(sa[i].i, 5).c_str(), output);
		//cout<<from[sa[i].i]<<' '<< output <<' '<<lcp[i]<<endl; 
		cout<< i << '\t' << output <<'\t'<<lcp[i]<<endl; 
	}
	return 0;
}
