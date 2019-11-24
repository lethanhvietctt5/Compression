#include "Huffman.h"
#include <iostream>
#include <string>
#include <unordered_map>
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
	ifstream input(inputfile, ios::binary);
	char symb;
	while (input >> noskipws >> symb)
	{
		if (freq_Symbols.count(symb) == 0)
			freq_Symbols[symb] = 0;
		freq_Symbols[symb]++;

		allSymbol.insert(symb);
		content.push_back(symb);
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
	if ((count_byte * 8) < path.length())
	{
		string temp = path.substr(index_string, path.length() - 1);
		bitset<8> bit(temp);
		char c = (unsigned char)(bit.to_ulong());
		out << c;
	}
}

void Huffman::encode()
{
	// STATE : DOING
	ifstream input(inputfile, ios::binary);
	ofstream output(outputfile);
	getSymbolsFromFile();

	// Gọi hàm lưu cây tại đây

	creatHuffmanTree();
	for (set<char>::iterator index = allSymbol.begin();index!=allSymbol.end();index++)
	{
		if (pathOfallSymbols.count(*index) == 0)
		{
			pathOfallSymbols[*index] = "";
		}
		pathOfallSymbols[*index] = getPathToLeaf(root, *index, "");
	}

	string allPath = "";
	for (int index = 0; index < content.length(); index++)
	{
		string path = pathOfallSymbols[content[index]];
		allPath.append(path);
	}
	writePathToFile(output, allPath);
}

bool Huffman::checkLeaf(node* crr, char sym)
{
	if (crr != nullptr)
	{
		if (crr->symbol == sym)
			return true;
	}
	return false;
}

void Huffman::decode()
{
	ifstream input(inputfile, ios::binary);
	ofstream output(outputfile);

	// đọc file build lại cây

	char symbol;
	while (input >> noskipws >> symbol)
	{
		string path = "";
	}
}
