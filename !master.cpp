#include<vector>
#include<string>
#include<list>
#include<initializer_list>
#include<iterator>
#include <iostream>
#include <cstdlib>
#include <functional>
#include <stdexcept>
#include <map>
using namespace std;
template<class KeyType, class ValueType, class Hash = std::hash<KeyType> >
class HashMap {
private:
	vector<list<pair<const KeyType, ValueType>>> lists;
	int memory = 1;
	int num_elements = 0;
	Hash f;
	void resize(int new_size) {
		vector<list<pair<const KeyType, ValueType>>> t1 = lists;
		lists.clear();
		lists.resize(new_size);
		memory = new_size;
		for (auto t : t1) {
			for (auto t2 : t) {
				int pos = f(t2.first) % memory;
				lists[pos].push_back(t2);
			}
		}
	}
	//using veclistpair = vector<list<pair<const KeyType, ValueType>>>;
public:

	using iterator = typename std::list<std::pair<const KeyType, ValueType>>::iterator;

	using const_iterator = typename std::list<std::pair<const KeyType, ValueType>>::const_iterator;

	iterator begin() {
		return lists[0].begin();
	}

	const_iterator begin() const {
		return lists[0].begin();
	}

	iterator end() {
		return lists[lists.size() - 1].end();
	}

	const_iterator end() const {
		return lists[lists.size() - 1].end();
	}
	template< class T >
	T findpos(T it, const KeyType key, int hash) {
		while (it != lists[hash].end()) {
			if (it->first == key) {
				return it;
			}
			++it;
		}
		return end();
	}
	iterator find(const KeyType key) {
		if (memory == 0) {
			return end();
		}
		int hash = f(key) % memory;
		iterator it = lists[hash].begin();
		return findpos(it, key, hash);
	}

	const_iterator find(const KeyType key) const {
		if (memory == 0) {
			return end();
		}
		int hash = hasher(key) % memory;
		const_iterator it = lists[hash].begin();
		return findpos(it, key, hash);
	}
	HashMap(Hash h = Hash()) {
		num_elements = 0;
		f = h;
		lists.resize(1);
	}
	HashMap(iterator beg, iterator endd, Hash h = Hash()) {
		num_elements = 0;
		f = h;
		lists.resize(1);
		for (iterator t = beg; t != endd; ++t) {
			insert(*t);
		}
	}
	HashMap operator =(HashMap h) {
		this->clear();
		for (int i = 0; i < h.lists.size(); ++i) {
			for (auto t : lists[i]) {
				this->insert(t);
			}
		}
	}
	HashMap(pair<KeyType, ValueType> *begin, pair<KeyType, ValueType> *end, Hash h = Hash()) {
		num_elements = 0;
		f = h;
		lists.resize(1);
		for (auto t = begin; t != end; ++t) {
			insert(*t);
		}
	}
	HashMap(initializer_list<pair<KeyType, ValueType>> sp) {
		num_elements = 0;
		f = Hash();
		lists.resize(1);
		for (auto t : sp) {
			insert(t);
		}
	}
	const int size() {
		return num_elements;
	}
	const bool empty() {
		if (num_elements == 0) {
			return 0;
		}
		return 1;
	}
	const bool empty() const {
		if (num_elements == 0) {
			return 0;
		}
		return 1;
	}
	const Hash hash_function() {
		return f;
	}
	const Hash hash_function() const {
		return f;
	}
	void insert(const pair<KeyType, ValueType> sp) {
		if (find(sp.first) != end()) {
			return;
		}
		int pos = f(sp.first) % memory;
		lists[pos].push_back(sp);
		++num_elements;
		if (num_elements * 2 >= memory) {
			resize(memory * 2);
		}
	}
	void erase(const KeyType &sp) {
		int pos = f(sp) % memory;
		auto it1 = lists[pos].begin();
		for (auto t : lists[pos]) {
			if (t.first == sp) {
				--num_elements;
				lists[pos].erase(it1);
				if (num_elements * 4 < memory) {
					resize(memory / 2);
				}
				return;
			}
			++it1;
		}
	}
	ValueType& operator [] (KeyType type) {
		ValueType h = ValueType();
		insert({ type, h });
		int pos = f(type) % memory;
		auto it1 = lists[pos].begin();
		for (auto t : lists[pos]) {
			if (t.first == type) {
				return it1->second;
			}
			++it1;
		}
	}
	const ValueType& at(const KeyType &type) const {
		int pos = f(type) % memory;
		auto it = lists[pos].begin();
		for (auto t : lists[pos]) {
			if (t.first == type) {
				const_iterator w = it;
				return w->second;
			}
			++it;
		}
		throw std::out_of_range("NOT_CE");
	}
	void clear() {
		num_elements = 0;
		memory = 1;
		for (int i = 0; i < memory; ++i) {
			lists[i].clear();
		}
		lists.resize(1);
	}
};

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);
	HashMap<int, int, hash<int>> sp;
	int n;
	cin >> n;
	for (int i = 0; i < n; ++i) {
		string s;
		cin >> s;
		if (s == "+") {
			int key;
			cin >> key;
			int w;
			cin >> w;
			sp[key] = w;
		}
		else if (s == "?") {
			int key;
			cin >> key;
			if (sp.find(key) != sp.end()) {
				cout << sp[key] << "\n";
			}
			else {
				cout << -1 << "\n";
			}
		}
		else if (s == "-") {
			int key;
			cin >> key;
			sp.erase(key);
		}
		else if (s == "!") {
			sp.clear();
		}
		else {
			for (auto t : sp) {
				cout << t.first << " " << t.second << "\n";
			}
		}
	}
	cout << sp.size();
	return 0;
}
