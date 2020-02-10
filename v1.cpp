#include <iostream>
#include <string.h>
using namespace std;

struct Slice {
	uint8_t size;
	char*   data;
};

struct TrieNode {
	int arr[27];
	bool end = false;
	Slice* data;
};

/* 
* Implementation version 1
* Just a vanilla trie with 26 entries
*/

class kvStore {
public:
	kvStore(uint64_t max_entries);
	bool get(Slice &key, Slice &value); //returns false if key didn’t exist
	bool put(Slice &key, Slice &value, int, int); //returns true if value overwritten
	bool del(Slice &key);			
	bool get(int N, Slice &key, Slice &value); //returns Nth key-value pair
	bool del(int N); //delete Nth key-value pair

	TrieNode *nodes;
	int free_head, free_tail;
};

kvStore::kvStore(uint64_t max_entries) {
	nodes = (TrieNode*) calloc(max_entries * 52, sizeof(TrieNode));
	nodes[0].arr[26] = -1;
	for(int i = 1;i < 52 * max_entries - 1; i++)
		nodes[i].arr[26] = i+1;
	free_head = 1;
	free_tail = max_entries * 52 - 1;
}

bool kvStore::get(Slice &key, Slice &value) {
	int cur = 0;
	for(int i = 0;i < key.size; i++) {
		cur = nodes[cur].arr[key.data[i] - 'a'];
		if(!cur)
			return false;
	}
	if(!nodes[cur].end)
		return false;
	value = *nodes[cur].data;
	return true;
}

bool kvStore::put(Slice &key, Slice &value, int i = 0, int cur = 0) {
	if(i == key.size)
	{
		nodes[cur].data = &value;
		nodes[cur].end = true;
		return true;
	}
	if(nodes[cur].arr[key.data[i] - 'a'])
		cur = nodes[cur].arr[key.data[i] - 'a'], cout<<"WTF"<<endl;
	else
		cur = nodes[cur].arr[key.data[i] - 'a'] = free_head, free_head = nodes[free_head].arr[26];
	nodes[cur].arr[26] = -1;
	return put(key, value, i+1, cur);
}

bool kvStore::del(Slice &key) {
	int cur = 0;
	for(int i = 0;i < key.size; i++) {
		if(nodes[cur].arr[key.data[i] - 'a'])
			cur = nodes[cur].arr[key.data[i] - 'a'];
		else
			cur = nodes[cur].arr[key.data[i] - 'a'] = free_head, free_head = nodes[free_head].arr[26];
		nodes[cur].arr[26] = -1;
	}
	nodes[cur].end = false;
	return true;
}