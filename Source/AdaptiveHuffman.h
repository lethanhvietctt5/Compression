#pragma once

#include <iostream>
#include <fstream>

#define MAX_ORDER 512
#define ILLEGAL -1

using namespace std;

struct Node
{
	int symbol;
	int freq;
	int order;

	Node* parent;
	Node* left;
	Node* right;

	bool isNYT;
	bool isLeaf;

	Node()
	{
		symbol = ILLEGAL;
		freq = 0;
		order = 0;
		parent = left = right = nullptr;
		isNYT = isLeaf = false;
	}
	Node(int symb, int fr, int ord, Node* p, Node* l, Node* r, bool NYT, bool leaf)
	{
		symbol = symb;
		freq = fr;
		order = ord;
		parent = p;
		left = l;
		right = r;
		isNYT = NYT;
		isLeaf = leaf;
	}
};
class AdaptiveHuffman
{
	Node* root;
	Node* NYT;

public:
	AdaptiveHuffman(const AdaptiveHuffman& copy);
	Node* getNodeOfSymbol(int symb, Node* root);
	int findNodeSameFreq(Node*& crr, Node* root);
	int swapNode(Node* node1, Node* node2);
	int updateTree(Node* currentNode);
	void addSymbol(int symb);
	void encode(ifstream in, ofstream &out);
	void decode(ifstream in, ofstream& out);
};