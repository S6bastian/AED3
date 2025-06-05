#include <iostream>
#include <algorithm>
#include <functional>
using namespace std;

template<class T>
struct Node{
	T value;
	Node<T>* next[3]; // child 1, child 2, parent
	Node<T>* side[2]; // before, after
	Node(T v, Node<T>* nBefore = nullptr, Node<T>* nParent = nullptr){ // value, node before, parent node
		value = v;
		side[1] = next[0] = next[1] = nullptr;
		next[2] = nParent;
		side[0] = nBefore;
	}
};


template<class T, class Compare = greater <T>>
struct Heap{
	Node<T>* root, * leftMost, * rightMost, * current;
	
	Heap(){
		root = nullptr;
		leftMost = rightMost = current = nullptr;
	}
	
	bool empty(){
		return (root == nullptr);
	}
		
	T top(){
		return root->value;
	}
		
	void push(T v){
		if(root == nullptr){
			root = new Node<T>(v);
			leftMost = rightMost = current = root;
			return;
		}
		
		if(current == rightMost){
			current = leftMost->next[0] = new Node<T>(v,rightMost,leftMost);
			rightMost->side[1] = leftMost = current;
		}
		else if(current == current->next[2]->next[0]){
			current = current->side[1] = current->next[2]->next[1] = new Node<T>(v,current,current->next[2]);
			if(current->next[2] == rightMost) rightMost = current;	
		}
		else{
			current = current->side[1] = current->next[2]->side[1]->next[0] = new Node<T>(v,current,current->next[2]->side[1]);
		}
		
		Node<T>* tmp = current;
		
		while(tmp->next[2]){
			if(Compare()(tmp->value,tmp->next[2]->value)){
				swap(tmp->value,tmp->next[2]->value);
				tmp = tmp->next[2];
			}
			else break;
		}
	}
		
	void pop(){
		if(current == root){
			delete[] root;
			root = current = leftMost = rightMost = nullptr;
		}
		
		if(current == leftMost) leftMost = leftMost->next[2];
		else if(current == rightMost) rightMost = rightMost->next[2];
		
		Node<T>* tmp = current;
		current = current->side[0];
		
		if(tmp == tmp->next[2]->next[1]) tmp->next[2]->next[1] = nullptr;
		else tmp->next[2]->next[0] = nullptr;
		
		current->side[1] = nullptr;
		
		root->value = tmp->value;
		
		delete[] tmp;
		
		tmp = root;
		while(tmp->next[0] && tmp->next[1]){
			Node<T>* mx;
			if(Compare()(tmp->next[0]->value,tmp->next[1]->value)) mx = tmp->next[0];
			else mx = tmp->next[1];
			
			if(Compare()(mx->value,tmp->value)) swap(mx->value,tmp->value);
			else break;
			
			tmp = mx;
		}
		
		if(tmp->next[0] && Compare()(tmp->next[0]->value,tmp->value)) swap(tmp->next[0]->value,tmp->value);
	}
		
	void print(){
		Node<T>* tmp = root;
		while(tmp){
			cout << tmp->value << " ";
			tmp = tmp->side[1];
		}
		cout << "\n";
	}
};

int main() {
	Heap<int,less<int>> Test;
	
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

