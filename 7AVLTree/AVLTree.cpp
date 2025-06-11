#include <iostream>
#include <stack>
using namespace std;

template<class T>
struct Node{
	T value;
	Node<T>* next[2];
	int height;
	
	Node(int v){
		value = v;
		height = 1;
		next[0] = next[1] = nullptr;
	}
};

template<class T>
struct AVLTree{
	Node<T>* root;
	
	AVLTree(){
		root = nullptr;
	}
	
	T get_successor(Node<T>**& p, stack<Node<T>**>& backtrack){
		backtrack.push(p);
		p = &(*p)->next[0];
		
		while((*p)->next[1]){
			backtrack.push(p);
			p = &(*p)->next[1];
		}
		return (*p)->value;
	}
		
	int get_difference(Node<T>* parent){
		int left = (parent->next[0] ? parent->next[0]->height : 0);
		int right = (parent->next[1] ? parent->next[1]->height : 0);
		return right - left;
	}
		
	void update_height(Node<T>* parent){
		int left = (parent->next[0] ? parent->next[0]->height : 0);
		int right = (parent->next[1] ? parent->next[1]->height : 0);
		parent->height = (right > left ? right : left) + 1;
	}
		
	bool find(T v, Node<T>**& p, stack<Node<T>**>& backtrack){
		for(p = &root; *p && (*p)->value; backtrack.push(p), p = &(*p)->next[(*p)->value < v]);
		return *p;
	}
		
	void balance(Node<T>** parent){
		int diff = get_difference(*parent);
		bool side = (diff > 0 ? 1 : 0);
		
		if(diff * get_difference((*parent)->next[side]) > 0){ // LL(0) - RR(1)
			Node<T>* tmp = *parent;
			
			*parent = (*parent)->next[side];
			tmp->next[side] = (*parent)->next[!side];
			(*parent)->next[!side] = tmp;
		}
		else{
			Node<T>* tmp = *parent;
			
			*parent = (*parent)->next[side]->next[!side];
			tmp->next[side]->next[!side] = (*parent)->next[side];
			(*parent)->next[side] = tmp->next[side];
			tmp->next[side] = (*parent)->next[!side];
			(*parent)->next[!side] = tmp;
		}
		
		update_height(*parent);
		update_height((*parent)->next[0]);
		update_height((*parent)->next[1]);
	}
	
	bool insert(T v){
		stack<Node<T>**> backtrack;
		Node<T>** p;
		
		if(find(v,p,backtrack)) return false;
		
		*p = new Node<T>(v);
		
		while(!backtrack.empty()){
			int diff = get_difference(*backtrack.top());
			if(diff == 2 || diff == -2) balance(backtrack.top());
			update_height(*backtrack.top());
			backtrack.pop();
		}
	}
		
	bool remove(T v){
		stack<Node<T>**> backtrack;
		Node<T>** p, ** tmp;
		
		if(!find(v,p,backtrack)) return false;
		
		tmp = p;
		(*p)->value = get_successor(tmp,backtrack);
		delete[] tmp;
		
		
		while(!backtrack.empty()){
			int diff = get_difference(*backtrack.top());
			if(diff == 2 || diff == -2) balance(backtrack.top());
			update_height(*backtrack.top());
			backtrack.pop();
		}
	}
	
};

int main() {
	
	return 0;
}

