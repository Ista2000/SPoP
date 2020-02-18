#include <iostream>
#include <string.h>
using namespace std;

struct Slice
{
	uint8_t size;
	char *data;
};

struct TrieNode
{
	int arr[27];
	bool end = false;
	int ends = 0;
	Slice *data;
};

/*
* Implementation version 1
* Just a vanilla trie with 26 entries
*/

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
	nodes[0].arr[26] = -1;
	for(int i = 0;i<127;i++)
		nodes[i].arr[26] = i+1;
	free_head = 1;
	free_tail = 127;
}

bool kvStore::resize()
{
	int old_size = size;
	size <<= 1;
	TrieNode *new_nodes = (TrieNode *)calloc(size, sizeof(TrieNode));
	memcpy(new_nodes, nodes, sizeof(TrieNode) * old_size);
	// memset(nodes[i].arr, 0, 27 * sizeof(int));
	delete[] nodes;
	nodes = new_nodes;


	nodes[free_tail].arr[26] = old_size;
	for (int i = old_size; i < size - 1; i++)
		nodes[i].arr[26] = i + 1;
	free_tail = size - 1;
	return true;
}

bool kvStore::get(Slice &key, Slice &value)
{
	int cur = 0;
	for (int i = 0; i < key.size; i++)
	{
		cur = nodes[cur].arr[key.data[i] - 'a'];
		if (!cur)
			return false;
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
		nodes[cur].data = &value;
		nodes[cur].end = true;
		nodes[cur].ends++;
		return true;
	}
	int old_cur = cur;
	if (nodes[cur].arr[key.data[i] - 'a'])
		cur = nodes[cur].arr[key.data[i] - 'a'];
	else
		cur = nodes[cur].arr[key.data[i] - 'a'] = free_head, free_head = nodes[free_head].arr[26];

	if (free_head == free_tail)
		resize();

	nodes[cur].arr[26] = -1;
	bool ret = put(key, value, i + 1, cur);
	nodes[old_cur].ends += ret;
	return ret;
}

bool kvStore::del(Slice &key, int i = 0, int cur = 0)
{
	if (i == key.size)
	{
		nodes[cur].end = false;
		return true;
	}

	if (nodes[cur].arr[key.data[i] - 'a'])
		cur = nodes[cur].arr[key.data[i] - 'a'];
	else
		return false;

	bool ret = del(key, i + 1, cur);
	nodes[cur].ends -= ret;

	if (!nodes[cur].ends)
	{
		nodes[free_tail].arr[26] = cur;
		nodes[cur].arr[26] = 0;
		free_tail = cur;
	}

	return ret;
}

bool kvStore::get(int N, Slice &key, Slice &value)
{
	Slice temp;
	char *array = (char*)malloc(64);
	temp.data = array;
	temp.size=0;
	array[temp.size]='\0';
	int cur = 0;
	while (1)
	{
		// cout<<"GET: "<<nodes[cur].ends<<endl;
		for (int i = 0; i < 26; i++)
		{
			if (!nodes[cur].arr[i])
			{
				if (i == 25)
					return false;
				continue;
			}
			if (nodes[nodes[cur].arr[i]].ends < N)
				N -= nodes[nodes[cur].arr[i]].ends;
			else
			{
				array[temp.size] = (char)(97+i);
				temp.size++;
				array[temp.size] = '\0';
				cur = nodes[cur].arr[i];
				break;
			}
		}
		if (N == 1 && nodes[cur].end)
		{
			value = *nodes[cur].data;
			key = temp;
			return true;
		}
		N -= nodes[cur].end;
	}
	return false;
}

bool kvStore::del(int N, int cur = 0)
{
	if (N == 1 && nodes[cur].end)
	{
		nodes[cur].end = false;
		return true;
	}

	N -= nodes[cur].end;

	for (int i = 0; i < 26; i++)
	{
		if (!nodes[cur].arr[i])
		{
			if (i == 25)
				return false;

			continue;
		}

		if (nodes[nodes[cur].arr[i]].ends < N)
			N -= nodes[nodes[cur].arr[i]].ends;
		else
		{
			cur = nodes[cur].arr[i];
			break;
		}
	}

	bool ret = del(N, cur);
	nodes[cur].ends -= ret;
	if (!nodes[cur].ends)
	{
		nodes[free_tail].arr[26] = cur;
		nodes[cur].arr[26] = 0;
		free_tail = cur;
	}
	return ret;
}
