#include <iostream>
using namespace std;

template<class T>
struct SMAux;


template<class T>
struct Node{
	T value;
	int x, y;
	Node<T>* next[2]; // next[0] next row, next[1] next column;
	
	Node(T v, int cX, int cY, Node<T>* nX = nullptr, Node<T>* nY = nullptr){
		value = v;
		x = cX;
		y = cY;
		next[0] = nY;
		next[1] = nX;
	}
};

template<class T>
struct SMatrix{
	int rows, columns; 
	Node<T>** xAxis, ** yAxis;
	
	SMatrix(int sizeY, int sizeX){        // Rows and columns 
		rows = sizeY;
		columns = sizeX;
		xAxis = new Node<T>*[sizeY];                // Column[row]
		yAxis = new Node<T>*[sizeX];                // Row[colum]
		
		for(int i = 0; i < rows; i++) *(xAxis+i) = nullptr;
		for(int i = 0; i < columns; i++) *(yAxis+i) = nullptr;
	}
		
	bool findX(int x, int y, Node<T>** &p){ // Finds the X axis pointer to Node
		//cout << "DDD \n";
		for(p = &xAxis[y]; *p != nullptr && (*p)->x < x; p = &(*p)->next[1]);
		//cout << *p << " <<<<\n";
		return *p != nullptr && (*p)->x == x;
	}
	
	bool findY(int x, int y, Node<T>** &p){ // Finds the Y axis pointer to Node
		for(p = &yAxis[x]; *p != nullptr && (*p)->y < y; p = &(*p)->next[0]);
		return *p && (*p)->y == y;
	}
	
	T get(int x, int y){
		Node<T>** p;
		
		if(!findX(x,y,p)) return 0;
		
		return (*p)->value;
	}
	
	void set(int x, int y, T v){
		//print();
		Node<T>** p;
		if(findX(x,y,p)){
			//cout << "FOUND .l.\n";
			if(!v){                                        // Only if v == 0, removes Node
				Node<T>* tmp = *p;
				*p = (*p)->next[1];
				findY(x,y,p);
				*p = (*p)->next[0];
				delete[] tmp;
				return;
			}
			
			(*p)->value = v;
			return;
		}
		
		Node<T>* tmp = *p = new Node<T>(v,x,y,*p);
		
		findY(x,y,p);
		tmp->next[0] = *p;
		*p = tmp;
	}
	
	SMAux<T> operator()(int x, int y){         // We could use a referece to make this but the problem with this is that if we want to assign a value or get access to the value and it doesn't exist program will crash
		return SMAux<T>(x,y,this);
		
	}
	
	void print(){
		cout << "------------------------------------------------------------------------------------------------------------------------\n";
		for(int i = 0; i < columns; i++) cout << "\t" << i;
		cout << "\n";
		for(int i = 0; i < columns; i++) cout << "\t|";
		cout << "\n";
		for(int i = 0; i < columns; i++) cout << "\tv";
		cout << "\n";
		
		for(int j = 0; j < rows; j++){
			cout << j << " ->";
			Node<T>* current = xAxis[j];
			for(int i = 0; i < columns; i++){
				if(!current || current->x != i){
					cout << "\t0";
					continue;
				}
				cout << "\t" << current->value;
				current = current->next[1];
			}
			cout << "\n";
		}
		cout << "------------------------------------------------------------------------------------------------------------------------\n";
	}
};

template<class T>
struct SMAux{
	SMatrix<T>* psmaux;
	int x, y;
	
	SMAux(int cx, int cy, SMatrix<T>* psm){
		x = cx;
		y = cy;
		psmaux = psm;
	}
	
	T operator =(T v){
		psmaux->set(x,y,v);
		return v;
	}
	operator T(){
		return psmaux->get(x,y);
	}
};

int main() {
	SMatrix<int> m(4,4);
	
	m.set(0,1,7);
	//cout << "TEST \n";
	m.set(1,2,5);
	m.set(2,1,3);
	m.set(3,1,20);
	
	m.print();
	m.set(2,1,0);
	m.print();
	
	m(0,1) = 17;
	m(3,3) = 6;
	
	int a, b;
	a = m(0,1);
	b = m(3,3);
	m.print();
	cout << a << " " << b << "\n";
	
	m(2,0) = m(2,3) = 250;
	m.print();
	
	return 0;
}
