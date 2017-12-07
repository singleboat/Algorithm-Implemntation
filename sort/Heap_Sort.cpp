#include <iostream>
#include <cstdlib>

#define MAX 100
#define SWAP(x,y) x^=y^=x^=y 

using namespace std;

class HeapSort {
	private:
		int data[MAX+1];
		int size;
		void seep(int pos) {
			int tar = pos;
			if (2*pos <= size && data[2*pos] > data[tar]) tar = 2*pos;
			if (2*pos+1 <= size && data[2*pos+1] > data[tar]) tar = 2*pos+1;
			if (pos != tar) {
				SWAP(data[pos], data[tar]);
				seep(tar);
			}
		}
	public:
		HeapSort() {
			size = 0;
		}
		HeapSort(int *arr, int n) {
			size = n;
			int i = 0;
			for (i = 0; i < size; i++) data[i+1] = arr[i];
			for (i = size/2; i > 0; i--) seep(i);
		}
		~HeapSort() {}
		
		void buildHeap(int *arr, int n) {
			size = n;
			int i = 0;
			for (i = 0; i < size; i++) data[i+1] = arr[i];
			for (i = size/2; i > 0; i--) seep(i);
		}
		
		void clear() {
			size = 0;
		}
		
		void sort() {
			int _size = size;
			while (size > 1) {
				SWAP(data[1], data[size]);
				size--;
				seep(1);
			}
			size = _size;
		}
		
		void print() {
			for (int i = 1; i <= size; i++) cout << data[i] << " ";
			cout << endl;
		}
};

int main() {
	int i, n, entry[MAX+1];
	cin >> n;
	for (i = 0; i < n; i++) entry[i] = (rand()%1000);
	HeapSort *h = new HeapSort(entry, n);
	h->sort();
	h->print();
	return 0;
}
