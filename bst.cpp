#include <iostream>
#include <string.h>
using namespace std;

struct BSTNode {
	int left, right;
	int c;
	int data;
	int ends;
};

class BST {
public:
	BSTNode nodes[52];
	int free_head, free_tail, sz;
	BST() 
	{
		free_head = 0;
		free_tail = 51;
		sz = 0;
		for(int i = 0;i<51;i++)
		{
			nodes[i].data = i+1;
			nodes[i].left = -1;
			nodes[i].right = -1;
		}
	}
	// int inorder(int cur,int &N) // Have to implement properly
	// {
	// 	if(nodes[cur].left == -1 && nodes[cur].right == -1){
	// 		return 0;
	// 	}
	// 	if(nodes[cur].left!=-1)
	// 	{
	// 		N = inorder(nodes[cur].left,N);
	// 	}
	// 	N -= nodes[cur].
	// 	//inorder stuff
	// 	if(nodes[cur].right!=-1)
	// 		N = inorder(nodes[cur].right, N);
	// }
	bool insert(int c, int data, int ends) 
	{
		int cur = 0;
		while(1) {
			if(!sz)
				break;
			if(nodes[cur].c == c)
				return false;
			int ncur;
			if(nodes[cur].c < c)
				ncur = nodes[cur].right;
			else
				ncur = nodes[cur].left;
			if(ncur == -1)
			{
				ncur = free_head;
				if(nodes[cur].c < c)
					nodes[cur].right = ncur;
				else
					nodes[cur].left = ncur;
				cur = ncur;
				break;
			}
			cur = ncur;
		}
		free_head = nodes[cur].data;
		nodes[cur].c = c;
		nodes[cur].data = data;
		nodes[cur].ends = ends;
		sz++;
		return true;
	}
	int find(int c) 
	{
		if(!sz) return -1;
		int cur = 0;
		while(cur != -1) 
		{
			if(nodes[cur].c == c)
				return cur;
			if(nodes[cur].c < c)
				cur = nodes[cur].right;
			else
				cur = nodes[cur].left;
		}
		return -1;
	}
	int par(int c)
	{
		if(!sz) return -1;
		int cur = 0, par = -1;
		while(cur != -1) 
		{
			if(nodes[cur].c == c)
				return par;
			par = cur;
			if(nodes[cur].c < c)
				cur = nodes[cur].right;
			else
				cur = nodes[cur].left;
		}
		return -1;
	}
	bool remove(int c)
	{
		if(!sz) return false;
		if(sz == 1)
		{
			sz = 0;
			nodes[0].left = -1;
			nodes[0].right = -1;
			nodes[0].data = free_head;
			free_head = 0;
		}
		int cur = find(c);
		if(cur == -1)
			return false;

		if(nodes[cur].left == -1 && nodes[cur].right == -1)
		{
			int p = par(c);
			nodes[cur].data = 0;
			nodes[free_tail].data = cur;
			free_tail = cur;
			if(nodes[p].c < c)
				nodes[p].right = -1;
			else
				nodes[p].left = -1;
			sz--;
			return true;
		}

		while(nodes[cur].left != -1 && nodes[cur].right != -1)
		{
			int ncur = nodes[cur].left;
			nodes[cur].data = nodes[ncur].data;
			nodes[cur].ends = nodes[ncur].data;
			nodes[cur].c = nodes[ncur].c;
			if(nodes[ncur].left == -1 && nodes[ncur].right == -1)
			{
				nodes[ncur].data = 0;
				nodes[free_tail].data = ncur;
				free_tail = ncur;
				nodes[cur].left = -1;
				sz--;
				return true;
			}
			cur = ncur;
		}
		if(nodes[cur].left == -1)
		{
			int ncur =  nodes[cur].right;
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
		}
		else
		{
			int ncur =  nodes[cur].left;
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
		}
		sz--;
		return true;
	}
};

int main()
{
	BST bst;
	bst.insert(0, 5, 0);
	bst.insert(5, 0, 0);
	bst.insert(2, 3, 0);
	bst.insert(4, 1, 0);
	bst.insert(1, 4, 0);
	bst.insert(3, 2, 0);
	for(int i = 0;i<6;i++)
		cout<<bst.find(i)<<" ";
	cout<<endl;
	for(int i = 0;i<6;i++)
	{
		bst.remove(i);
		for(int j = 0;j<6;j++)
		{
			cout<<bst.find(j)<<" ";
			if(bst.find(j)!=-1)
				cout<<bst.nodes[bst.find(j)].data;
			cout<<endl;
		}
		cout<<endl;
	}


	bst.insert(0, 5, 0);
	bst.insert(5, 0, 0);
	bst.insert(2, 3, 0);
	bst.insert(4, 1, 0);
	bst.insert(1, 4, 0);
	bst.insert(3, 2, 0);
	for(int i = 0;i<6;i++)
		cout<<bst.find(i)<<" ";
	cout<<endl;
	for(int i = 0;i<6;i++)
	{
		bst.remove(i);
		for(int j = 0;j<6;j++)
		{
			cout<<bst.find(j)<<" ";
			if(bst.find(j)!=-1)
				cout<<bst.nodes[bst.find(j)].data;
			cout<<endl;
		}
		cout<<endl;
	}
}
