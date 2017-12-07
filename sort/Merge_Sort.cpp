#include<iostream>

template <class Record>
struct Node {
	Record entry;
    Node<Record> *next;
};

template <class Record>
Node<Record>* msort(Node<Record> *head) {
	if (head == NULL) return NULL;
	if (head->next == NULL) return head;
	Node<Record> *mid, *p;
	mid = p = head;
	while (p != NULL) {
		p = p->next;
		if (p != NULL && p->next != NULL) {
			p = p->next;
			mid = mid->next;
		}
	}
	p = mid->next;
	mid->next = NULL;
	Node<Record> *first = msort(head);
	Node<Record> *second = msort(p);
	if (first->entry <= second->entry) {
		head = first;
		first = first->next;
	} else {
		head = second;
		second = second->next;
	}
	p = head;
	while (first != NULL && second != NULL) {
		if (first->entry <= second->entry) {
			p->next = first;
			first = first->next;
		} else {
			p->next = second;
			second = second->next;
		}
		p = p->next;
	}
	if (first == NULL) p->next = second;
	else p->next = first;
	return head;
}

template <class Record>
class Sortable_list {
	public:
		Sortable_list() {
			head = NULL;
		}
		
		~Sortable_list() {
			Node<Record> *p;
			while (head != NULL) {
				p = head;
				head = head->next;
				delete p;
			}
		}
		
		void insert(int position, const Record &entry) {
			Node<Record> *p = new Node<Record>;
			p->entry = entry;
			p->next = NULL;
			if (position == 0) {
				p->next = head;
				head = p;
			} else {
				Node<Record> *tmp = head;
				int count = 0;
				while (count < position-1 && tmp != NULL) {
					tmp = tmp->next;
					count++;
				}
				p->next = tmp->next;
				tmp->next = p;
			}
		}
		
	    void print() {
	    	Node<Record> *p = head;
	    	while (p != NULL) {
	    		std::cout << p->entry;
	    		if (p->next != NULL) std::cout << " ";
	    		p = p->next;
			}
			std::cout << std::endl;
		}
		
	    void merge_sort() {
	    	head = msort(head);
		}
		
	private:	
		Node<Record> *head;
};

int main() {
	Sortable_list<int> list;
	int num, n;
	std::cin >> n;
	for (int i = 0; i < n; i++) {
		std::cin >> num;
		list.insert(i, num);
	}
	list.merge_sort();
	list.print();
	return 0;
}
