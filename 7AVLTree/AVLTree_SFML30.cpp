#include <iostream>
#include <stack>
#include <vector>
#include <queue>
#include <utility>
#include <list>
#include <string>
#include <iterator> // For std::prev
#include <math.h>
#include <SFML/Graphics.hpp>

using namespace std;

const int WIDTH = 1920, HEIGHT = 1080;
const float RADIUS = 40;

// ------------------------------------------------------------------------------
// Node
// ------------------------------------------------------------------------------

template<class T>
struct Node {
	T value;
	Node<T>* next[2];
	int height;

	Node(int v) {
		value = v;
		height = 1;
		next[0] = next[1] = nullptr;
	}
};

// ------------------------------------------------------------------------------
// AVLTree
// ------------------------------------------------------------------------------

template<class T>
struct AVLTree {
	Node<T>* root;

	AVLTree() {
		root = nullptr;
	}

	T get_successor(Node<T>**& p, stack<Node<T>**>& backtrack) {
		backtrack.push(p);
		p = &(*p)->next[0];

		while ((*p)->next[1]) {
			backtrack.push(p);
			p = &(*p)->next[1];
		}
		return (*p)->value;
	}

	int get_difference(Node<T>* parent) {
		int left = (parent->next[0] ? parent->next[0]->height : 0);
		int right = (parent->next[1] ? parent->next[1]->height : 0);
		return right - left;
	}

	void update_height(Node<T>* parent) {
		int left = (parent->next[0] ? parent->next[0]->height : 0);
		int right = (parent->next[1] ? parent->next[1]->height : 0);
		parent->height = (right > left ? right : left) + 1;
	}

	bool find(T v, Node<T>**& p, stack<Node<T>**>& backtrack) {
		for (p = &root; *p && (*p)->value != v; backtrack.push(p), p = &(*p)->next[(*p)->value < v]);
		return *p;
	}

	void balance(Node<T>** parent) {
		int diff = get_difference(*parent);
		bool side = (diff > 0 ? 1 : 0);

		if (diff * get_difference((*parent)->next[side]) > 0) { // LL(0) - RR(1)
			Node<T>* tmp = *parent;

			*parent = (*parent)->next[side];
			tmp->next[side] = (*parent)->next[!side];
			(*parent)->next[!side] = tmp;
		}
		else {
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

	bool insert(T v) {
		stack<Node<T>**> backtrack;
		Node<T>** p;

		if (find(v, p, backtrack)) return false;

		*p = new Node<T>(v);

		while (!backtrack.empty()) {
			int diff = get_difference(*backtrack.top());
			if (diff == 2 || diff == -2) balance(backtrack.top());
			update_height(*backtrack.top());
			backtrack.pop();
		}
		return true;
	}

	bool remove(T v) {
		stack<Node<T>**> backtrack;
		Node<T>** p, ** tmp;

		if (!find(v, p, backtrack)) return false;

		tmp = p;
		(*p)->value = get_successor(tmp, backtrack);
		delete[] tmp;


		while (!backtrack.empty()) {
			int diff = get_difference(*backtrack.top());
			if (diff == 2 || diff == -2) balance(backtrack.top());
			update_height(*backtrack.top());
			backtrack.pop();
		}
		return true;
	}


	void bfs(list<Node<T>*>*& lvlNodes) {
		list<Node<T>*>* newList = new list<Node<T>*>;

		for (Node<T>* current; !lvlNodes->empty(); lvlNodes->pop_front()) {
			current = lvlNodes->front();

			if (current) {
				newList->push_back(current->next[0]);
				newList->push_back(current->next[1]);
			}
			else {
				newList->push_back(nullptr);
				newList->push_back(nullptr);
			}

		}
		delete lvlNodes;
		lvlNodes = newList;

	}

	void print() {
		if (root == nullptr) {
			cout << "Empty Tree" << endl;
			return;
		}

		list<Node<T>*>* lvlNodes = new list<Node<T>*>;
		lvlNodes->push_back(root);
		cout << "-print()-" << endl;
		int digits = 3;

		for (int height = lvlNodes->front()->height, margin, space; height > 0; height--) {

			margin = (pow(2, height - 1) - 1) * digits;
			for (int i = 0; i < margin; i++) cout << " ";

			space = (pow(2, height) - 1) * 3;
			for (auto place = lvlNodes->begin(); place != lvlNodes->end(); ++place) {
				int offSpace = digits - ((*place) ? to_string((*place)->value).length() : 1);
				for (; offSpace > 0; cout << " ", --offSpace);

				if (*place)
					cout << (*place)->value;
				else
					cout << "N";

				if (place != prev(lvlNodes->end())) {
					for (int j = 0; j < space; j++) cout << " ";
				}
			}

			cout << endl;

			if (height != 1)
				bfs(lvlNodes);  // updates node queue
		}

		cout << endl;
	}

	void draw(sf::RenderWindow& target, sf::Font& font) const {
		// Text
		sf::Text text(font); // a font is required to make a text object
		// set the string to display
		text.setString(to_string(root->value));
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
		//text.setPosition(sf::Vector2f(x, y));

		// Draw Circle
		sf::CircleShape shape(RADIUS);

		shape.setOrigin(sf::Vector2f(RADIUS, RADIUS));
		//shape.setPosition(sf::Vector2f(x, y));
		shape.setFillColor(sf::Color::White);

		// Draw Lines
		sf::Vertex line1[2];
		//line1[0].position = sf::Vector2f(x, y);
		line1[0].color = sf::Color::White;
		//line1[1].position = sf::Vector2f(x - dist, y + RADIUS * 4);
		line1[1].color = sf::Color::White;

		sf::Vertex line2[2];
		//line2[0].position = sf::Vector2f(x, y);
		line2[0].color = sf::Color::White;
		//line2[1].position = sf::Vector2f(x + dist, y + RADIUS * 4);
		line2[1].color = sf::Color::White;


		queue<pair<Node<T>*, vector<float>>> q;
		q.push({ root, {WIDTH / 2, RADIUS * 2, WIDTH / 4} }); // node, x, y, dist

		while (!q.empty()) {
			float x, y, dist;
			x = q.front().second[0];
			y = q.front().second[1];
			dist = q.front().second[2];

			if (q.front().first->next[0]) q.push({ q.front().first->next[0], {x - dist, y + (RADIUS * 4), dist / 2} });
			if (q.front().first->next[1]) q.push({ q.front().first->next[1], {x + dist, y + (RADIUS * 4), dist / 2} });

			text.setString(to_string(q.front().first->value));
			text.setPosition(sf::Vector2f(x, y));
			shape.setPosition(sf::Vector2f(x, y));

			line1[0].position = sf::Vector2f(x, y);
			line1[1].position = sf::Vector2f(x - dist, y + RADIUS * 4);
			line2[0].position = sf::Vector2f(x, y);
			line2[1].position = sf::Vector2f(x + dist, y + RADIUS * 4);

			target.draw(line1, 2, sf::PrimitiveType::Lines);
			target.draw(line2, 2, sf::PrimitiveType::Lines);
			target.draw(shape);
			target.draw(text);

			q.pop();
		}

		/* LEVEL PRINT NOT USEFULL WITH AVL 
		queue<pair<Node<T>*,int>> q;
		q.push({root,1});

		float x, y;
		int dist, lvl;
		x = WIDTH / 2;
		y = 0;
		dist = WIDTH / 4; 
		lvl = 0;

		
		while (!q.empty()) {
			if (q.front().first->next[0]) q.push({ q.front().first->next[0], q.front().second + 1 });
			if (q.front().first->next[1]) q.push({ q.front().first->next[1], q.front().second + 1 });
			
			if (q.front().second != lvl) {
				lvl++;
				x = WIDTH / (1 << lvl);
				y += RADIUS * 4;
				dist = WIDTH / (1 << lvl + 1);

			}
			else x += WIDTH / (1 << lvl - 1);

			text.setString(to_string(q.front().first->value));
			text.setPosition(sf::Vector2f(x, y));
			shape.setPosition(sf::Vector2f(x, y));

			line1[0].position = sf::Vector2f(x, y);
			line1[1].position = sf::Vector2f(x - dist, y + RADIUS * 4);
			line2[0].position = sf::Vector2f(x, y);
			line2[1].position = sf::Vector2f(x + dist, y + RADIUS * 4);

			target.draw(line1, 2, sf::PrimitiveType::Lines);
			target.draw(line2, 2, sf::PrimitiveType::Lines);
			target.draw(shape);
			target.draw(text);

			q.pop();
		}
		*/
		

		
	}

};

// ------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------

int main() {
	AVLTree<int> Test;
	for (int i = 100; i <= 200; i += 10) Test.insert(i);
	Test.print();

	for (int i = 100; i >= 0; i -= 10) {
		Test.insert(i);
		cout << i << "\n";
		Test.print();
	}
	Test.print();


	// SFML

	sf::RenderWindow window(sf::VideoMode({ WIDTH, HEIGHT }), "RED-BLACK NUNCA MAS");

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
		Test.draw(window, font);
		window.display();

	}

	return 0;
}

