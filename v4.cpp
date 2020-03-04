#include <iostream>
#include <string.h>
#include "bst.cpp"
using namespace std;

mutex head, tail;
struct Slice
{
	uint8_t size;
	char *data;
};

struct TrieNode
{
	BST bst;
	int nxt;
	bool end = false;
	Slice *key = NULL;
	Slice *data;
};

/*
* Implementation version 3
* Trie with BST in each node
*/

#define encode(x, c)      \
	if (c >= 'a')         \
		x = c - 'a' + 26; \
	else                  \
		x = c - 'A';

class kvStore
{
public:
	kvStore(uint64_t max_entries);
	bool get(Slice &key, Slice &value);			  //returns false if key didn’t exist
	bool put(Slice &key, Slice &value, int, int, int); //returns true if value overwritten
	bool del(Slice &key, int, int, int);
	bool get(int, Slice &key, Slice &value); //returns Nth key-value pair
	bool get2(int, Slice &key, Slice &value); //returns Nth key-value pair
	bool del(int);						 //delete Nth key-value pair
	bool del2(int, int, int);						 //delete Nth key-value pair

	bool resize();

	TrieNode *nodes;
	int free_head, free_tail;
	int size;
};

kvStore::kvStore(uint64_t max_entries)
{
	nodes = (TrieNode *)calloc(128, sizeof(TrieNode));
	size = 128;
	nodes[0].nxt = -1;
	nodes[0].bst = *((BST *)calloc(1, sizeof(BST)));
	nodes[0].bst = BST();
	for (int i = 1; i < 127; i++)
		nodes[i].nxt = i + 1;
	free_head = 1;
	free_tail = 127;
}

bool kvStore::resize()
{
	// cout << free_head << " " << free_tail << endl;
	int old_size = size;
	size <<= 1;
	TrieNode *new_nodes = (TrieNode *)calloc(size, sizeof(TrieNode));
	memcpy(new_nodes, nodes, sizeof(TrieNode) * old_size);
	delete[] nodes;
	// nodes = NULL;

	nodes = new_nodes;
	tail.lock();
	nodes[free_tail].nxt = old_size;
	tail.unlock();
	for (int i = old_size; i < size - 1; i++)
		nodes[i].nxt = i + 1;
	tail.lock();
	// assert(size != 0);
	free_tail = size - 1;
	tail.unlock();
	return true;
}

bool kvStore::get(Slice &key, Slice &value)
{
	int cur = 0;
	for (int i = 0; i < 10; i++)
	{
		if(i == key.size)
			break;
		uint8_t x;
		encode(x, key.data[i]);

		uint8_t node = nodes[cur].bst.find(x);
		if (node == (uint8_t)-1)
			return false;
		cur = nodes[cur].bst.nodes[node].data;
		if (cur == -1)
			return false;
	}
	if (!nodes[cur].end)
		return false;

	if(key.size <= 10)
	{
		value = *nodes[cur].data;
		return true;
	}

	if((int)key.size - 10 != (int)nodes[cur].key->size)
		return false;
	for(int i = 10;i<(int)key.size;i++)
		if(key.data[i] != nodes[cur].key->data[i-10])
			return false;

	value = *nodes[cur].data;
	return true;
}

bool kvStore::put(Slice &key, Slice &value, int i = 0, int cur = 0, int lvl = 0)
{
	if (cur < 0)
		return false;

	if (i == key.size)
	{
		nodes[cur].data = (Slice *)malloc(sizeof(value));
		memcpy(nodes[cur].data, &value, sizeof(value));

		if (!nodes[cur].end)
		{
			nodes[cur].end = true;
			return false;
		}
		return true;
	}

	if(lvl == 10)
	{
		nodes[cur].data = (Slice *)malloc(sizeof(value));
		memcpy(nodes[cur].data, &value, sizeof(value));

		nodes[cur].key = (Slice *) malloc(sizeof(Slice));
		nodes[cur].key->data = (char*) malloc((int)key.size-10);
		nodes[cur].key->size = (int)key.size-10;
		memcpy(nodes[cur].key->data, key.data+10, (int)key.size-10);

		nodes[cur].end = true;
		return false;
	}
	int old_cur = cur;
	uint8_t x;
	encode(x, key.data[i]);

	uint8_t node = nodes[cur].bst.find(x);

	if (node != (uint8_t)-1)
	{
		cur = nodes[cur].bst.nodes[node].data;
		if (cur == -1)
			return false;
	}
	else
	{
		head.lock();
		nodes[cur].bst.insert(x, free_head, 0);
		cur = free_head;
		head.unlock();
		if (cur < 0)
			return false;
		head.lock();
		free_head = nodes[cur].nxt;
		head.unlock();
		nodes[cur].nxt = -1;
		nodes[cur].bst = BST();
	}

	head.lock();
	tail.lock();
	if (free_head == free_tail)
	{
		head.unlock();
		tail.unlock();
		resize();
	}
	head.unlock();
	tail.unlock();
	bool ret = put(key, value, i + 1, cur, lvl+1);
	nodes[old_cur].bst.change_ends(x, 1 - ret);
	return ret;
}

bool kvStore::del(Slice &key, int i = 0, int cur = 0, int lvl = 0)
{
	if (cur < 0)
		return false;

	if (i == key.size)
	{
		if (nodes[cur].end)
		{
			nodes[cur].end = false;
			return true;
		}
		return false;
	}

	if(lvl == 10)
	{
		if (!nodes[cur].end || (int)key.size - 10 != (int)nodes[cur].key->size)
			return false;
		
		for(int i = 10;i<(int)key.size;i++)
			if(key.data[i] != nodes[cur].key->data[i-10])
				return false;
		
		nodes[cur].end = false;

		delete nodes[cur].key;
		return true;
	}

	uint8_t x;
	int old_cur = cur;
	encode(x, key.data[i]);
	uint8_t nxt = nodes[cur].bst.find(x);

	if (nxt != (uint8_t)-1)
	{
		if (nodes[cur].bst.nodes[nxt].data == -1)
			return false;
		cur = nodes[cur].bst.nodes[nxt].data;
	}
	else
		return false;

	bool ret = del(key, i + 1, cur, lvl+1);
	nodes[old_cur].bst.change_ends(x, -ret);

	if (!nodes[cur].bst.sz && !nodes[cur].end)
	{
		nodes[cur].bst = BST();

		tail.lock();
		nodes[free_tail].nxt = cur;
		nodes[cur].nxt = 0;
		free_tail = cur;
		tail.unlock();
		nodes[old_cur].bst.remove_bst(x);
	}

	return ret;
}

bool kvStore::get(int N, Slice &key, Slice &value){
	return get2(N+1, key, value);
}

bool kvStore::get2(int N, Slice &key, Slice &value)
{
	Slice temp;
	char *array = (char *)malloc(65);
	temp.data = array;
	temp.size = 0;
	int cur = 0;
	int lvl = 0;
	while (1)
	{
		if (N == 1 && nodes[cur].end)
		{
			// value.data = (char *)malloc(257);
			// memcpy(value.data, (nodes[cur].data)->data, 257);
			// value.size = (nodes[cur].data)->size;
			value = *nodes[cur].data;
			if(lvl == 10 && nodes[cur].key != NULL)
				for(int i = 0;i<nodes[cur].key->size;i++)
					array[temp.size++] = nodes[cur].key->data[i];
			key = temp;
			return true;
		}
		if(lvl == 10)
			return false;
		N -= nodes[cur].end;
		uint8_t nxt = nodes[cur].bst.inorder(N);
		if (nxt == (uint8_t)-1)
			return false;

		uint8_t i = nodes[cur].bst.nodes[nxt].c;
		if (i < (uint8_t)26)
			array[temp.size] = (char)('A' + i);
		else
			array[temp.size] = (char)('a' + i - 26);
		temp.size++;

		if (nodes[cur].bst.nodes[nxt].data == -1)
			return false;
		cur = nodes[cur].bst.nodes[nxt].data;
		lvl++;

	}
	return false;
}

bool kvStore::del(int N){
	return del2(N+1, 0, 0);
}

bool kvStore::del2(int N, int cur, int lvl)
{
	if (N == 1 && nodes[cur].end)
	{	
		nodes[cur].end = false;
		delete nodes[cur].key;

		return true;
	}
	if(lvl == 10)
		return false;

	N -= nodes[cur].end;

	int old_cur = cur;
	uint8_t nxt = nodes[cur].bst.inorder(N);
	if (nxt == (uint8_t)-1)
		return false;

	uint8_t x = nodes[cur].bst.nodes[nxt].c;
	if (nodes[cur].bst.nodes[nxt].data == -1)
		return false;
	cur = nodes[cur].bst.nodes[nxt].data;

	bool ret = del2(N, cur, lvl+1);
	nodes[old_cur].bst.change_ends(x, -ret);

	if (!nodes[cur].bst.sz && !nodes[cur].end)
	{
		nodes[cur].bst = BST();
		tail.lock();
		nodes[free_tail].nxt = cur;
		nodes[cur].nxt = 0;
		// if (cur == -1)
		// 	int t=5;
		free_tail = cur;
		tail.unlock();
		nodes[old_cur].bst.remove_bst(x);
	}
	return ret;
}