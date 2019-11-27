#pragma once

#include <bitset>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <set>

#define INVALID -1000

using namespace std;

struct node
{
	char symbol;
	int freq;
	node* left;
	node* right;
	bool isLeaf;

	node()
	{
		symbol = INVALID;
		freq = 0;
		left = nullptr;
		right = nullptr;
		isLeaf = true;
	}

	node(char s, int f, node* l, node* r,bool il)
	{
		symbol = s;
		freq = f;
		left = l;
		right = r;
		isLeaf = il;
	}
};
class Huffman
{
	string inputfile;
	string outputfile;
	node* root;
	string content;
	set<char> allSymbol;
	map<char, int> freq_Symbols;
	map<char, string> pathOfallSymbols;
	string allPath;
public:
	Huffman();
	Huffman(string in, string out);
	// Encode
	void sortSymbol(vector<node*> &tree);
	void getSymbolsFromFile();
	void creatHuffmanTree();
	string getPathToLeaf(node* crr, char symbol, string path);
	void writePathToFile(ofstream& out, string path);
	void encode();

	// Decode
	void redefineTree(node* newTree);
	void decode();
	void deleteTree(node* root);
	~Huffman();
};