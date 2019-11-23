#include "Huffman.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <utility>

using namespace std;

Huffman::Huffman()
{
	root = new node;
	inputfile = "";
	outputfile = "";
	freq_Symbols.clear();
	pathOfallSymbols.clear();
}

Huffman::Huffman(string in, string out)
{
	root = new node;
	inputfile = in;
	outputfile = out;
	freq_Symbols.clear();
	pathOfallSymbols.clear();
}

void Huffman::sortSymbol(vector<node*> &tree)
{
	for (int index = 0; index < tree.size(); index++)
	{
		for (int index_2 = index + 1; index_2 < tree.size(); index_2++)
		{
			if ((tree[index]->freq > tree[index_2]->freq) || (tree[index]->freq == tree[index_2]->freq && tree[index]->symbol >tree[index_2]->symbol))
			{
				node* tmp = tree[index];
				tree[index] = tree[index_2];
				tree[index_2] = tmp;
			}
		}
	}
}

bool Huffman::checkGetAllSymbols()
{
	for (unordered_map<char, int>::iterator index = freq_Symbols.begin(); index != freq_Symbols.end(); index++)
	{
		if (index->first != INVALID)
			return true;
	}
	return false;
}

void Huffman::getSymbolsFromFile()
{
	ifstream input(inputfile);
	char symb;
	while (input >> noskipws >> symb)
	{
		if (freq_Symbols.count(symb) == 0)
			freq_Symbols[symb] = 0;
		freq_Symbols[symb]++;

		allSymbol.insert(symb);
	}

	input.close();
}

void Huffman::creatHuffmanTree()
{
	vector<node*> tree;
	while (checkGetAllSymbols())
	{
		pair<char, int> Node = (*freq_Symbols.begin());
		node* newNode(new node(Node.first, Node.second, nullptr, nullptr));
		freq_Symbols.erase(freq_Symbols.begin());
		tree.push_back(newNode);
	}

	sortSymbol(tree);

	while (tree.size() > 1)
	{
		node* first = tree.front();
		tree.erase(tree.begin());

		node* second = tree.front();
		tree.erase(tree.begin());

		root = new node(INVALID, first->freq + second->freq, first, second);

		tree.push_back(root);
		sortSymbol(tree);
	}
}

string Huffman::getPathToLeaf(node* crr, char symbol, string path)
{
	if (crr == nullptr)
		return "";
	else
	{
		if (crr->symbol == symbol)
			return path;
		else
		{
			string leftPath = getPathToLeaf(crr->left, symbol, path + "0");
			if (leftPath != "")
				return leftPath;
			else
				return getPathToLeaf(crr->right, symbol, path + "1");
		}
	}
}

void Huffman::encode()
{
	// STATE : DOING
	getSymbolsFromFile();
	creatHuffmanTree();
	for (set<char>::iterator index = allSymbol.begin();index!=allSymbol.end();index++)
	{
		if (pathOfallSymbols.count(*index) == 0)
		{
			pathOfallSymbols[*index] = "";
		}
		pathOfallSymbols[*index] = getPathToLeaf(root, *index, "");
	}
}
