#include <iostream>
using namespace std;

template<class T>
struct Node{
	T array[5];
	int size;
	Node* next;
	
	Node(Node* PNext = nullptr){
		next = PNext;
		size = 5;
	}
};

template<class T>
struct StackInf{
	T* top;
	Node<T>* currentNode;
	
	StackInf(){
		top = nullptr;
		currentNode = nullptr;
	}
		
	void push(T v){
		if(top == nullptr){
			currentNode = new Node<T>();
			top = currentNode->array;
			*top = v;
			return;
		}
		
		if(top == currentNode->array + currentNode->size - 1){
			currentNode = new Node<T>(currentNode);
			top = currentNode->array;
			*top = v;
			return;
		}
		
		*(++top) = v;
	}
		
	void pop(T& v){
		if(top == nullptr){
			v = -1;
			cout << "Empty Stack" << endl;
			return;
		}
		if(top == currentNode->array){
			v = *top;
			if(currentNode->next == nullptr){
				delete[] currentNode;
				currentNode = nullptr;
				top = nullptr;
			}
			else{
				Node<T>* tmp = currentNode;
				currentNode = currentNode->next;
				delete[] tmp;
				top = currentNode->array + currentNode->size - 1;
			}
			return;
		}
		v = *top;
		top--;
	}
		
	void print(){
		T* pTop = top;
		Node<T>* pNode = currentNode;
		
		if(top == nullptr){
			cout << "EMPTY STACK, NOTHING TO PRINT" << endl;
			return;
		}
		
		cout << "TOP [";
		while(true){
			cout << " " << *pTop;
			if(pTop == pNode->array){
				if(pNode->next == nullptr){
					cout << " ] BOTTOM" << endl;
					return;
				}
				pNode = pNode->next;
				pTop = pNode->array + pNode->size - 1;
				cout << " ] -----> [";
			}
			else pTop--;
		}
	}
};

template<class T>
struct QueueInf{
	T* head, * tail;
	Node<T>* headNode, * tailNode;
	
	
	QueueInf(){
		head = tail = nullptr;
		headNode = tailNode = nullptr;
	}
		
	void push(T v){
		if(head == nullptr){
			headNode = tailNode = new Node<T>();
			head = tail = headNode->array;
			*head = v;
			return;
		}
		
		if(tail == tailNode->array + tailNode->size - 1){
			tailNode = tailNode->next = new Node<T>();
			tail = tailNode->array;
			*tail = v;
			return;
		}
		
		*(++tail) = v;
	}
		
	void pop(T& v){
		if(head == nullptr){
			v = -1;
			cout << "Empty Queue" << endl;
			return;
		}
		
		v = *head;
		
		if(head == tail){
			delete[] headNode;
			head = tail = nullptr;
			headNode = tailNode = nullptr;
			return;
		}
		
		if(head == headNode->array + headNode->size - 1){
			Node<T>* tmp = headNode;
			headNode = headNode->next;
			head = headNode->array;
			delete[] tmp;
			return;
		}
		head++;
	}
		
	void print(){
		Node<T>* pNode = headNode;
		T* pElement = head;
		
		if(head == nullptr){
			cout << "EMPTY QUEUE, NOTHING TO PRINT" << endl;
			return;
		}
		
		cout << "HEAD [";
		while(true){
			cout << " " << *pElement;
			if(pElement == tail){
				cout << " ] TAIL" << endl;
				return;
			}
			if(pElement == pNode->array + pNode->size - 1){
				cout << " ] -----> [";
				pNode = pNode->next;
				pElement = pNode->array;
			}
			else pElement++;
		}
	}
};

	
// ----------------------------------------------------------------------------
// Main
// ----------------------------------------------------------------------------

int main() {
	cout << "STACK TESTING" << endl;
	StackInf<int> S;
	
	for(int i = 0; i < 12; i++){
		S.push(i);
		S.print();
	}
	
	for(int i = 0; i < 12; i++){
		int v;
		S.pop(v);
		cout << "POPPED: " << v;
		cout << endl;
		S.print();
	}
	
	
	
	cout << endl << "QUEUE TESTING" << endl;
	QueueInf<int> Q;
	
	for(int i = 0; i < 16; i++){
		Q.push(i);
		Q.print();
	}
	
	for(int i = 0; i < 16; i++){
		int v;
		Q.pop(v);
		cout << "POPPED: " << v;
		cout << endl;
		Q.print();
	}
	
	
	return 0;
}
