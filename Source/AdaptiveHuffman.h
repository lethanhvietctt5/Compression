#pragma once

#include <iostream>
#include <string>
#include <fstream>

#define MAX_ORDER 512
#define ILLEGAL -1

using namespace std;

struct Node
{
	char symbol;
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
	Node(char symb, int fr, int ord, Node* p, Node* l, Node* r, bool NYT, bool leaf)
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
	AdaptiveHuffman();
	//AdaptiveHuffman(const AdaptiveHuffman& copy);

	// Build Adaptive Huffman Tree
	Node* getNodeOfSymbol(char symb, Node* root);
	Node* findNodeSameFreq(Node* crr, Node* root);
	int swapNode(Node* node1, Node* node2);
	int updateTree(Node* currentNode);
	void addSymbol(char symb);
	string getPathtoSymbol(Node* crr, Node* root, string path);
	
	//Encode 
	void encode(ifstream& in, ofstream& out);

	//Decode
	Node* getNodebyPath(Node* &from, char path);
	void decode(ifstream& in, ofstream& out);
};