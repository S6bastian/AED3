//------------------------------------------------------------------------------
//Binary Search Tree
//------------------------------------------------------------------------------

#include <iostream>
#include <queue>
#include <stack>


using namespace std;

//------------------------------------------------------------------------------
//Node
//------------------------------------------------------------------------------
template<class T>
struct Node{
	T value;
	Node<T>* next[2];
	
	Node(T v){
		value = v;
		next[0] = next[1] = nullptr;
	}
		
};

//------------------------------------------------------------------------------
//BSTree
//------------------------------------------------------------------------------
template<class T>
struct BSTree{
	Node<T>* root;
	bool side;
	
	BSTree(){
		root = nullptr;
		side = 0;
	}
		
	bool find(T v, Node<T>** &pos){
		for(pos = &root;
		*pos != nullptr && v != (*pos)->value;
		pos = &(*pos)->next[(*pos)->value < v]);
		return *pos != nullptr;
	}
	
	bool insert(T v){
		Node<T>** pos;
		if(!find(v,pos)){
			*pos = new Node<T>(v);
			return true;
		}
		return false;
	}
	
	Node<T>** rep(Node<T>** target){ //replace
		target = &(*target)->next[side];
		side = !side;
		
		while((*target)->next[side] != nullptr){
			target = &(*target)->next[side];
		}
		
		return target;
	}
	
	bool remove(T v){
		Node<T>** pos;
		if(!find(v,pos)) return false;
		//Node<T>* tmp = *pos
		
		if((*pos)->next[0] && (*pos)->next[1]){
			Node<T>** tmp = rep(pos);
			(*pos)->value = (*tmp)->value;
			
			pos = tmp;
		}
		
		Node<T>* tmp = *pos;
		
		*pos = (*pos)->next[(*pos)->next[1] != nullptr];
		delete[] tmp;
		
		return true;
	}
	
	void printPreorder(){
		printPreorder(root);
	}
	
	void printPreorder(Node<T>* nroot){
		if(nroot == nullptr) return;
		cout << nroot->value << " ";
		printPreorder(nroot->next[0]);
		printPreorder(nroot->next[1]);
	}
		
	
	void printInorder(){
		printInorder(root);
	}
	
	void printInorder(Node<T>* nroot){
		if(nroot == nullptr) return;
		printInorder(nroot->next[0]);
		cout << nroot->value << " ";
		printInorder(nroot->next[1]);
	}
	
	void printPosorder(){
		printPosorder(root);
	}
	
	void printPosorder(Node<T>* nroot){
		if(nroot == nullptr) return;
		printPosorder(nroot->next[0]);
		printPosorder(nroot->next[1]);
		cout << nroot->value << " ";
	}
		
	void printLevel(){
		printLevel(root);
	}
		
	void printLevel(Node<T>* nroot){
		queue<Node<T>*> q;
		q.push(nroot);
		while(!q.empty()){
			cout << q.front()->value << " ";
			if(q.front()->next[0] != nullptr) q.push(q.front()->next[0]);
			if(q.front()->next[1] != nullptr) q.push(q.front()->next[1]);
			q.pop();
		}
		cout << "\n";
	}
		
	void printInorderStack(){
		printInorderStack(root);
	}
		
	void printInorderStack(Node<T>* nroot){
		stack<pair<Node<T>*,int>> s;
		s.push({nroot,0});
		while(!s.empty()){
			pair<Node<T>*,int>& x = s.top();
			
			switch(x.second){
			case 0:
				if(x.first->next[0] != nullptr) s.push({x.first->next[0],0});
				break;
			case 1:
				cout << x.first->value << " ";
				break;
			case 2:
				if(x.first->next[1] != nullptr) s.push({x.first->next[1],0});
				break;
			case 3:
				s.pop();
				break;
			}
			
			x.second++;
		}
		cout << "\n";
	}
	
	int height(){
		return height(root);
	}
		
	int height(Node<T>* nroot){
		if(!nroot) return 0;
		int l = height(nroot->next[0]);
		int r = height(nroot->next[1]);
		
		return (l > r ? l : r)+1;
	}
};

//------------------------------------------------------------------------------
// Main
//------------------------------------------------------------------------------

int main(){
	
	BSTree<int> BT;
	/*
	BT.insert(15);
	BT.insert(9);
	BT.insert(20);
	BT.insert(6);
	BT.insert(14);
	BT.insert(17);
	BT.insert(64);
	BT.insert(13);
	BT.insert(26);
	BT.insert(72);
	*/
	
	BT.insert(50);
	BT.insert(30);
	BT.insert(80);
	BT.insert(90);
	BT.insert(60);
	BT.insert(70);
	
	BT.remove(50);
	
	cout << "Preorder: ";
	BT.printPreorder();
	cout << endl << endl;;
	
	cout << "Inorder: ";
	BT.printInorder();
	cout << endl << endl;;
	
	cout << "Posorder: ";
	BT.printPosorder();	
	cout << endl << endl;
	
	cout << "Level: ";
	BT.printLevel();
	cout << "\n\n";
	
	cout << "InorderStack: ";
	BT.printInorderStack();
	cout << "\n\n";
	
	cout << "Height: " << BT.height() << "\n";
	
	return 0;
}
