#include <iostream>
#include <string.h>
using namespace std;

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

