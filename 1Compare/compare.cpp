// https://docs.google.com/spreadsheets/d/1kZqEj5hn3Nc48gmwOIvRv7mk9fkfvFr3VKeqAjgA8zg/edit?usp=sharing

#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>

using namespace std;
using namespace chrono;

// ---------------------------------------------------------------------
// Polymorphism
// ---------------------------------------------------------------------

template<class T>
struct PCompare{
	virtual bool cmp(T a, T b) = 0;
	virtual ~PCompare() {};
};

template<class T>
struct PGreater : PCompare<T>{
	bool cmp(T a, T b){return a > b;}
};

template<class T>
struct PLesser : PCompare<T>{
	bool cmp(T a, T b){return a < b;}
};
	
// ---------------------------------------------------------------------
// Function Object
// ---------------------------------------------------------------------

template<class T>
struct FOGreater{
	bool operator()(T a, T b){return a > b;}
};

template<class T>
struct FOLesser{
	bool operator()(T a, T b){return a < b;}
};

// ---------------------------------------------------------------------
// Function Pointer
// ---------------------------------------------------------------------

template<class T>
bool Gcmp(T a, T b){return a > b;}

template<class T>
bool Lcmp(T a, T b){return a < b;}

template<class T>
bool (*FPGreater)(T a, T b) = Gcmp;

template<class T>
bool (*FPLesser)(T a, T b) = Lcmp;

// ---------------------------------------------------------------------

template<class T>
struct PAJAROTE{	//BubbleSort Methods
	
	void print(vector<T>& array, int n){
		for(int i = 0; i < n; i++){
			cout << array[i] << " ";
		}
		cout << endl;
	}
	
	// Direct TV
	void bubbleSort(vector<int> array, int n){
		auto start = high_resolution_clock::now();
		
		for(int i = 0; i < n; i++){
			for(int j = 1;  j < n; j++){
				if(array[j-1] < array[j]) swap(array[j-1],array[j]);
			}
		}
		
		auto end = high_resolution_clock::now();
		duration<double> duration_ms = end - start;
		
		//print(array, n);
		
		cout << "Direct TV Time: " << duration_ms.count() << " ms" << endl;
	}
	
	// Polymorphism
	void PbubbleSort(vector<int> array, int n){
		PCompare<T>* C = new PLesser<T>;
		
		auto start = high_resolution_clock::now();
		
		/*
		PGreater<T> G;
		C = &G;
		*/
		
		for(int i = 0; i < n; i++){
			for(int j = 1;  j < n; j++){
				if(C->cmp(array[j-1],array[j])) swap(array[j-1],array[j]);
			}
		}
		
		auto end = high_resolution_clock::now();
		duration<double> duration_ms = end - start;
		
		//print(array, n);
		delete C;
		
		cout << "Polymorphism Time: " << duration_ms.count() << " ms" << endl;
	}
	
	// Function Object
	void FObubbleSort(vector<int> array, int n){
		FOLesser<T> C;
		
		auto start = high_resolution_clock::now();
		
		for(int i = 0; i < n; i++){
			for(int j = 1;  j < n; j++){
				if(C(array[j-1],array[j])) swap(array[j-1],array[j]);
			}
		}
		
		auto end = high_resolution_clock::now();
		duration<double> duration_ms = end - start;
		
		//print(array, n);
		
		cout << "Function Object Time: " << duration_ms.count() << " ms" << endl;
	}
		
	// Function Pointer
	void FPbubbleSort(vector<int> array, int n){
		auto start = high_resolution_clock::now();
		
		for(int i = 0; i < n; i++){
			for(int j = 1;  j < n; j++){
				if(FPLesser<T>(array[j-1],array[j])) swap(array[j-1],array[j]);
			}
		}
		
		auto end = high_resolution_clock::now();
		duration<double> duration_ms = end - start;
		
		//print(array, n);
		
		cout << "Function Pointer Time: " << duration_ms.count() << " ms" << endl;
	}
};

template<class T>
void solve(vector<int> array, int n){
	
	PAJAROTE<T> Methods;
	
	cout << "Polymorphism"<< endl;
	Methods.PbubbleSort(array, n);
	cout << endl << endl;
	
	cout << "Function Pointer"<< endl;
	Methods.FPbubbleSort(array, n);
	cout << endl << endl;
	
	cout << "Function Object"<< endl;
	Methods.FObubbleSort(array, n);
	cout << endl << endl;
	
	cout << "Direct TV"<< endl;
	Methods.bubbleSort(array, n);
	cout << endl << endl;
}

int main(){
	int n = 1000000;
	//cin >> n;
	
	vector<int> array(n);
	for(int i = 0; i < n; i++){
		array[i] = i;
	}
	
	while(true){
		cin >> n;
		solve<int>(array, n);
	}
	return 0;
}
