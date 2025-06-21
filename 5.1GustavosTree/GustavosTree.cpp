#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <SFML/Graphics.hpp>
using namespace std;

const int WIDTH = 1920, HEIGHT = 1080, FONT_SIZE = 10;
const float RADIUS = 20;

template <class T>
struct Node {
	T value;
	Node<T>* next[2];

	Node(T v, Node<T>* l = nullptr, Node<T>* r = nullptr) {
		value = v;
		next[0] = l;
		next[1] = r;
	}
};

template <class T>
void connect(vector<Node<T>*>& s, vector<Node<T>*>& p, int start, int size, int interval) {
	for (int i = start; i < size/2; i += interval) {
		p[i] = new Node<T>(s[i * 2]->value + s[i * 2 + 1]->value, s[i * 2], s[i * 2 + 1]);
	}
}

template <class T>
struct GustavosTree {
	Node<T>* root;

	GustavosTree(const vector<T>& values) {
		root = nullptr;

		int n = values.size();
		int nt = 4;
		nt = thread::hardware_concurrency();

		vector<Node<T>*> s(n), p(n / 2);
		for (int i = 0; i < n; i++) s[i] = new Node<T>(values[i]);

		while (n > 1) {
			p.assign(n / 2, nullptr);
			vector<thread> rt(nt);

			int mn = nt;
			if (mn > n) mn = n;

			for (int i = 0; i < mn; i++) rt[i] = thread(connect<T>, ref(s), ref(p), i, n, nt);
			for (int i = 0; i < mn; i++) rt[i].join();

			n >>= 1;
			s = move(p);
		}

		root = s[0];
	}

	void draw(sf::RenderWindow& target, sf::Font& font) const {
		// Text
		sf::Text text(font); // a font is required to make a text object
		// set the string to display
		text.setString(to_string(root->value));
		// set the character size
		text.setCharacterSize(FONT_SIZE); // in pixels, not points!
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
	}
};

int main() {
	long long lvl = 7;
	vector<long long> input(1LL << lvl);

	for (int i = 0; i < 1 << lvl; i++) input[i] = i + 1;

	GustavosTree<long long> Test(input);
	cout << Test.root->value << "\n";

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

