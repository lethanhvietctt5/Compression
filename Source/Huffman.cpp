﻿#include "Huffman.h"
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <utility>
#include <bitset>

using namespace std;

Huffman::Huffman()
{
	root = new node;
	inputfile = "";
	outputfile = "";
	content = "";
	allPath = "";
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
	for (map<char, int>::iterator index = freq_Symbols.begin(); index != freq_Symbols.end(); index++)
	{
		if (index->first != INVALID)
			return true;
	}
	return false;
}

void Huffman::getSymbolsFromFile(ofstream& out)
{
	ifstream input(inputfile);
	char symb;
	while (input.get(symb))
	{
		if (freq_Symbols.count(symb) == 0)
			freq_Symbols[symb] = 0;
		freq_Symbols[symb]++;

		allSymbol.insert(symb);
		content.push_back(symb);
	}

	int size = freq_Symbols.size();
	out << size;
	out << " ";

	for (map<char, int>::iterator index = freq_Symbols.begin(); index != freq_Symbols.end(); index++)
	{
		out << index->first;
		out << " ";
		out << index->second;
		out << " ";
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

void Huffman::writePathToFile(ofstream& out, string path)
{
	int index_string = 0;
	int count_byte = path.length() / 8;
	for (int i = 0; i < count_byte; i++)
	{
		char tmp;
		tmp = tmp & 0x00;
		for (int index = 0; index < 8; index++)
		{
			if (path[index_string] == '1')
				tmp ^= 0x01;
			if (index != 7)
				tmp <<= 1;
			index_string++;
		}
		out << tmp;
	}

	int missing = 0;

	if ((count_byte * 8) < path.length())
	{
		missing = (count_byte + 1) * 8 - path.length();
		string temp = path.substr(index_string, path.length() - 1);
		for (int index = 0; index < 8 && temp.length() < 8; index++)
		{
			temp.push_back('0');
		}

		char tmp;
		tmp = tmp & 0x00;
		for (int index = 0; index < 8; index++)
		{
			if (temp[index] == '1')
				tmp ^= 0x01;
			if (index != 7)
				tmp <<= 1;
		}

		out << tmp;
	}

	out << (char)(missing);
}

bool isLeaf(node* root)
{
	return (root->left == nullptr && root->right == nullptr);
}

bool restoreTree(node* root, string& result)
{
	if (isLeaf(root)) {
		result += '1';
		bitset<8> temp(root->symbol);
		result += temp.to_string();
		return true;
	}
	else
	{
		result += '0';
		if (restoreTree(root->left, result))
		{
			restoreTree(root->right, result);
		}
	}
}

bool rebuildTree(node*& root, string& code)
{
	while (code.size() != 0)
	{
		if (code[0] == '1')
		{
			bitset<8> symb(code.substr(1, 8));
			root = new node((char)((int)(symb.to_ulong())), NULL, nullptr, nullptr);
			if (code.size() > 8)
				code = code.substr(9, code.size() - 9);
			else code = "";
			return true;
		}
		else
		{
			root = new node(-1, NULL, nullptr, nullptr);
			code = code.substr(1, code.size() - 1);
			rebuildTree(root->left, code);
			rebuildTree(root->right, code);
			return true;
		}
	}
	return true;
}

void Huffman::encode()
{
	// STATE : DOING
	ifstream input(inputfile, ios::binary);
	ofstream output(outputfile, ios::binary);
	getSymbolsFromFile(output);
	creatHuffmanTree();

	for (set<char>::iterator index = allSymbol.begin();index!=allSymbol.end();index++)
	{
		if (pathOfallSymbols.count(*index) == 0)
		{
			pathOfallSymbols[*index] = "";
		}
		pathOfallSymbols[*index] = getPathToLeaf(root, *index, "");
	}

	for (int index = 0; index < content.length(); index++)
	{
		string path = pathOfallSymbols[content[index]];
		allPath.append(path);
	}
	writePathToFile(output, allPath);
}

void Huffman::redefineTree(node* newTree)
{
	root = newTree;
}

bool Huffman::checkLeaf(node* crr)
{
	if (crr != nullptr)
	{
		if (crr->left == nullptr && crr->right == nullptr)
			return true;
	}
	return false;
}

void Huffman::decode()
{
	ifstream input(inputfile, ios::binary);
	ofstream output(outputfile);
	int count_symbols;
	char sym;
	input >> noskipws >> count_symbols;
	input >> noskipws >> sym;
	int index_char = 0;
	while(index_char < count_symbols && input.get(sym))
	{
		char temp;
		input >> noskipws >> temp;
		int freq;
		input >> noskipws >> freq;
		input >> noskipws >> temp;
		freq_Symbols.insert({ sym,freq });
		index_char++;
	}
	creatHuffmanTree();
	char symbol;
	symbol &= 0x00;
	while (input.get(symbol))
	{
		for (int index = 0; index < 8; index++)
		{
			if ((symbol & 0x80) == 0x80)
				allPath.push_back('1');
			else
				allPath.push_back('0');
			symbol <<= 1;
		}
	}

	string tmp = allPath.substr(allPath.length() - 8, allPath.length() - 1);

	char t;
	t = t & 0x00;
	for (int index = 0; index < 8; index++)
	{
		if (tmp[index] == '1')
			t ^= 0x01;
		if (index != 7)
			t <<= 1;
	}

	int missing = (int)(t);

	for (int i = 0; i < missing + 8; i++)
		allPath.pop_back();

	node* temp = root;
	for (int index = 0; index < allPath.length(); index++)
	{
		if (allPath[index] == '0')
		{
			if (checkLeaf(temp->left))
			{
				output << temp->left->symbol;
				temp = root;
			}
			else
				temp = temp->left;
		}
		else if (allPath[index] == '1')
		{
			if (checkLeaf(temp->right))
			{
				output << temp->right->symbol;
				temp = root;
			}
			else
				temp = temp->right;
		}
	}

	input.close();
	output.close();
}

void Huffman::deleteTree(node* root)
{
	if (root != nullptr)
	{
		deleteTree(root->left);
		deleteTree(root->right);
	}
	delete root;
}

Huffman::~Huffman()
{
	allSymbol.clear();
	freq_Symbols.clear();
	pathOfallSymbols.clear();
	deleteTree(root);
}
