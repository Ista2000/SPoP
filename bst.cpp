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
	BST() {
		free_head = 0;
		free_tail = 51;
		sz = 0;
		for(int i = 0;i<51;i++)
			nodes[i].data = i+1,nodes[i].left = -1, nodes[i].right = -1;
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
	bool insert(int c, int data, int ends) {
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
	int find(int c) {
		int cur = 0;
		while(cur != -1) {
			if(nodes[cur].c == c)
				return nodes[cur].data;
			if(nodes[cur].c < c)
				cur = nodes[cur].right;
			else
				cur = nodes[cur].left;
		}
		return 0;
	}
	bool remove(int c) // Have to implement properly
	{
		int cur = 0;
		while(nodes[cur].left != nodes[cur].right) {
			if(nodes[cur].c == c) {
				nodes[cur].left = nodes[cur].right = -1;
				nodes[free_tail].data = cur;
				free_tail = cur;
				sz--;
				return true;
			}
			if(nodes[cur].c < c)
				cur = nodes[cur].right;
			else
				cur = nodes[cur].left;
		}
		return false;
	}
};
