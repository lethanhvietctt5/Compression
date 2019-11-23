#pragma once

#ifndef __STATIC_HUFFMAN__
#define __STATIC_HUFFMAN__

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <set>

#if __cplusplus < 201103L
#define nullptr NULL
#endif

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
	set<char> allSymbol;
	unordered_map<char, int> freq_Symbols;
	unordered_map<char, string> pathOfallSymbols;
public:
	Huffman();
	Huffman(string in, string out);
	// Encode
	void sortSymbol(vector<node*> &tree);
	bool checkGetAllSymbols();
	void getSymbolsFromFile();
	void creatHuffmanTree();
	string getPathToLeaf(node* crr, char symbol, string path);
	void encode();
	~Huffman() = default;
};
#endif