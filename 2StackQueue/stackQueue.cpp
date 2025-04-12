#include <iostream>

using namespace std;


template <class T>
struct Stack{
	T* top;
	T array[10];
	
	Stack(){
		top = nullptr;
	}
		
		void push(T v){
			if(top == nullptr){
				top = array;
				*top = v;
				return;
			}
			
			if(top == array+10-1){
				cout << "FULL STACK" << endl;
				return;
			}
			
			*(++top) = v;
		}
			
		void pop(T& v){
			if(top == nullptr){
				cout << "EMPTY STACK" << endl;
				return;
			}
			
			v = *top;
			
			if(top == array){
				top = nullptr;
				return;
			}
			
			top--;
		}
			
		void print(){
			T* pTop = top;
			
			if(top == nullptr){
				cout << "EMPTY STACK, NOTHING TO PRINT" << endl;
				return;
			}
			
			cout << "TOP [";
			while(true){
				cout << " " << *pTop;
				if(pTop == array){
						cout << " ] BOTTOM" << endl;
						return;
				}
				else pTop--;
			}
		}
};

template <class T>
struct Queue{
	T* head, * tail;
	T array[10];
	
	Queue(){
		head = tail = nullptr;
	}
		
		void push(T v){
			if(head == nullptr){
				head = tail = array;
				*tail = v;
				return;
			}
			
			if((tail+1) == head || (tail == array+10-1 && head == array)){
				cout << "FULL QUEUE" << endl;
				return;
			}
			
			tail = (tail == array + 10-1 ? array : tail+1);
			
			*tail = v;
		}
			
		void pop(T& v){
			if(head == nullptr){
				cout << "EMPTY QUEUE" << endl;
				head = tail = nullptr;
				return;
			}
			
			v = *head;
			
			if(head == tail){
				head = tail = nullptr;
				return;
			}
			
			head = (head == array + 10 ? array : head+1);
		}
			
		void print(){
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
				else pElement++;
			}
		}
};


int main() {
	Stack<int> S;
	
	for(int i = 0 ; i < 15; i++){
		S.push(i);
		S.print();
	}
	for(int i = 0 ; i < 15; i++){
		int v;
		S.pop(v);
		cout << "POPPED: " << v << endl;
		S.print();
	}
	
	Queue<int> Q;
	
	for(int i = 0 ; i < 15; i++){
		Q.push(i);
		Q.print();
	}
	for(int i = 0 ; i < 15; i++){
		int v;
		Q.pop(v);
		cout << "POPPED: " << v << endl;
		Q.print();
	}
	return 0;
}
