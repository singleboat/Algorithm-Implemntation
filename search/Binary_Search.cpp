#include<iostream>
#include<vector>

using std::vector;

template <typename T>
int binarySearch(const vector<T> table, int bot, int top, const T &x) {
	if (top < bot) return -1;
	while (bot != top) {
		if (table[(bot+top+1)/2] > x) top = (bot+top)/2;
		else bot = (bot+top+1)/2;
	}
	if (table[bot] == x) return bot;
	else return -1;
}

int main() {
	int a[] = {0, 1, 1, 3, 3, 3, 6};
	vector<int> vec(a, a+7);
	std::cout << binarySearch(vec, 0, vec.size()-1, 3) << std::endl;
	std::cout << binarySearch(vec, 0, vec.size()-1, 5) << std::endl;
	return 0;
}

