#pragma once

#include <bitset>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <set>

#define INVALID -1

using namespace std;

struct node
{
	char symbol;
	int freq;
	node* left;
	node* right;

	node()
	{
		symbol = INVALID;
		freq = 0;
		left = nullptr;
		right = nullptr;
	}

	node(char s, int f, node* l, node* r)
	{
		symbol = s;
		freq = f;
		left = l;
		right = r;
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
	bool checkGetAllSymbols();
	void getSymbolsFromFile();
	void creatHuffmanTree();
	string getPathToLeaf(node* crr, char symbol, string path);
	void writePathToFile(ofstream& out, string path);
	void encode();

	// Decode
	bool checkLeaf(node* crr);
	void redefineTree(node* newTree);
	void decode();

	void deleteTree(node* root);
	~Huffman();
};