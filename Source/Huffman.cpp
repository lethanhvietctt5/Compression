#include "Huffman.h"

using namespace std;

Huffman::Huffman()
{
	root = new node;
	inputfile = "";
	outputfile = "";
}

Huffman::Huffman(string in, string out)
{
	root = new node;
	inputfile = in;
	outputfile = out;
}

void Huffman::encodeAFileinFolder(ostream& output)
{
	ifstream input(inputfile, ios::binary);
	output << inputfile.size() << " " << inputfile << " ";
	outputfile = "";

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
	int outfilesize = 0;
	for (int i = 0; i < 256; i++)
	{
		outfilesize += freq_Symbols[i] * pathOfallSymbols[i].size();
	}
	if (outfilesize % 8 == 0)
		output << ' ' << (outfilesize / 8 + 1) << ' ';
	else
		output << ' ' << (outfilesize / 8 + 2) << ' ';

	input.seekg(0, ios_base::end);
	int bufferSize = 1024 * 8, outBufferSize = 1024 * 8, fileSize = input.tellg();
	input.seekg(0, ios_base::beg);
	char* temp = new char[outBufferSize];
	int curBit = 0;
	while (fileSize != 0)
	{
		bufferSize = (bufferSize > fileSize) ? fileSize : bufferSize;
		char* symb = new char[bufferSize];
		fileSize -= bufferSize;
		input.read(symb, bufferSize);
		for (int i = 0; i < bufferSize; i++)
		{
			for (int k = 0; k < pathOfallSymbols[(int)symb[i] + 128].size(); k++)
			{
				temp[curBit / 8] <<= 1;
				if (pathOfallSymbols[(int)symb[i] + 128][k] - '0')
					temp[curBit / 8] ^= 0x01;
				curBit++;
				if (curBit / 8 == outBufferSize)
				{
					output.write(temp, outBufferSize);
					delete[] temp; curBit = 0;
					temp = new char[outBufferSize];
				}
			}
		}
		delete[] symb;
	}
	int missing = 0;
	if (curBit != 0)
	{
		if (curBit % 8 != 0)
		{
			while (curBit % 8 != 0)
			{
				temp[curBit / 8] <<= 1;
				missing++;
				curBit++;
			}
			output.write(temp, curBit / 8);
		}
	}
	delete[] temp;
	output << (char)(missing);
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
	int bufferSize = 1024 * 8, outBufferSize = 1024 * 8, fileSize = input.tellg();
	input.seekg(0, ios_base::beg);
	char* temp = new char[outBufferSize];
	int curBit = 0;
	while (fileSize != 0)
	{
		bufferSize = (bufferSize > fileSize) ? fileSize : bufferSize;
		char* symb = new char[bufferSize];
		fileSize -= bufferSize;
		input.read(symb, bufferSize);
		for (int i = 0; i < bufferSize; i++)
		{
			for (int k = 0; k < pathOfallSymbols[(int)symb[i] + 128].size(); k++)
			{
				temp[curBit / 8] <<= 1;
				if (pathOfallSymbols[(int)symb[i] + 128][k] - '0')
					temp[curBit / 8] ^= 0x01;
				curBit++;
				if (curBit / 8 == outBufferSize)
				{
					output.write(temp, outBufferSize);
					delete[] temp; curBit = 0;
					temp = new char[outBufferSize];
				}
			}
		}
		delete[] symb;
	}
	int missing = 0;
	if (curBit != 0)
	{
		if (curBit % 8 != 0)
		{
			while (curBit % 8 != 0)
			{
				temp[curBit / 8] <<= 1;
				missing++;
				curBit++;
			}
			output.write(temp, curBit / 8);
		}
	}
	delete[] temp;
	output << (char)(missing);
}

void Huffman::redefineTree(node* newTree)
{
	root = newTree;
}

void Huffman::decodeFolder(string outfolder)
{
	ifstream input(inputfile, ios::binary);
	int n, nameLength;
	char symb;
	input >> n;
	input >> noskipws >> symb;
	for (int i = 0; i < n; i++)
	{
		string filename;
		node* root = new node;
		input >> nameLength;
		input >> noskipws >> symb;
		filename.resize(nameLength);
		for (int k = 0; k < nameLength; k++)
		{
			input >> filename[k];
		}
		filename = outfolder + '\\' + filename;
		ofstream output(filename, ios::binary);
		if (output.fail())
		{
			string cmd = "mkdir " + filename.substr(0, filename.rfind('\\'));
			system(cmd.c_str());
			output.open(filename, ios::binary);
		}
		input >> noskipws >> symb;
		string treecode = "", allPath = "";
		int fileSize, nChar, nbuffer;
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
		rebuildTree(root, treecode);
		input >> noskipws >> symb;
		input >> fileSize;
		input >> noskipws >> symb;
		
		int bufferSize = 1024 * 8;
		node* temp = root;
		while (fileSize != 0)
		{
			int missing = 0;
			char* symb = new char[bufferSize];
			if (bufferSize > fileSize)
			{
				bufferSize = fileSize;
				input.read(symb, bufferSize);
				missing = (int)symb[bufferSize - 1];
				bufferSize--;
				fileSize = 0;
			}
			else
			{
				input.read(symb, bufferSize);
				fileSize -= bufferSize;
			}
			decodeBuffer(symb, bufferSize, root, temp, missing, output);
			delete[] symb;
		}
		output.close();

	}
	input.close();
}

bool getBit(char byte, int pos)
{
	return (byte >> pos) & 0x1;
}

void decodeBuffer(char* symb, int bufferSize, node* root, node* &temp, int missing, ofstream &output)
{
	for (int i = 0; i < bufferSize - 1; i++)
	{
		for (int index = 7; index >= 0; index--)
		{
			if (getBit(symb[i], index) == 0)
			{
				if (temp->left->isLeaf)
				{
					output.write(&temp->left->symbol, 1);
					temp = root;
				}
				else
					temp = temp->left;
			}
			else if (getBit(symb[i], index) == 1)
			{
				if (temp->right->isLeaf)
				{
					output.write(&temp->right->symbol, 1);
					temp = root;
				}
				else
					temp = temp->right;
			}
		}
	}
	for (int i = 7; i >= missing; i--)
	{
		if (getBit(symb[bufferSize - 1], i) == 0)
		{
			if (temp->left->isLeaf)
			{
				output.write(&temp->left->symbol, 1);
				temp = root;
			}
			else
				temp = temp->left;
		}
		else if (getBit(symb[bufferSize - 1], i) == 1)
		{
			if (temp->right->isLeaf)
			{
				output.write(&temp->right->symbol, 1);
				temp = root;
			}
			else
				temp = temp->right;
		}
	}
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

	int here = input.tellg();
	input.seekg(0, ios_base::end);
	int bufferSize = 1024 * 8, fileSize = input.tellg();
	fileSize -= here;
	input.seekg(here, ios_base::beg);
	node* temp = root;
	while (fileSize != 0)
	{
		int missing = 0;
		char* symb = new char[bufferSize];
		if (bufferSize > fileSize)
		{
			bufferSize = fileSize;
			input.read(symb, bufferSize);
			missing = (int)symb[bufferSize - 1];
			bufferSize--;
			fileSize = 0;
		}
		else
		{
			input.read(symb, bufferSize);
			fileSize -= bufferSize;
		}		
		decodeBuffer(symb, bufferSize, root, temp, missing, output);
		delete[] symb;
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
		delete root;
	}
}

void Huffman::clear()
{
	deleteTree(root);
	inputfile.clear();
	outputfile.clear();

	for (int index = 0; index < 256; index++)
		pathOfallSymbols[index].clear();
}

Huffman::~Huffman()
{
}
