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
	int ends = 0;
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
	bool del(Slice &key, int, int);			
	bool get(int, Slice &key, Slice &value); //returns Nth key-value pair
	bool del(int, int, int); //delete Nth key-value pair

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
	if(i == key.size) {
		nodes[cur].data = &value;
		nodes[cur].end = true;
		nodes[cur].ends++;
		return true;
	}
	if(nodes[cur].arr[key.data[i] - 'a'])
		cur = nodes[cur].arr[key.data[i] - 'a'];
	else
		cur = nodes[cur].arr[key.data[i] - 'a'] = free_head, free_head = nodes[free_head].arr[26];
	nodes[cur].arr[26] = -1;
	nodes[cur].ends++;
	return put(key, value, i+1, cur);
}

bool kvStore::del(Slice &key, int i = 0, int cur = 0) {
	if(i == key.size) {
		nodes[cur].end = false;
		nodes[cur].ends--;

		if(!nodes[cur].ends)
		{
			nodes[free_tail].arr[26] = cur;
			nodes[cur].arr[26] = 0;
			free_tail = cur;
		}

		return true;
	}

	if(nodes[cur].arr[key.data[i] - 'a'])
		cur = nodes[cur].arr[key.data[i] - 'a'];
	else
		return false;
	bool ret = del(key, i+1, cur);
	nodes[cur].ends -= ret;

	if(!nodes[cur].ends)
	{
		nodes[free_tail].arr[26] = cur;
		nodes[cur].arr[26] = 0;
		free_tail = cur;
	}


	return ret;
}

bool get(int N, Slice &key, Slice &value) {
	int cur = 0;
	while(1) {
		for(int i = 0;i<26;i++) {
			if(!nodes[cur].arr[i]) continue;
			if(nodes[nodes[cur].arr[i]].ends < N)
				N -= nodes[nodes[cur].arr[i]].ends;
			else {
				cur = nodes[cur].arr[i];
				break;
			}
			if(i == 25)
				return false;
		}
		if(nodes[cur].ends == 1 && N == 1 && nodes[cur].end) {
			// TODO: Assign key
			value = *nodes[cur].data;
			return true;
		}
	}
}

bool kvStore::del(int N, int i = 0, int cur = 0) {
	if(nodes[cur].ends == 1 && N == 1 && nodes[cur].end) {
		nodes[cur].end = false;
		nodes[cur].ends--;
		
		if(!nodes[cur].ends) {
			nodes[free_tail].arr[26] = cur;
			nodes[cur].arr[26] = 0;
			free_tail = cur;
		}

		return true;
	}

	for(int i = 0;i<26;i++) {
		if(!nodes[cur].arr[i]) continue;
		if(nodes[nodes[cur].arr[i]].ends < N)
			N -= nodes[nodes[cur].arr[i]].ends;
		else {
			cur = nodes[cur].arr[i];
			break;
		}
		if(i == 25)
			return false;
	}

	bool ret = del(N, i+1, cur);
	nodes[cur].ends -= ret;
	if(!nodes[cur].ends) {
		nodes[free_tail].arr[26] = cur;
		nodes[cur].arr[26] = 0;
		free_tail = cur;
	}
}
