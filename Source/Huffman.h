#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
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
	unordered_map<char, int> freq_Symbols;
	unordered_map<char, string> pathOfallSymbols;
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
	void decode();
	~Huffman() = default;
};