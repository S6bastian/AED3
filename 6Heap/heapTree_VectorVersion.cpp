#include<iostream>
#include<vector> 
#include<algorithm>

using namespace std;

template<class T, class Container = vector<T>, class Compare = less<T>>
struct Heap{
	Container storage;
	
	Heap<T,Container,Compare>(){};
	
	T top(){
		if(storage.empty()) return -451;
		
		return storage[0];
	}
	
	void push(T v){
		storage.push_back(v);
		int index = storage.size()-1;
		
		while(index){
			if(Compare()(storage[(index-1)/2],storage[index])) swap(storage[(index-1)/2],storage[index]);
			else break;
			index = (index-1)/2;
		}
	}
	
	void pop(){
		if(storage.empty()) return;
		
		storage[0] = storage.back();
		storage.pop_back();
		
		int index = 0, limit = storage.size();
		while(index < limit){
			int mx;
			if(index*2+2 >= limit){
				if(index*2+1 >= limit) break;
				else mx = index*2+1; 
			}
			else mx = (Compare()(storage[index*2+1],storage[index*2+2]) ? index*2+2 : index*2+1);
			
			if(Compare()(storage[index],storage[mx])) swap(storage[index],storage[mx]);
			else return;
			
			index = mx;
		} 
	}
	
	void print(){
		if(storage.empty()) return;
		for(auto u : storage) cout << u << " ";
		cout << "\n";
	}
};

int main() {
	Heap<int> Test;
	
	for(int i = 0; i < 10; i++){
		Test.push(i);
		cout << "TEST " << i << "\n";
		cout << "TOP " << Test.top() << "\n";
		Test.print();
	}
	
	Test.push(-10);
	Test.print();
	Test.push(10);
	Test.print();
	
	cout << "POPPING\n";
	
	Test.pop();
	Test.print();
	Test.pop();
	Test.print();
	return 0;
}

