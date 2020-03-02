#include <iostream>
#include <string.h>
#include "bst.cpp"
using namespace std;

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
	bool put(Slice &key, Slice &value, int, int); //returns true if value overwritten
	bool del(Slice &key, int, int);
	bool get(int, Slice &key, Slice &value); //returns Nth key-value pair
	bool del(int, int);						 //delete Nth key-value pair
	bool resize();

	TrieNode *nodes;
	int free_head, free_tail;
	uint64_t size;
};

kvStore::kvStore(uint64_t max_entries)
{
	nodes = (TrieNode *)calloc(128, sizeof(TrieNode));
	size = 128;
	nodes[0].nxt = -1;
	nodes[0].bst = BST();
	for (int i = 1; i < 127; i++)
		nodes[i].nxt = i + 1;
	free_head = 1;
	free_tail = 127;
}

bool kvStore::resize()
{
	int old_size = size;
	size <<= 1;
	TrieNode *new_nodes = (TrieNode *)calloc(size, sizeof(TrieNode));
	memcpy(new_nodes, nodes, sizeof(TrieNode) * old_size);
	delete[] nodes;
	nodes = new_nodes;
	nodes[free_tail].nxt = old_size;
	for (int i = old_size; i < size - 1; i++)
		nodes[i].nxt = i + 1;
	free_tail = size - 1;
	return true;
}

bool kvStore::get(Slice &key, Slice &value)
{
	int cur = 0;
	for (int i = 0; i < key.size; i++)
	{
		int x;
		encode(x, key.data[i]);
		int node = nodes[cur].bst.find(x);
		if (node == -1)
			return false;
		cur = nodes[cur].bst.nodes[node].data;
	}
	if (!nodes[cur].end)
		return false;

	value = *nodes[cur].data;
	return true;
}

bool kvStore::put(Slice &key, Slice &value, int i = 0, int cur = 0)
{
	if (i == key.size)
	{
		nodes[cur].data = (Slice *)malloc(sizeof(value));
		memcpy(nodes[cur].data, &value, sizeof(value));
		// nodes[cur].data->data[nodes[cur].data->size] = '\0';
		// nodes[cur].data = &value;

		if (!nodes[cur].end)
		{
			nodes[cur].end = true;
			// nodes[cur].ends++;
			return false;
		}
		return true;
	}

	int old_cur = cur;
	int x;
	encode(x, key.data[i]);
	// cout<<"TRIE: "<<i<<" "<<cur<<" "<<x<<" "<<nodes[0].bst.nodes[0].right<<" "<<nodes[0].bst.nodes[0].left<<endl;

	int node = nodes[cur].bst.find(x);
		
	if (node != -1)
		cur = nodes[cur].bst.nodes[node].data;
	else
	{
		// nodes[cur].arr[x] = free_head;
		nodes[cur].bst.insert(x, free_head, 0);
		cur = free_head;
		free_head = nodes[cur].nxt;
		nodes[cur].nxt = -1;
		nodes[cur].bst = BST();
	}

	if (free_head == free_tail)
		resize();


	bool ret = put(key, value, i + 1, cur);
	// nodes[old_cur].ends += 1 - ret;
	nodes[old_cur].bst.change_ends(x, 1 - ret);
	return ret;
}

bool kvStore::del(Slice &key, int i = 0, int cur = 0)
{
	cout<<"TRIE: "<<cur<<endl;
	if (i == key.size)
	{
		if (nodes[cur].end)
		{
			nodes[cur].end = false;
			// nodes[cur].ends--;
			cout<<"IN TRIE: "<<cur<<" "<<nodes[cur].bst.sz<<endl;
			if (!nodes[cur].bst.sz)
			{
				// for (int i = 0; i < 52; i++)
				// 	nodes[cur].arr[i] = 0;
				nodes[free_tail].nxt = cur;
				nodes[cur].nxt = 0;
				free_tail = cur;
			}
			return true;
		}
		return false;
	}

	int x;
	int old_cur = cur;
	encode(x, key.data[i]);
	int nxt = -1;
	cout<<"BEFORE remove: "<<old_cur<<endl;
	if (nodes[cur].bst.remove(x, nxt))
		cout<<"TRIE NEXT: "<<nxt<<endl, cur = nxt;
	else
		return false;

	cout<<"AFTER TRIE: "<<old_cur<<endl;

	bool ret = del(key, i + 1, cur);
	nodes[old_cur].bst.change_ends(x, -ret);

	if (!nodes[old_cur].bst.sz && !nodes[old_cur].end)
	{
		// for (int i = 0; i < 52; i++)
		// 	nodes[old_cur].arr[i] = 0;

		nodes[free_tail].nxt = old_cur;
		nodes[old_cur].nxt = 0;
		free_tail = old_cur;

		if (old_cur == 0) // might be unnecessary
			nodes[old_cur].nxt = -1;
	}

	return ret;
}

bool kvStore::get(int N, Slice &key, Slice &value)
{
// 	Slice temp;
// 	char *array = (char *)malloc(65);
// 	temp.data = array;
// 	temp.size = 0;
// 	array[temp.size] = '\0';
// 	int cur = 0;
// 	while (1)
// 	{
// 		// cout<<"GET: "<<nodes[cur].ends<<endl;
// 		N -= nodes[cur].end;
// 		for (int i = 0; i < 52; i++)
// 		{
// 			if (!nodes[cur].arr[i])
// 			{
// 				if (i == 51)
// 					return false;
// 				continue;
// 			}
// 			if (nodes[nodes[cur].arr[i]].ends < N)
// 				N -= nodes[nodes[cur].arr[i]].ends;
// 			else
// 			{
// 				if (i < 26)
// 					array[temp.size] = (char)('A' + i);
// 				else
// 					array[temp.size] = (char)('a' + i - 26);
// 				temp.size++;
// 				array[temp.size] = '\0';
// 				cur = nodes[cur].arr[i];
// 				break;
// 			}
// 			if (i == 51)
// 				return false;
// 		}
// 		if (N == 1 && nodes[cur].end)
// 		{
// 			value.data = (char *)malloc(257);
// 			memcpy(value.data, (nodes[cur].data)->data, 257);
// 			value.size = (nodes[cur].data)->size;
// 			key = temp;
// 			return true;
// 		}
// 	}
// 	return false;
}
bool kvStore::del(int N, int cur = 0)
{
// 	// cout << "DEL: " << cur << " " << N << " " << nodes[cur].end << " " << nodes[cur].ends << endl;
// 	if (N == 1 && nodes[cur].end)
// 	{
// 		nodes[cur].end = false;
// 		// cout << "BASE: " << nodes[cur].ends << " " << endl;
// 		nodes[cur].ends--;
// 		if (!nodes[cur].ends)
// 		{
// 			for (int i = 0; i < 52; i++)
// 				nodes[cur].arr[i] = 0;
// 			nodes[free_tail].nxt = cur;
// 			nodes[cur].nxt = 0;
// 			free_tail = cur;
// 		}
// 		return true;
// 	}

// 	N -= nodes[cur].end;

// 	int old_cur = cur, i;
// 	for (i = 0; i < 52; i++)
// 	{
// 		if (!nodes[cur].arr[i])
// 		{
// 			if (i == 51)
// 				return false;
// 			continue;
// 		}
// 		// cout << i << " " << N << endl;
// 		if (nodes[nodes[cur].arr[i]].ends < N)
// 			N -= nodes[nodes[cur].arr[i]].ends;
// 		else
// 		{
// 			old_cur = cur;
// 			cur = nodes[cur].arr[i];
// 			break;
// 		}
// 		if (i == 51)
// 			return false;
// 	}
// 	// cout << old_cur << endl;
// 	char c;
// 	if (i < 26)
// 		c = (char(i + 'A'));
// 	else
// 		c = (char)(i + 'a' - 26);
// 	// cout << c << endl;
// 	bool ret = del(N, cur);

// 	nodes[old_cur].ends -= ret;
// 	if (ret && nodes[cur].ends == 0)
// 		nodes[old_cur].arr[i] = 0;
// 	if (!nodes[old_cur].ends && !nodes[old_cur].end) // second if might be unnecessary
// 	{
// 		for (int i = 0; i < 52; i++)
// 			nodes[old_cur].arr[i] = 0;

// 		nodes[free_tail].nxt = old_cur;
// 		nodes[old_cur].nxt = 0;
// 		free_tail = old_cur;

// 		if (old_cur == 0) // might be unnecessary
// 			nodes[old_cur].nxt = -1;
// 	}
// 	return ret;
}