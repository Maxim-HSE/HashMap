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
	int num = 1;
	int empt = 0;
	Hash f;
	void resize(int new_size) {
		vector<list<pair<const KeyType, ValueType>>> t1 = lists;
		lists.clear();
		lists.resize(new_size);
		num = new_size;
		for (auto t : t1) {
			for (auto t2 : t) {
				int pos = f(t2.first) % num;
				lists[pos].push_back(t2);
			}
		}
	}
public:
	class iterator {
	private:
		HashMap<KeyType, ValueType, hash<KeyType>> *link;
		typename vector<list<pair<const KeyType, ValueType>>>::iterator f1;
		typename list<pair<const KeyType, ValueType>>::iterator f2;

	public:
		iterator() {

		}
		iterator(HashMap<KeyType, ValueType, hash<KeyType>> *t1,
			typename vector<list<pair<const KeyType, ValueType>>>::iterator t2,
			typename list<pair<const KeyType, ValueType>>::iterator t3) {
			this->link = t1;
			this->f1 = t2;
			this->f2 = t3;
		}
		iterator(const iterator &h) {
			this->link = h.link;
			this->f1 = h.f1;
			this->f2 = h.f2;
		}
		bool operator ==(iterator h) {
			return (this->f1 == h.f1) && (this->f2 == h.f2);
		}
		bool operator !=(iterator h) {
			return !(*this == h);
		}
		pair<const KeyType, ValueType> operator*() {
			return *(this->f2);
		}
		typename list<pair<const KeyType, ValueType>>::iterator operator ->() {
			return this->f2;
		}
		pair<const KeyType, ValueType> operator &() {
			return *this->f2;
		}
		iterator operator++() {
			++this->f2;
			if (this->f2 == this->f1->end()) {
				auto h1 = this->f1;
				while (h1 != link->lists.end()) {
					auto r = h1;
					++h1;
					if (h1 == link->lists.end()) {
						this->f1 = r;
						this->f2 = this->f1->end();
						return *this;
					}
					if (h1->size() != 0) {
						this->f1 = h1;
						this->f2 = this->f1->begin();
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
	class const_iterator {
	private:
		HashMap<KeyType, ValueType, hash<KeyType>> *link;
		typename vector<list<const pair<const KeyType, ValueType>>>::iterator f1;
		typename list<const pair<const KeyType, ValueType>>::iterator f2;
	public:
		const_iterator() {

		}
		const_iterator(HashMap<KeyType, ValueType, hash<KeyType>> *t1,
			typename vector<list<const pair<const KeyType, ValueType>>>::iterator t2,
			typename list<const pair<const KeyType, ValueType>>::iterator t3) {
			this->link = t1;
			this->f1 = t2;
			this->f2 = t3;
		}
		const_iterator(const const_iterator &h) {
			this->link = h.link;
			this->f1 = h.f1;
			this->f2 = h.f2;
		}
		bool operator ==(const_iterator h) const {
			return this->f1 = h.f1 && this->f2 = h.f2;
		}
		const_iterator operator =(const_iterator h) {
			this->link = h.link;
			this->f1 = h.f1;
			this->f2 = h.f2;
		}
		bool operator !=(const_iterator h) const {
			return !(*this == h);
		}
		typename list<const pair<const KeyType, ValueType>>::const_iterator operator ->() const {
			return this->f2;
		}
		const pair<const KeyType, ValueType> operator &() const {
			return *this->f2;
		}
		const pair<const KeyType, ValueType> operator*() {
			return this->f2;
		}
		const_iterator operator++() {
			++this->f2;
			if (this->f2 == this->f1->end()) {
				auto h1 = this->f1;
				while (h1 != link->lists.end()) {
					auto r = h1;
					++h1;
					if (h1 == link->lists.end()) {
						this->f1 = r;
						this->f2 = this->f1->end();
						return *this;
					}
					if (h1->size() != 0) {
						this->f1 = h1;
						this->f2 = this->f1->begin();
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
		if (empt == 0) {
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
		if (empt == 0) {
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
		int pos = f(key) % num;
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
		int pos = f(key) % num;
		const_iterator ans;
		ans.link = *this;
		for (auto t = lists[pos].begin(); t != lists[pos].end(); ++t) {
			if (t->first == key) {
				ans.f1 = *lists[pos];
				ans.f2 = t;
				return ans;
			}
		}
		ans.f1 = *lists[lists.size() - 1];
		ans.f2 = ans.f1->end();
		return ans;
	}
	HashMap(Hash h = Hash()) {
		empt = 0;
		f = h;
		lists.resize(1);
	}
	HashMap(iterator beg, iterator endd, Hash h = Hash()) {
		empt = 0;
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
		empt = 0;
		f = h;
		lists.resize(1);
		for (auto t = begin; t != end; ++t) {
			insert(*t);
		}
	}
	HashMap(initializer_list<pair<KeyType, ValueType>> sp) {
		empt = 0;
		f = Hash();
		lists.resize(1);
		for (auto t : sp) {
			insert(t);
		}
	}
	const int size() {
		return empt;
	}
	const bool empty() {
		if (empty == 0) {
			return 0;
		}
		return 1;
	}
	const bool empty() const {
		if (empty == 0) {
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
		int pos = f(sp.first) % num;
		for (auto t : lists[pos]) {
			if (t.first == sp.first) {
				return;
			}
		}
		lists[pos].push_back(sp);
		++empt;
		if (empt * 2 >= num) {
			resize(num * 2);
		}
	}
	void erase(const KeyType &sp) {
		int pos = f(sp) % num;
		auto it1 = lists[pos].begin();
		for (auto t : lists[pos]) {
			if (t.first == sp) {
				--empt;
				lists[pos].erase(it1);
				if (empt * 4 < num) {
					resize(num / 2);
				}
				return;
			}
			++it1;
		}
	}
	ValueType& operator [] (KeyType type) {
		ValueType h = ValueType();
		insert({ type, h });
		int pos = f(type) % num;
		auto it1 = lists[pos].begin();
		for (auto t : lists[pos]) {
			if (t.first == type) {
				return it1->second;
			}
			++it1;
		}
	}
	const ValueType& at(const KeyType &type) const {
		int pos = f(type) % num;
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
		empt = 0;
		num = 1;
		for (int i = 0; i < num; ++i) {
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
