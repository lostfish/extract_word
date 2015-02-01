#include <iostream>
#include <string>
#include <string.h>
#include <algorithm>
using namespace std;


struct SuffixInfo {
	size_t i; //string index 
	size_t key[2];//two ranking values
	bool operator < (const SuffixInfo& s) const {   
		return key[0] < s.key[0] || 
			key[0] == s.key[0] && key[1] < s.key[1]; 
	}
};

void sort_suffix_array(SuffixInfo* in, size_t n, int key, SuffixInfo* out, size_t *rank) { //radix sort is stable
	size_t* count = rank;
	memset( count, 0, sizeof(size_t) * (n + 1) );
	for (size_t i = 0; i < n; i++)
		count[ in[i].key[key] ]++; //get count for every ranking value
	for (size_t i = 1; i <= n; i++)
		count[i] += count[i - 1]; //accumulate
	for (long long i = n - 1; i >= 0; i--){
		out[ --count[ in[i].key[key] ] ] = in[i]; //sort
	}
}

size_t get_lcp(const wchar_t* a, const wchar_t* b) { 
	size_t len=0;  
	while(*a && *b && *a==*b) {
        len++;
        a++;
        b++;
    }  
	return len;
}

void get_lcp(const wchar_t* text, SuffixInfo* sfx, size_t len, size_t* lcp, size_t *rank) {
	for (size_t i=0, r=0; i < len; i++, r++)
		rank[ sfx[i].i ] = r;
	lcp[0] = 0;
	if (rank[0])
		lcp[ rank[0] ] = get_lcp( text, text + sfx[ rank[0]-1 ].i );
	for (size_t i = 1; i < len; i++) {
		if ( !rank[i] )
			continue;
		if (lcp[ rank[i - 1] ] <= 1)
			lcp[ rank[i] ] = get_lcp( text+i, text+sfx[ rank[i]-1 ].i );
		else{
			size_t L = lcp[ rank[i - 1] ] - 1;
			lcp[rank[i]] = L+get_lcp(text+i+L, text+sfx[rank[i]-1].i+L);
		}
	}
}

/**
 *@brief Build suffix array and longest common prefix
 *@param[in] text input text
 *@param[in] len input text length
 *@param[out] lcp longest common prefix
 *@param[out] sa suffix array
 *
 *return: 0 if success, -1 if fail
 */
int build_suffix_array(const wchar_t* text, const size_t len, size_t *lcp, SuffixInfo *sa) {
    if (!text)
        return -1;
	SuffixInfo *temp = new SuffixInfo[len];
	size_t *rank = new size_t[len + 1]; //'rank' array
	for (size_t i = 0; i < len; i++){ //initialize
		sa[i].i = i; //from 0 to len-1
		sa[i].key[1] = i;  
		sa[i].key[0] = text[i]; 
	}
	sort(sa, sa + len);
	for (size_t i = 0; i < len; i++)
		sa[i].key[1] = 0;
	size_t width = 1;
	while (width < len) {
		rank[ sa[0].i ] = 1; //the rank is from 1 to len
		for (size_t i = 1; i < len; i++){ //sa is sorted
			rank[ sa[i].i ] = rank[ sa[i - 1].i ];
			if ( sa[i-1] < sa[i] ) 
				rank[ sa[i].i ]++;
		}
		for (size_t i = 0; i < len; i++){ //update the two ranking values
			sa[i].i = i;
			sa[i].key[0] = rank[i];
			sa[i].key[1] = i + width < len? rank[i + width]: 0;
		} 
		sort_suffix_array(sa, len, 1, temp, rank); //sort by the second key
		sort_suffix_array(temp, len, 0, sa, rank); //sort by the first key
		width *= 2; //double increase
	}
	get_lcp(text, sa, len, lcp, rank);
	delete[] rank;
	delete[] temp;
    return 0;
}
