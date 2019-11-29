#include "Huffman.h"
#include <iostream>
#include <string>
#include <bitset>
#include <vector>
#include <fstream>
#include <utility>

using namespace std;

Huffman::Huffman()
{
	root = new node;
	inputfile = "";
	outputfile = "";
	allPath = "";
}

Huffman::Huffman(string in, string out)
{
	root = new node;
	inputfile = in;
	outputfile = out;
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

void Huffman::getSymbolsFromFile()
{
	ifstream input(inputfile, ios::binary);
	input.seekg(0, ios_base::end);
	int bufferSize = 1024 * 8, fileSize = input.tellg();
	input.seekg(0, ios_base::beg);
	while (fileSize != 0)
	{
		bufferSize = (bufferSize > fileSize) ? fileSize : bufferSize;
		char* symb = new char[bufferSize];
		fileSize -= bufferSize;
		input.read(symb, bufferSize);
		for (int i = 0; i < bufferSize; i++)
		{
			freq_Symbols[(int)symb[i] + 128]++;
		}
		delete[] symb;
	}
	input.close();
}

void Huffman::creatHuffmanTree()
{
	vector<node*> tree;
	for (int index = 0; index < 256; index++)
	{
		if (freq_Symbols[index] != 0)
		{
			char s = (char)(index - 128);
			node* newNode(new node(s, freq_Symbols[index], nullptr, nullptr, true));
			tree.push_back(newNode);
		}
	}

	sortSymbol(tree);

	while (tree.size() > 1)
	{
		node* first = tree.front();
		tree.erase(tree.begin());

		node* second = tree.front();
		tree.erase(tree.begin());

		root = new node(INVALID, first->freq + second->freq, first, second,false);

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
		if (crr->symbol == symbol && crr->isLeaf)
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
	char* symb = new char[count_byte];
	for (int i = 0; i < count_byte; i++)
	{
		symb[i] = symb[i] & 0x00;
		for (int index = 0; index < 8; index++)
		{
			if (path[index_string] == '1')
				symb[i] ^= 0x01;
			if (index != 7)
				symb[i] <<= 1;
			index_string++;
		}
	}
	out.write(symb, count_byte);

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

bool restoreTree(node* root, string& result)
{
	if (root->isLeaf)
	{
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
			root = new node((char)((int)(symb.to_ulong())), 0, nullptr, nullptr, true);
			if (code.size() > 8)
				code = code.substr(9, code.size() - 9);
			else code = "";
			return true;
		}
		else
		{
			root = new node(INVALID, 0, nullptr, nullptr, false);
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
	ifstream input(inputfile, ios::binary);
	ofstream output(outputfile, ios::binary);

	getSymbolsFromFile();
	creatHuffmanTree();

	int nChar, buffer = 0;
	string Hufftree = "";
	restoreTree(root, Hufftree);
	while (Hufftree.size() % 8)
	{
		Hufftree += '0';
		buffer++;
	}
	nChar = Hufftree.size() / 8;
	output << nChar;
	output << ' ';
	output << buffer;
	output << ' ';
	while (Hufftree.size() != 0)
	{
		bitset<8> character(Hufftree.substr(0, 8));
		output << (char)((int)(character.to_ulong()));
		Hufftree = Hufftree.substr(8, Hufftree.size() - 8);
	}
	for (int index = 0; index < 256; index++)
	{
		if (freq_Symbols[index] != 0)
			pathOfallSymbols[index] = getPathToLeaf(root, (char)(index - 128), "");
	}

	input.seekg(0, ios_base::end);
	int bufferSize = 1024 * 8, fileSize = input.tellg();
	input.seekg(0, ios_base::beg);
	while (fileSize != 0)
	{
		bufferSize = (bufferSize > fileSize) ? fileSize : bufferSize;
		char* symb = new char[bufferSize];
		fileSize -= bufferSize;
		input.read(symb, bufferSize);
		for (int i = 0; i < bufferSize; i++)
		{
			if (pathOfallSymbols[(int)symb[i] + 128] != "")
			{
				allPath += pathOfallSymbols[(int)symb[i] + 128];
			}
		}
		delete[] symb;
	}

	writePathToFile(output, allPath);
}

void Huffman::redefineTree(node* newTree)
{
	root = newTree;
}

void Huffman::decode()
{
	ifstream input(inputfile, ios::binary);
	ofstream output(outputfile, ios::binary);

	string treecode = "";
	int nChar, nbuffer;
	char ctree;
	input >> nChar;
	input >> noskipws >> ctree;
	input >> nbuffer;
	input >> noskipws >> ctree;
	for (int i = 0; i < nChar; i++)
	{
		input >> noskipws >> ctree;
		bitset<8> character(ctree);
		treecode += character.to_string();
	}

	treecode = treecode.substr(0, treecode.size() - nbuffer);
	node* newTree = new node;
	rebuildTree(newTree, treecode);
	redefineTree(newTree);

	/*int here = input.tellg();
	input.seekg(0, ios_base::end);
	int bufferSize = 1024 * 8, fileSize = input.tellg();
	fileSize -= here;
	input.seekg(here, ios_base::beg);
	while (fileSize != 0)
	{
		bufferSize = (bufferSize > fileSize) ? fileSize : bufferSize;
		char* symb = new char[bufferSize];
		fileSize -= bufferSize;
		input.read(symb, bufferSize);
		for (int i = 0; i < bufferSize; i++)
		{
			for (int index = 0; index < 8; index++)
			{
				if ((symb[i] & 0x80) == 0x80)
					allPath.push_back('1');
				else
					allPath.push_back('0');
				symb[i] <<= 1;
			}
		}
	}*/

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
	string tmp = allPath.substr(allPath.length() - 8, 8);

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
	int i = 0, bufferSize = 1024 * 8;
	char* result = new char[bufferSize];
	node* temp = root;
	for (int index = 0; index < allPath.length(); index++)
	{
		if (allPath[index] == '0')
		{
			if (temp->left->isLeaf)
			{
				result[i++] = temp->left->symbol;
				temp = root;
				if (i == bufferSize)
				{
					output.write(result, i);
					i = 0;
				}
			}
			else
				temp = temp->left;
		}
		else if (allPath[index] == '1')
		{
			if (temp->right->isLeaf)
			{
				result[i++] = temp->right->symbol;
				temp = root;
				if (i == bufferSize)
				{
					output.write(result, i);
					i = 0;
				}
			}
			else
				temp = temp->right;
		}
	}
	output.write(result, i);
	delete[] result;
	input.close();
	output.close();
}

void Huffman::deleteTree(node* root)
{
	if (root != nullptr)
	{
		deleteTree(root->left);
		deleteTree(root->right);
		delete root;
	}
}

void Huffman::clear()
{
	deleteTree(root);
	inputfile.clear();
	outputfile.clear();
	//content.clear();
	allPath.clear();

	for (int index = 0; index < 256; index++)
		pathOfallSymbols[index].clear();
}

Huffman::~Huffman()
{
}
