#include <iostream>
#include <string.h>
#include "bst.cpp"
using namespace std;

int nxt=26;
struct Slice {
	uint8_t size;
	char*   data;
};

struct TrieNode {
	BST bst;
	bool end = false;
	int ends = 0;
	int switch=0;
	Slice* data;
	int arr[27];
};

/*
* Implementation version 3
* Trie with ART nodes
* Use a normal unsorted array until size 6
* Use a BST till size 40
* Use a static array of size 52 like version 1 till size 52
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
	nodes[0].arr[nxt] = -1;
	for(int i = 1;i < 52 * max_entries - 1; i++)
		nodes[i].arr[nxt] = i+1;
	free_head = 1;
	free_tail = max_entries * 52 - 1;
}

bool kvStore::get(Slice &key, Slice &value) {
	// int cur = 0;
	// for(int i = 0;i < key.size; i++) {
	// 	cur = nodes[cur].arr[key.data[i] - 'a'];
	// 	if(!cur)
	// 		return false;
	// }
	// if(!nodes[cur].end)
	// 	return false;
	// value = *nodes[cur].data;
	// return true;

	int cur = 0;
     for(int i = 0;i < key.size; i++) {
		cur = nodes[cur].bst.find(key.data[i]);
		if(!cur)
			return false;
	}
     if(!nodes[cur].end)
		return false;
	value = *nodes[cur].data;
	return true;
}

bool kvStore::put(Slice &key, Slice &value, int i = 0, int cur = 0) {
	// if(i == key.size) {
	// 	nodes[cur].data = &value;
	// 	nodes[cur].end = true;
	// 	nodes[cur].ends++;
	// 	return true;
	// }
	// if(nodes[cur].arr[key.data[i] - 'a'])
	// 	cur = nodes[cur].arr[key.data[i] - 'a'];
	// else
	// 	cur = nodes[cur].arr[key.data[i] - 'a'] = free_head, free_head = nodes[free_head].arr[nxt];
	// nodes[cur].arr[nxt] = -1;
	// nodes[cur].ends++;
	// return put(key, value, i+1, cur);
	//pretty sure its correct
	if(i==key.size){
		nodes[cur].data=&value;
		nodes[cur].end = true;
		nodes[cur].ends++;
		return true;
	}
	if(nodes[cur].bst.find(key.data[i])){
		cur=nodes[cur].bst.find(key.data[i]);
	}
	else{
          nodes[cur].ends++;
		nodes[cur].bst.insert(key.data[i],free_head,&nodes[cur].ends);
		cur=free_head;
		free_head=nodes[free_head].arr[nxt];
	}
	nodes[cur].arr[nxt]=-1;
	return put(key,value,i+1,cur);

}

bool kvStore::get(int N, Slice &key, Slice &value) {
	int cur = 0;
	while(1) {
		for(int i = 0;i<nxt;i++) {
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

bool kvStore::get(int N, Slice &key, Slice &value, int cur) {
	int cur = 0;
	while(1) {
		for(int i = 0;i<nxt;i++) {
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
     if(nodes[cur].bst.left==-1 && nodes[cur].bst.right==-1){
          return 0;
     }
     if(nodes[cur].left!=-1)
     {
          N=get(nodes[cur].left,N);
     }
     N-=nodes[cur].
     //inorder stuff
     if(nodes[cur].right!=-1)
          N=get(nodes[cur].right,N)
}

bool kvStore::del(Slice &key, int i = 0, int cur = 0) {
	if(i == key.size) {
		nodes[cur].end = false;
		nodes[cur].ends--;

		if(!nodes[cur].ends)
		{
			nodes[free_tail].arr[nxt] = cur;
			nodes[cur].arr[nxt] = 0;
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
		nodes[free_tail].arr[nxt] = cur;
		nodes[cur].arr[nxt] = 0;
		free_tail = cur;
	}


	return ret;
}

bool kvStore::del(int N, int i = 0, int cur = 0) {
	if(N == 1 && nodes[cur].end) {
		nodes[cur].end = false;
		nodes[cur].ends--;

		if(!nodes[cur].ends) {
			nodes[free_tail].arr[nxt] = cur;
			nodes[cur].arr[nxt] = 0;
			free_tail = cur;
		}

		return true;
	}

	for(int i = 0;i<nxt;i++) {
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
		nodes[free_tail].arr[nxt] = cur;
		nodes[cur].arr[nxt] = 0;
		free_tail = cur;
	}
}
