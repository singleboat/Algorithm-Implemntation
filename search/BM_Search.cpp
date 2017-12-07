#include <iostream>
#include <string>

#define MAX_CHAR 128
#define MAX_LENGTH 100

using namespace std;

int max(int a, int b) {return ((a > b) ? a : b);}

int search(string pattern, string text) {
	int i, j, k;
	int bm_bc[MAX_CHAR] = {0};
	int suffix[MAX_LENGTH] = {0};
	int bm_gs[MAX_LENGTH] = {0};
	int plen = pattern.length(), tlen = text.length();
	
	for (i = 0; i < MAX_CHAR; i++) bm_bc[i] = plen;
	for (i = 0; i < plen; i++) bm_bc[pattern[i]] = plen-i-1;

	suffix[plen-1] = plen;
	j = plen-1;
	for (i = plen-2; i >= 0; i--) {
		if (i > j && suffix[plen-1-(k-i)] < i-j) suffix[i] = suffix[plen-1-(k-i)];
		else {
			if (j > i) j = i;
			k = i;
			while (j >= 0 && pattern[j] == pattern[plen-1-(i-j)]) j--;
			suffix[i] = i-j;
		}
	}
	
	for (i = 0; i < plen; i++) bm_gs[i] = plen;
	
	j = 0;
	for (i = plen-1; i >= 0; i--) {
		if (suffix[i] == i+1) {
			for (;j < plen-1-i; j++) {
				if (bm_gs[j] == plen) bm_gs[j] = plen-1-i;
			}
		}
	}
	
	for (i = 0; i < plen-1; i++) bm_gs[plen-1-suffix[i]] = plen-i-1;

	j = 0;
	while (j <= tlen-plen) {
		cout << text << endl;
		for (k = 0; k < j; k++) cout << " ";
		cout << pattern << endl; //for observing the process
		
		for (i = plen-1; i >= 0 && pattern[i] == text[i+j]; i--) {}
		if (i < 0) return j;
		else j += max(bm_bc[text[i+j]]-(plen-i-1), bm_gs[i]);
	}
	return -1;
}

int main() {
	string pattern, text;
	cout << "please input pattern: ";
	cin >> pattern;
	while (pattern.length() > MAX_LENGTH) {
		cout << "your input is too long, please input again: ";
		cin >> pattern;
	}
	cout << "please input text: ";
	cin >> text;
	
	int pos = search(pattern, text);
	if (pos != -1) cout << pattern << " first appeared in " << pos+1;
	else cout << pattern << " does not in the text";
	
	return 0;
}
