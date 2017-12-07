#include<iostream>

template<class record>
void swap(record *a, record *b) {
	record tmp = *a;
	*a = *b;
	*b = tmp;
}

template <class record>
void qsort(record *start, record *end) {
	if (start >= end) return;
	int size = end - start + 1;
	record mid = *(start + size / 2);
	swap(end, start + size / 2);
	record *i = end - 1, *lastBig = end;
	while (i >= start) {
		if (*i >= mid) swap(i, --lastBig);
		i--;
	}
	swap(lastBig, end);
	qsort(start, lastBig-1);
	qsort(lastBig+1, end);
}

int main() {
	int t;
	std::cin >> t;
	while (t--) {
		int arr[100] = {0};
		int n;
		std::cin >> n;
		for (int i = 0; i < n; i++) std::cin >> arr[i];
		quick_sort(arr, arr+n-1);
		for (int i = 0; i < n; i++) std::cout << arr[i] << " ";
		std::cout << std::endl;
	}
	return 0;
}

