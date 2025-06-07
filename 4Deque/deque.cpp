#include <iostream>

using namespace std;

template<class T>
struct CDeque{
	T* head, * tail;
	T** mapArray, ** mapHead, ** mapTail;
	int mapSize, arraySize;
	
	CDeque(){
		mapSize = 10;
		arraySize = 5;
		mapArray = mapHead = mapTail = nullptr;
		head = tail = nullptr;
	}
		
	void push_front(T v){
		if(mapArray == nullptr){
			mapArray = new T*[mapSize];
			mapHead = mapTail = mapArray+(mapSize/2);
			*mapHead = new T[arraySize];
			head = tail = *mapHead;
			*head = v;
			return;
		}
		
		if(head == *mapHead){
			if(mapHead == mapArray){
				cout << "FULL FRONTSIDE DEQUE" << endl;
				return;
			}
			*(--mapHead) = new T[arraySize];
			head = *mapHead+arraySize-1;
			*head = v;
			return;
		}
		
		*(--head) = v;
	}
	
	void push_back(T v){
		if(mapArray == nullptr){
			mapArray = new T*[mapSize];
			mapHead = mapTail = mapArray+(mapSize/2);
			*mapHead = new T[arraySize];
			head = tail = *mapHead;
			*head = v;
			return;
		}
		
		if(tail == *mapTail+arraySize-1){
			if(mapTail == mapArray+mapSize-1){
				cout << "FULL BACKSIDE DEQUE" << endl;
				return;
			}
			
			*(++mapTail) = new T[arraySize];
			tail = *mapTail;
			*tail = v;
			return;
		}
		
		*(++tail) = v;
	}
	
	void pop_front(){
		if(head == nullptr){
			cout << "EMPTY DEQUE" << endl;
			return;
		}
		
		if(head == tail){
			delete[] *mapHead;
			delete[] mapArray;
			head = tail = nullptr;
			mapArray = mapHead = mapTail = nullptr;
			return;
		}
		
		if(head == *mapHead+arraySize-1){
			delete[] *mapHead;
			head = *(++mapHead);
			return;
		}
		
		head++;
	}
	
	void pop_back(){
		if(head == nullptr){
			cout << "EMPTY DEQUE" << endl;
			return;
		}
		
		if(head == tail){
			delete[] *mapHead;
			delete[] mapArray;
			head = tail = nullptr;
			mapArray = mapHead = mapTail = nullptr;
			return;
		}
		
		if(tail == *mapTail){
			delete[] mapTail;
			tail = *(--mapTail)+arraySize-1;
			return;
		}
		
		tail--;
	}
	/*
	T operator[](int index){
	int headMargin, tailMargin, total;
	headMargin = *mapHead+arraySize - head;
	tailMargin = tail-*mapTail+1;
	
	if(mapHead == mapTail) total = tail-head+1;
	else total = headMargin + (mapTail-mapHead-1)*arraySize + tailMargin;
	
	if(index >= total){
	cout << "INDEX OUT OF RANGE" << endl;
	return -1111111111111111111;
	}
	
	if(index < headMargin) return *(head+index);
	
	int leftOver = index-headMargin+1;
	//cout << "LEFTOVER: " << leftOver << endl;
	
	return *(*(mapArray + 1 + ((leftOver-1)/arraySize)) + ((leftOver-1)%arraySize));
	}*/
	T& operator[](int index) {  // Nota el '&' que indica que devuelve una referencia
		int headMargin, tailMargin, total;
		headMargin = *mapHead + arraySize - head;
		tailMargin = tail - *mapTail + 1;
		
		if(mapHead == mapTail) total = tail - head + 1;
		else total = headMargin + (mapTail - mapHead - 1) * arraySize + tailMargin;
		
		if(index >= total) {
			cout << "INDEX OUT OF RANGE" << endl;
			// Esto es problemático porque devolvemos una referencia a un temporal
			// En una implementación real, deberías lanzar una excepción
			static T dummy;
			return dummy;
		}
		
		if(index < headMargin) return *(head + index);
		
		int leftOver = index - headMargin + 1;
		return *(*(mapHead + 1 + ((leftOver - 1) / arraySize)) + ((leftOver - 1) % arraySize));
	}
	
	
	void print(){
		T** mapCurrent = mapHead;
		T* current = head;
		
		cout << "HEAD [";
		
		if(current){
			while(mapCurrent < mapTail){
				cout << " (";
				while(current < *mapCurrent+arraySize){
					cout << " " << *(current++);
				}
				current = *(++mapCurrent);
				cout << " )";
			}
			
			cout << "(";
			while(current <= tail){
				cout << " " << *(current++);
			}
			current = *(++mapCurrent);
			cout << " )";
		}
		
		cout << " ] TAIL" << endl;
	}
};

int main() {
	
	CDeque<int> D;
	cout << D.mapArray-D.mapHead << endl; 
	
	int c = 0;
	
	cout << "PUSH_FRONT" << endl;
	
	for(int i = 0; i < 26; i++, c++) D.push_front(c);
	D.print();
	cout << endl;
	
	cout << "PUSH_BACK" << endl;
	
	for(int i = 0; i < 26; i++, c++) D.push_back(c);
	D.print();
	cout << endl;
	
	cout << "OPERATOR[] ACCESS" << endl;
	for(int i = 0; i < 51; i++) cout << D[i] << " ";
	cout << endl << endl;
	
	cout << "OPERATOR[] REFERENCE" << endl;
	for(int i = 0; i < 51; i++) D[i] = i;
	for(int i = 0; i < 51; i++) cout << D[i] << " ";
	cout << endl << endl;
	/*
	cout << "POP_FRONT" << endl;
	for(int i = 0; i < 52; i++, c++) D.pop_front();
	D.print();
	cout << endl;
	
	cout << "POP_BACK" << endl;
	for(int i = 0; i < 52; i++, c++) D.pop_back();
	D.print();
	cout << endl;
	*/
	return 0;
}
