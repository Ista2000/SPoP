#include <iostream>
#include <string.h>
using namespace std;

struct BSTNode {
	uint8_t left, right;
	char c;
	int data;
};

class BST {
public:
	BSTNode nodes[52];
	int free_head, free_tail;
	BST() {
		free_head = 0, free_tail = 51;
		for(int i = 0;i<51;i++)
			nodes[i].data = i+1,nodes[i].left = -1, nodes[i].right = -1;
	}
	bool insert(char c, int data) {
		int cur = 0;
		while(nodes[cur].left != nodes[cur].right) {
			if(nodes[cur].c == c)
				return false;
			if(nodes[cur].c < c)
				cur = nodes[cur].right;
			else
				cur = nodes[cur].left;
		}
		if(nodes[cur].c < c)
			cur = nodes[cur].left = free_head;
		else
			cur = nodes[cur].right = free_head;
		free_head = nodes[cur].c;
		nodes[cur].c = c;
		nodes[cur].data = data;
		return true;
	}
	int find(char c) {
		int cur = 0;
		while(nodes[cur].left != nodes[cur].right) {
			if(nodes[cur].c == c)
				return nodes[cur].data;
			if(nodes[cur].c < c)
				cur = nodes[cur].right;
			else
				cur = nodes[cur].left;
		}
		return 0;
	}
	bool remove(char c)
	{
		int cur = 0;
		while(nodes[cur].left != nodes[cur].right) {
			if(nodes[cur].c == c) {
				nodes[cur].left = nodes[cur].right = -1;
				nodes[free_tail].data = cur;
				free_tail = cur;
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