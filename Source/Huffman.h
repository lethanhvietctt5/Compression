#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#define INVALID 0

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
	int freq_Symbols[256] = { 0 };
	string pathOfallSymbols[256] = { "" };
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
	void clear();
	~Huffman();
};