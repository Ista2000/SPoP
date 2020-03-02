#include <iostream>
#include <string.h>
using namespace std;

// pthread_mutex
struct BSTNode
{
	int left, right;
	int c;
	int data;
	int ends;
};

class BST
{
public:
	BSTNode *nodes;
	int free_head, free_tail, sz;
	BST()
	{
		nodes = (BSTNode *)malloc(53 * sizeof(BSTNode));
		free_head = 0;
		free_tail = 52;
		sz = 0;
		for (int i = 0; i < 52; i++)
		{
			nodes[i].data = i + 1;
			nodes[i].left = -1;
			nodes[i].right = -1;
		}
	}
	int inorder(int &N, int cur = 0)
	{
		if (!sz || !N)
			return -1;
		int ret = -1;
		if (nodes[cur].left != -1)
			ret = inorder(N, nodes[cur].left);

		if (ret != -1)
			return ret;

		if (N <= nodes[cur].ends)
			return cur;

		N -= nodes[cur].ends;

		if (nodes[cur].right != -1)
			return inorder(N, nodes[cur].right);

		return -1;
	}
	void debug(vector<int> &v, int cur = 0)
	{
		if (cur == -1)
			return;
		debug(v, nodes[cur].left);
		v.push_back(nodes[cur].c);
		cout << "DEBUG: ";
		if (nodes[cur].left == -1)
			cout << "E ";
		else
			cout << nodes[nodes[cur].left].c << " ";
		cout << v.back() << " ";
		if (nodes[cur].right == -1)
			cout << "E ";
		else
			cout << nodes[nodes[cur].right].c << " ";
		cout << endl;

		debug(v, nodes[cur].right);
	}
	bool insert(int c, int data, int ends)
	{
		// cout<<"BST INSERT BEFORE: "<<free_head<<" "<<free_tail<<" "<<sz<<endl;
		// vector<int> v;
		// debug(v);
		// for(int i: v)
		// 	cout<<i<<" ";
		// cout<<endl;
		// for(int i = 1;i<v.size();i++)
		// 	if(v[i-1] > v[i])
		// 		exit(0);
		int cur = 0;
		while (1)
		{
			if (!sz)
				break;
			if (nodes[cur].c == c)
			{
				nodes[cur].data = data;
				nodes[cur].ends = ends;
				return false;
			}
			int ncur;
			if (nodes[cur].c < c)
				ncur = nodes[cur].right;
			else
				ncur = nodes[cur].left;
			// cout<<"BST INSERT: "<<cur<<" "<<ncur<<endl;
			if (ncur == -1)
			{
				// cout<<"INSIDE -1: "<<ncur<<" "<<free_head<<endl;
				ncur = free_head;
				if (nodes[cur].c < c)
					nodes[cur].right = ncur;
				else
					nodes[cur].left = ncur;
				cur = ncur;
				break;
			}
			cur = ncur;
		}
		free_head = nodes[cur].data;
		// cout<<"BST INSERT AFTER: "<<free_head<<" "<<free_tail<<" "<<sz<<endl;
		nodes[cur].c = c;
		nodes[cur].data = data;
		nodes[cur].ends = ends;
		nodes[cur].left = nodes[cur].right = -1;
		sz++;
		// v.clear();
		// debug(v);
		// for(int i: v)
		// 	cout<<i<<" ";
		// cout<<endl;
		// for(int i = 1;i<v.size();i++)
		// 	if(v[i-1] > v[i])
		// 		exit(0);
		return true;
	}
	bool change_ends(int c, int change)
	{
		int cur = find(c);
		if (cur == -1)
			return false;
		nodes[cur].ends += change;
		return true;
	}
	int find(int c)
	{
		if (!sz)
			return -1;
		int cur = 0;
		while (cur != -1)
		{
			if (nodes[cur].c == c)
				return cur;
			if (nodes[cur].c < c)
				cur = nodes[cur].right;
			else
				cur = nodes[cur].left;
		}
		return -1;
	}
	int par(int c)
	{
		if (!sz)
			return -1;
		int cur = 0, par = -1;
		while (cur != -1)
		{
			if (nodes[cur].c == c)
				return par;
			par = cur;
			if (nodes[cur].c < c)
				cur = nodes[cur].right;
			else
				cur = nodes[cur].left;
		}
		return -1;
	}
	bool remove_bst(int c)
	{
		// cout<<"DELETE"<<endl;
		if (!sz)
			return false;
		if (sz == 1)
		{
			sz = 0;
			nodes[0].left = -1;
			nodes[0].right = -1;
			nodes[0].data = free_head;
			free_head = 0;
			return true;
		}
		int cur = find(c);
		// cout<<"BST: "<<c<<" "<<cur<<endl;
		if (cur == -1)
			return false;

		if (nodes[cur].left == -1 && nodes[cur].right == -1)
		{
			int p = par(c);
			nodes[cur].data = 0;
			nodes[free_tail].data = cur;
			free_tail = cur;
			if (nodes[p].c < c)
				nodes[p].right = -1;
			else
				nodes[p].left = -1;
			sz--;
			return true;
		}

		if (nodes[cur].left == -1)
		{
			int ncur = nodes[cur].right;
			nodes[cur].left = nodes[ncur].left;
			nodes[cur].right = nodes[ncur].right;
			nodes[cur].c = nodes[ncur].c;
			nodes[cur].data = nodes[ncur].data;
			nodes[cur].ends = nodes[ncur].ends;
			nodes[ncur].left = -1;
			nodes[ncur].right = -1;
			nodes[ncur].data = 0;
			nodes[free_tail].data = ncur;
			free_tail = ncur;
			sz--;
		}
		else
		{
			int ncur = nodes[cur].left;
			while (nodes[ncur].right != -1)
				ncur = nodes[ncur].right;

			int character = nodes[ncur].c;
			int data = nodes[ncur].data;
			int ends = nodes[ncur].ends;

			remove_bst(nodes[ncur].c);

			nodes[cur].c = character;
			nodes[cur].data = data;
			nodes[cur].ends = ends;
		}

		// while(nodes[cur].left != -1 && nodes[cur].right != -1)
		// {
		// 	int ncur = nodes[cur].left;
		// 	nodes[cur].data = nodes[ncur].data;
		// 	nodes[cur].ends = nodes[ncur].ends;
		// 	nodes[cur].c = nodes[ncur].c;
		// 	if(nodes[ncur].left == -1 && nodes[ncur].right == -1)
		// 	{
		// 		nodes[ncur].data = 0;
		// 		nodes[free_tail].data = ncur;
		// 		free_tail = ncur;
		// 		nodes[cur].left = -1;
		// 		sz--;
		// 		return true;
		// 	}
		// 	cur = ncur;
		// }
		// if(nodes[cur].left == -1)
		// {
		// 	int ncur =  nodes[cur].right;
		// 	nodes[cur].left = nodes[ncur].left;
		// 	nodes[cur].right = nodes[ncur].right;
		// 	nodes[cur].c = nodes[ncur].c;
		// 	nodes[cur].data = nodes[ncur].data;
		// 	nodes[cur].ends = nodes[ncur].ends;
		// 	nodes[ncur].left = -1;
		// 	nodes[ncur].right = -1;
		// 	nodes[ncur].data = 0;
		// 	nodes[free_tail].data = ncur;
		// 	free_tail = ncur;
		// }
		// else
		// {
		// 	int ncur =  nodes[cur].left;
		// 	nodes[cur].left = nodes[ncur].left;
		// 	nodes[cur].right = nodes[ncur].right;
		// 	nodes[cur].c = nodes[ncur].c;
		// 	nodes[cur].data = nodes[ncur].data;
		// 	nodes[cur].ends = nodes[ncur].ends;
		// 	nodes[ncur].left = -1;
		// 	nodes[ncur].right = -1;
		// 	nodes[ncur].data = 0;
		// 	nodes[free_tail].data = ncur;
		// 	free_tail = ncur;
		// }
		return true;
	}
};

// int main()
// {
// 	BST bst;

// Test insert

// bst.insert(0, 5, 1);
// bst.insert(5, 0, 1);
// bst.insert(2, 3, 1);
// bst.insert(4, 1, 1);
// bst.insert(1, 4, 1);
// bst.insert(3, 2, 1);
// bst.insert(0, 5, 1);
// bst.insert(1, 4, 1);
// bst.insert(2, 3, 1);
// bst.insert(3, 2, 1);
// bst.insert(4, 1, 1);
// bst.insert(5, 0, 1);

// Test find

// for(int i = 0;i<6;i++)
// 	cout<<bst.find(i)<<" ";
// cout<<endl;

// Test remove

// for(int i = 0;i<6;i++)
// {
// 	bst.remove(i);
// 	for(int j = 0;j<6;j++)
// 	{
// 		cout<<bst.find(j)<<" ";
// 		if(bst.find(j)!=-1)
// 			cout<<bst.nodes[bst.find(j)].data;
// 		cout<<endl;
// 	}
// 	cout<<endl;
// }

// bst.insert(0, 5, 1);
// bst.insert(5, 0, 1);
// bst.insert(2, 3, 1);
// bst.insert(4, 1, 1);
// bst.insert(1, 4, 1);
// bst.insert(3, 2, 1);
// for(int i = 0;i<6;i++)
// 	cout<<bst.find(i)<<" ";
// cout<<endl;
// for(int i = 0;i<6;i++)
// {
// 	bst.remove(i);
// 	for(int j = 0;j<6;j++)
// 	{
// 		cout<<bst.find(j)<<" ";
// 		if(bst.find(j)!=-1)
// 			cout<<bst.nodes[bst.find(j)].data;
// 		cout<<endl;
// 	}
// 	cout<<endl;
// }

// Test inorder

// 	int N = 0;
// 	int node = bst.inorder(0, N);
// 	if(node != -1)
// 		cout<<bst.nodes[node].c<<endl;

// 	return 0;
// }
