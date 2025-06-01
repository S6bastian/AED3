//------------------------------------------------------------------------------
//Binary Search Tree
//------------------------------------------------------------------------------

#include <iostream>
#include <cmath>
#include <queue>
#include <stack>
#include <SFML/Graphics.hpp>


using namespace std;

const int WIDTH = 1920, HEIGHT = 1080;
const float RADIUS = 40;

//------------------------------------------------------------------------------
//Node
//------------------------------------------------------------------------------
template<class T>
struct Node {
	T value;
	int level;
	Node<T>* next[2];

	int dist, color;
	float x, y;

	Node(T v, int lvl, float xx, float yy) {
		value = v;
		level = lvl;
		next[0] = next[1] = nullptr;

		dist = WIDTH / (1 << lvl+1);
		color = 0;
		x = xx;
		y = yy;
	}

	void draw(sf::RenderWindow& target, sf::Font& font) const {
		// Text
		sf::Text text(font); // a font is required to make a text object
		// set the string to display
		text.setString(to_string(value));
		// set the character size
		text.setCharacterSize(25); // in pixels, not points!
		// set the color
		text.setFillColor(sf::Color::Red);
		// set the text style
		text.setStyle(sf::Text::Bold | sf::Text::Underlined);
		// Center text (SFML 3.0 compatible)
		auto center = text.getGlobalBounds().size / 2.f;
		auto localBounds = center + text.getLocalBounds().position;

		// Apply round to each component
		sf::Vector2f rounded(std::round(localBounds.x), std::round(localBounds.y));
		text.setOrigin(rounded);
		text.setPosition(sf::Vector2f(x, y));

		// Draw Circle
		sf::CircleShape shape(RADIUS);
		
		shape.setOrigin(sf::Vector2f(RADIUS, RADIUS));
		shape.setPosition(sf::Vector2f(x, y));

		switch (color) {
		case 0:	// Default
			shape.setFillColor(sf::Color::White);
			break;
		case 1:	// Borders
			shape.setFillColor(sf::Color::Blue);
			break;
		case 2:	// Uncles
			shape.setFillColor(sf::Color::Cyan);
			break;
		case 3:	// Cousins
			shape.setFillColor(sf::Color::Green);
			break;
		}


		// Draw Lines
		sf::Vertex line1[2];
		line1[0].position = sf::Vector2f(x, y);
		line1[0].color = sf::Color::White;
		line1[1].position = sf::Vector2f(x - dist, y + RADIUS * 4);
		line1[1].color = sf::Color::White;

		sf::Vertex line2[2];
		line2[0].position = sf::Vector2f(x, y);
		line2[0].color = sf::Color::White;
		line2[1].position = sf::Vector2f(x + dist, y + RADIUS * 4);
		line2[1].color = sf::Color::White;


		target.draw(line1, 2, sf::PrimitiveType::Lines);
		target.draw(line2, 2, sf::PrimitiveType::Lines);
		target.draw(shape);
		target.draw(text);
	}
};

//------------------------------------------------------------------------------
//BSTree
//------------------------------------------------------------------------------
template<class T>
struct BSTree {
	Node<T>* root;
	bool side;
	queue<Node<T>*> modified;

	BSTree() {
		root = nullptr;
		side = 0;
	}

	bool find(T v, Node<T>**& pos, int& lvl, float& x, float& y) {
		float dist = WIDTH / 2;
		x = WIDTH / 2;
		y = RADIUS + 10;

		for (pos = &root, lvl = 1;
			*pos != nullptr && v != (*pos)->value;
			lvl++,
			pos = &(*pos)->next[(*pos)->value < v]) {

			dist /= 2;
			if ((*pos)->value < v) x += dist;
			else x -= dist;
			y += RADIUS * 4;
		}
		return *pos != nullptr;
	}

	bool insert(T v) {
		Node<T>** pos;
		int lvl;
		float x, y;

		if (find(v, pos, lvl, x, y)) return false;
		
		*pos = new Node<T>(v, lvl, x, y);
		return true;
	}

	Node<T>** rep(Node<T>** target) { //replace
		target = &(*target)->next[side];
		side = !side;

		while ((*target)->next[side] != nullptr) {
			target = &(*target)->next[side];
		}

		return target;
	}

	bool remove(T v) {
		Node<T>** pos;
		int lvl;
		float x, y;
		if (!find(v, pos, lvl, x, y)) return false;
		//Node<T>* tmp = *pos

		if ((*pos)->next[0] && (*pos)->next[1]) {
			Node<T>** tmp = rep(pos);
			(*pos)->value = (*tmp)->value;

			pos = tmp;
		}

		Node<T>* tmp = *pos;

		*pos = (*pos)->next[(*pos)->next[1] != nullptr];
		delete[] tmp;

		return true;
	}

	void printPreorder() {
		printPreorder(root);
	}

	void printPreorder(Node<T>* nroot) {
		if (nroot == nullptr) return;
		cout << nroot->value << " ";
		printPreorder(nroot->next[0]);
		printPreorder(nroot->next[1]);
	}


	void printInorder() {
		printInorder(root);
	}

	void printInorder(Node<T>* nroot) {
		if (nroot == nullptr) return;
		printInorder(nroot->next[0]);
		cout << nroot->value << " ";
		printInorder(nroot->next[1]);
	}

	void printPosorder() {
		printPosorder(root);
	}

	void printPosorder(Node<T>* nroot) {
		if (nroot == nullptr) return;
		printPosorder(nroot->next[0]);
		printPosorder(nroot->next[1]);
		cout << nroot->value << " ";
	}

	void printLevel() {
		printLevel(root);
	}

	void printLevel(Node<T>* nroot) {
		queue<Node<T>*> q;
		q.push(nroot);
		while (!q.empty()) {
			cout << q.front()->value << " ";
			if (q.front()->next[0] != nullptr) q.push(q.front()->next[0]);
			if (q.front()->next[1] != nullptr) q.push(q.front()->next[1]);
			q.pop();
		}
		cout << "\n";
	}

	void printInorderStack() {
		printInorderStack(root);
	}

	void printInorderStack(Node<T>* nroot) {
		stack<pair<Node<T>*, int>> s;
		s.push({ nroot,0 });
		while (!s.empty()) {
			pair<Node<T>*, int>& x = s.top();

			switch (x.second) {
			case 0:
				if (x.first->next[0] != nullptr) s.push({ x.first->next[0],0 });
				break;
			case 1:
				cout << x.first->value << " ";
				break;
			case 2:
				if (x.first->next[1] != nullptr) s.push({ x.first->next[1],0 });
				break;
			case 3:
				s.pop();
				break;
			}

			x.second++;
		}
		cout << "\n";
	}

	int height() {
		return height(root);
	}

	int height(Node<T>* nroot) {
		if (!nroot) return 0;
		int l = height(nroot->next[0]);
		int r = height(nroot->next[1]);

		return (l > r ? l : r) + 1;
	}

	void draw(sf::RenderWindow& target, sf::Font& font) {
		draw(root, target, font);
	}

	void draw(Node<T>* nroot, sf::RenderWindow& target, sf::Font& font) {
		if (!nroot) return;
		nroot->draw(target, font);
		draw(nroot->next[0], target, font);
		draw(nroot->next[1], target, font);
	}


	void paintBorders() {
		paintReset();
		Node<T>* tmp = root;
		modified.push(tmp);

		while (tmp) {
			modified.push(tmp);
			tmp->color = 1;
			tmp = tmp->next[0];
		}

		tmp = root->next[1];

		while (tmp) {
			modified.push(tmp);
			tmp->color = 1;
			tmp = tmp->next[1];
		}
	}

	void paintUnclesAndCousins(T v) {
		paintReset();
		queue<Node<T>*> uncles, cousins;

		Node<T>* tmp = root, * parent = nullptr;

		while (tmp != nullptr && tmp->value != v) {
			parent = tmp;
			tmp = tmp->next[tmp->value < v];
		}

		if (!tmp) return;

		int tLvl = tmp->level;

		uncles.push(root);

		while (uncles.front()->level < tLvl - 1) {
			if (uncles.front()->next[0]) uncles.push(uncles.front()->next[0]);
			if (uncles.front()->next[1]) uncles.push(uncles.front()->next[1]);
			uncles.pop();
		}

		while (!uncles.empty()) {
			uncles.front()->color = 2;
			modified.push(uncles.front());
			if (uncles.front()->next[0]) {
				uncles.front()->next[0]->color = 3;
				modified.push(uncles.front()->next[0]);
			}
			if (uncles.front()->next[1]) {
				uncles.front()->next[1]->color = 3;
				modified.push(uncles.front()->next[1]);
			}
			uncles.pop();
		}

		if (parent) {
			parent->color = 0;
			parent->next[0]->color = 0;
			parent->next[1]->color = 0;
		}
	}

	void paintReset() {
		while (!modified.empty()) {
			modified.front()->color = 0;
			modified.pop();
		}
	}
};

//------------------------------------------------------------------------------
// Main
//------------------------------------------------------------------------------

int main() {

	BSTree<int> BT;
	// Insertions to form a complete binary tree of 4 levels(15 nodes)
	// Level 1 (Root)
	BT.insert(50);

	// Level 2
	BT.insert(30);
	BT.insert(70);

	// Level 3
	BT.insert(20);
	BT.insert(40);
	BT.insert(60);
	BT.insert(80);

	// Level 4
	BT.insert(10);
	BT.insert(25);
	BT.insert(35);
	BT.insert(45);
	BT.insert(55);
	BT.insert(65);
	BT.insert(75); // Using 75 instead of 85 to ensure 70's right child is 75, and 80's left child is 75. Oh wait, this is a BST.
	// Let's use 85 and 95 for right side to maintain BST property.
	BT.insert(90);

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

	BT.paintBorders();
	//BT.paintUnclesAndCousins(75);

	sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "SFML works!");

	// Font
	sf::Font font("arial.ttf");
	sf::Text text(font); // a font is required to make a text object

	while (window.isOpen()) {
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();
		}

		window.clear();
		BT.draw(window, font);
		window.display();

	}

	return 0;
}
