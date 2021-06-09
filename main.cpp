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
	int number = 1;
	int elements = 0;
	Hash f;
	void resize(int new_size) {
		vector<list<pair<const KeyType, ValueType>>> t1 = lists;
		lists.clear();
		lists.resize(new_size);
		number = new_size;
		for (auto t : t1) {
			for (auto t2 : t) {
				int pos = f(t2.first) % number;
				lists[pos].push_back(t2);
			}
		}
	}
using veclistpair = vector<list<pair<const KeyType, ValueType>>>;
public:
	class iterator {
	private:
		HashMap<KeyType, ValueType, hash<KeyType>> *link;
		typename veclistpair::iterator first;
		typename list<pair<const KeyType, ValueType>>::iterator second;
 
	public:
		iterator() {
 
		}
		iterator(HashMap<KeyType, ValueType, hash<KeyType>> *t1,
			typename veclistpair::iterator t2,
			typename list<pair<const KeyType, ValueType>>::iterator t3) {
			this->link = t1;
			this->first = t2;
			this->second = t3;
		}
		iterator(const iterator &h) {
			this->link = h.link;
			this->first = h.first;
			this->second = h.second;
		}
		bool operator ==(iterator h) {
			return (this->first == h.first) && (this->second == h.second);
		}
		bool operator !=(iterator h) {
			return !(*this == h);
		}
		pair<const KeyType, ValueType> operator*() {
			return *(this->second);
		}
		typename list<pair<const KeyType, ValueType>>::iterator operator->() {
			return this->second;
		}
		pair<const KeyType, ValueType> operator &() {
			return *this->second;
		}
		iterator operator++() {
			++this->second;
			if (this->second == this->first->end()) {
				auto h1 = this->first;
				while (h1 != link->lists.end()) {
					auto r = h1;
					++h1;
					if (h1 == link->lists.end()) {
						this->first = r;
						this->second = this->first->end();
						return *this;
					}
					if (h1->size() != 0) {
						this->first = h1;
						this->second = this->first->begin();
						return *this;
					}
				}
			}
			else {
				return *this;
			}
		}
		iterator operator++(int) {
			iterator pred = *this;
			++this;
			return pred;
		}
	};
	using constpair = vector<list<const pair<const KeyType, ValueType>>>;
	class const_iterator {
	private:
		HashMap<KeyType, ValueType, hash<KeyType>> *link;
		typename constpair::iterator first;
		typename list<const pair<const KeyType, ValueType>>::iterator second;
	public:
		const_iterator() {
 
		}
		const_iterator(HashMap<KeyType, ValueType, hash<KeyType>> *t1,
			typename vector<list<const pair<const KeyType, ValueType>>>::iterator t2,
			typename list<const pair<const KeyType, ValueType>>::iterator t3) {
			this->link = t1;
			this->first = t2;
			this->second = t3;
		}
		const_iterator(const const_iterator &h) {
			this->link = h.link;
			this->first = h.first;
			this->second = h.second;
		}
		bool operator ==(const_iterator h) const {
			return this->first == h.first && this->second == h.second;
		}
		const_iterator operator =(const_iterator h) {
			this->link = h.link;
			this->first = h.first;
			this->second = h.second;
		}
		bool operator !=(const_iterator h) const {
			return !(*this == h);
		}
		typename list<const pair<const KeyType, ValueType>>::const_iterator operator ->() const {
			return this->second;
		}
		const pair<const KeyType, ValueType> operator &() const {
			return *this->second;
		}
		const pair<const KeyType, ValueType> operator*() {
			return this->second;
		}
		const_iterator operator++() {
			++this->second;
			if (this->second == this->first->end()) {
				auto h1 = this->first;
				while (h1 != link->lists.end()) {
					auto r = h1;
					++h1;
					if (h1 == link->lists.end()) {
						this->first = r;
						this->second = this->first->end();
						return *this;
					}
					if (h1->size() != 0) {
						this->first = h1;
						this->second = this->first->begin();
						return *this;
					}
				}
			}
			else {
				return *this;
			}
		}
		const_iterator operator++(int) {
			const_iterator pred = *this;
			++this;
			return pred;
		}
	};
	iterator begin() {
		if (elements == 0) {
			return end();
		}
		for (int i = 0; i < lists.size(); ++i) {
			if (lists[i].size() > 0) {
				iterator ans(this, lists.begin() + i, lists[i].begin());
				return ans;
			}
		}
	}
	const_iterator begin() const {
		if (elements == 0) {
			return end();
		}
		for (int i = 0; i < lists.size(); ++i) {
			if (lists[i].size() > 0) {
				const_iterator ans(this, lists.begin() + i, lists[i].begin());
				return ans;
			}
		}
	}
	iterator end() {
		iterator h(this, lists.begin() + (lists.size() - 1), lists[lists.size() - 1].end());
		return h;
	}
	const_iterator end() const {
		const_iterator h(this, lists.begin() + (lists.size() - 1), lists[lists.size() - 1].end());
		return h;
	}
	iterator find(KeyType key) {
		int pos = f(key) % number;
		for (auto t = lists[pos].begin(); t != lists[pos].end(); ++t) {
			if (t->first == key) {
				iterator ans(this, lists.begin() + pos, t);
				return ans;
			}
		}
		iterator ans(this, lists.begin() + (lists.size() - 1), lists[lists.size() - 1].end());
		return ans;
	}
	const_iterator find(KeyType key) const {
		int pos = f(key) % number;
		const_iterator ans;
		ans.link = *this;
		for (auto t = lists[pos].begin(); t != lists[pos].end(); ++t) {
			if (t->first == key) {
				ans.first = *lists[pos];
				ans.second = t;
				return ans;
			}
		}
		ans.first = *lists[lists.size() - 1];
		ans.second = ans.first->end();
		return ans;
	}
	HashMap(Hash h = Hash()) {
		elements = 0;
		f = h;
		lists.resize(1);
	}
	HashMap(iterator beg, iterator endd, Hash h = Hash()) {
		elements = 0;
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
		elements = 0;
		f = h;
		lists.resize(1);
		for (auto t = begin; t != end; ++t) {
			insert(*t);
		}
	}
	HashMap(initializer_list<pair<KeyType, ValueType>> sp) {
		elements = 0;
		f = Hash();
		lists.resize(1);
		for (auto t : sp) {
			insert(t);
		}
	}
	const int size() {
		return elements;
	}
	const bool empty() {
		if (elements == 0) {
			return 0;
		}
		return 1;
	}
	const bool empty() const {
		if (elements == 0) {
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
		int pos = f(sp.first) % number;
		for (auto t : lists[pos]) {
			if (t.first == sp.first) {
				return;
			}
		}
		lists[pos].push_back(sp);
		++elements;
		if (elements * 2 >= number) {
			resize(number * 2);
		}
	}
	void erase(const KeyType &sp) {
		int pos = f(sp) % number;
		auto it1 = lists[pos].begin();
		for (auto t : lists[pos]) {
			if (t.first == sp) {
				--elements;
				lists[pos].erase(it1);
				if (elements * 4 < number) {
					resize(number / 2);
				}
				return;
			}
			++it1;
		}
	}
	ValueType& operator [] (KeyType type) {
		ValueType h = ValueType();
		insert({ type, h });
		int pos = f(type) % number;
		auto it1 = lists[pos].begin();
		for (auto t : lists[pos]) {
			if (t.first == type) {
				return it1->second;
			}
			++it1;
		}
	}
	const ValueType& at(const KeyType &type) const {
		int pos = f(type) % number;
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
		elements = 0;
		number = 1;
		for (int i = 0; i < number; ++i) {
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
