#include <iostream>
#include <string.h>
using namespace std;

#define decode(x, c) if(c >= 'a') x = c - 'a' + 26; else x = c - 'A';

struct Slice {
	uint8_t size;
	char*   data;
};

struct TrieNode {
	int arr[2];
	bool end = false;
	int ends = 0;
	int nxt;
	Slice* data;
};

/* 
* Implementation version 4
* Encode key to bits and save it in the trie
*/

class kvStore {
public:
	// kvStore(uint64_t max_entries);
	// bool get(Slice &key, Slice &value); //returns false if key didn’t exist
	// bool put(Slice &key, Slice &value, int, int); //returns true if value overwritten
	// bool del(Slice &key, int, int);			
	// bool get(int, Slice &key, Slice &value); //returns Nth key-value pair
	// bool del(int, int, int); //delete Nth key-value pair

	TrieNode *nodes;
	int free_head, free_tail;
};

kvStore::kvStore(uint64_t max_entries) {
	nodes = (TrieNode*) calloc(max_entries * 52 * 6, sizeof(TrieNode));
	nodes[0].nxt = -1;
	for(int i = 1;i < 52 * max_entries - 1; i++)
		nodes[i].nxt = i+1;
	free_head = 1;
	free_tail = max_entries * 52 * 6 - 1;
}

bool kvStore::get(Slice &key, Slice &value) {
	int cur = 0;
	for(int i = 0;i < key.size; i++) {
		int x;
		decode(x, key.data[i]);
		for(int j = 0; j<6;j++, x>>=1)
		{
			cur = nodes[cur].arr[x&1];
			if(!cur)
				return false;
		}
	}
	if(!nodes[cur].end)
		return false;
	value = *nodes[cur].data;
	return true;
}

bool kvStore::put(Slice &key, Slice &value, int i = 0, int cur = 0, int l = 0, int x = 0) {
	if(i == key.size) {
		nodes[cur].data = &value;
		nodes[cur].end = true;
		nodes[cur].ends++;
		return true;
	}
	if(l == 6)l = 0;
	if(!l) {
		decode(x, key.data[i]);
	}


	if(nodes[cur].arr[x&1])
		cur = nodes[cur].arr[x&1];
	else
		cur = nodes[cur].arr[x&1] = free_head, free_head = nodes[free_head].nxt;
	nodes[cur].nxt = -1;
	nodes[cur].ends++;
	return put(key, value, i+1, cur, l+1, x>>1);
}

bool kvStore::del(Slice &key, int i = 0, int cur = 0, int l = 0, int x = 0) {
	if(i == key.size) {
		nodes[cur].end = false;
		nodes[cur].ends--;

		if(!nodes[cur].ends) {
			nodes[free_tail].nxt = cur;
			nodes[cur].nxt = 0;
			free_tail = cur;
		}

		return true;
	}
	if(l == 6)l = 0;
	if(!l) {
		decode(x, key.data[i]);
	}


	if(nodes[cur].arr[x&1])
		cur = nodes[cur].arr[x&1];
	else
		return false;
	bool ret = del(key, i+1, cur, l+1, x>>1);
	nodes[cur].ends -= ret;

	if(!nodes[cur].ends) {
		nodes[free_tail].nxt = cur;
		nodes[cur].nxt = 0;
		free_tail = cur;
	}


	return ret;
}

bool kvStore::get(int N, Slice &key, Slice &value) {
	int cur = 0;
	while(1) {
		for(int i = 0;i<2;i++) {
			if(!nodes[cur].arr[i]) continue;
			if(nodes[nodes[cur].arr[i]].ends < N)
				N -= nodes[nodes[cur].arr[i]].ends;
			else {
				cur = nodes[cur].arr[i];
				break;
			}
			if(i)
				return false;
		}

		if(nodes[cur].ends == 1 && N == 1 && nodes[cur].end) {
			// TODO: Assign key
			value = *nodes[cur].data;
			return true;
		}
	}
}

bool kvStore::del(int N, int cur = 0) {
	if(nodes[cur].ends == 1 && N == 1 && nodes[cur].end) {
		nodes[cur].end = false;
		nodes[cur].ends--;
		
		if(!nodes[cur].ends) {
			nodes[free_tail].nxt = cur;
			nodes[cur].nxt = 0;
			free_tail = cur;
		}

		return true;
	}

	for(int i = 0;i<2;i++) {
		if(!nodes[cur].arr[i]) continue;
		if(nodes[nodes[cur].arr[i]].ends < N)
			N -= nodes[nodes[cur].arr[i]].ends;
		else {
			cur = nodes[cur].arr[i];
			break;
		}
		if(i)
			return false;
	}

	bool ret = del(N, cur);
	nodes[cur].ends -= ret;
	if(!nodes[cur].ends) {
		nodes[free_tail].nxt = cur;
		nodes[cur].nxt = 0;
		free_tail = cur;
	}
}

int main()
{
	return 0;
}