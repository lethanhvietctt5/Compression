#include "Folder.h"

using namespace folder;

void folder::ListFileInFolder(std::vector<std::string>& list_file, std::string path_folder, std::string path_header)
{
	WIN32_FIND_DATA find_file_data;

	HANDLE hFind = FindFirstFile(path_folder.c_str(), &find_file_data);
	std::string temp = find_file_data.cFileName;
	if (temp != "." && temp != "..")
	{
		temp = path_header + temp;
		list_file.push_back(temp);
	}
	while (FindNextFile(hFind, &find_file_data))
	{
		std::string temp = find_file_data.cFileName;
		if (temp != "." && temp != "..")
		{
			if (find_file_data.dwFileAttributes == 16)
			{
				std::string path = path_folder.substr(0, path_folder.length() - 1) + find_file_data.cFileName + "\\*";
				ListFileInFolder(list_file, path, path_header + find_file_data.cFileName + '\\');
			}
			else
			{
				temp = path_header + temp;
				list_file.push_back(temp);
			}
		}
	}
}

void Folder::open()
{
	ListFileInFolder(filename, name, name.substr(0, name.length()-1));
	for (int i = 0; i < filename.size(); i++)
	{
		for (int k = 0; k < filename[i].size(); k++)
		{
			if (filename[i][k] == '\\')
				filename[i][k] = '/';
		}
	}
}

void folder::sortSymbol(vector<node*>& tree)
{
	for (int index = 0; index < tree.size(); index++)
	{
		for (int index_2 = index + 1; index_2 < tree.size(); index_2++)
		{
			if ((tree[index]->freq > tree[index_2]->freq) || (tree[index]->freq == tree[index_2]->freq && tree[index]->symbol > tree[index_2]->symbol))
			{
				node* tmp = tree[index];
				tree[index] = tree[index_2];
				tree[index_2] = tmp;
			}
		}
	}
}

bool folder::checkGetAllSymbols(map<char, int> freq_Symbols)
{
	for (map<char, int>::iterator index = freq_Symbols.begin(); index != freq_Symbols.end(); index++)
	{
		if (index->first != INVALID)
			return true;
	}
	return false;
}

void folder::getSymbolsFromFile(string inputfile, map<char, int> &freq_Symbols, set<char> &allSymbol, string &content)
{
	//inputfile = "test/alo/input.txt";
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

void folder::creatHuffmanTree(node* &root, map<char, int> &freq_Symbols)
{
	vector<node*> tree;
	while (checkGetAllSymbols(freq_Symbols))
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

string folder::getPathToLeaf(node* crr, char symbol, string path)
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

void folder::writePathToFile(ofstream& out, string path)
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

bool folder::isLeaf(node* root)
{
	return (root->left == nullptr && root->right == nullptr);
}

bool folder::restoreTree(node* root, string& result)
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

bool folder::rebuildTree(node*& root, string& code)
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

void Folder::encode(string outputfile)
{
	ofstream output(outputfile, ios::binary);
	output << filename.size() << " ";
	for (int i = 0; i < filename.size(); i++)
	{
		ifstream input(filename[i], ios::binary);
		output << filename[i].size() << " " << filename[i] << " ";
		node* root = new node;
		set<char> allSymbol;
		map<char, int> freq_Symbols;
		map<char, string> pathOfallSymbols;
		string allPath = "", content = "";
		outputfile = "";
		freq_Symbols.clear();
		pathOfallSymbols.clear();

		getSymbolsFromFile(filename[i], freq_Symbols, allSymbol, content);
		creatHuffmanTree(root, freq_Symbols);

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
		for (set<char>::iterator index = allSymbol.begin(); index != allSymbol.end(); index++)
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
		if (allPath.size() % 8 == 0)
			output << " " << allPath.size() / 8 + 1 << " ";
		else 
			output << " " << allPath.size() / 8 + 2 << " ";
		writePathToFile(output, allPath);
	}
}

void Folder::decode(string inputfile, string outfolder)
{
	ifstream input(inputfile, ios::binary);
	int n, nameLength;
	char symb;
	input >> n;
	input >> noskipws >> symb;
	for (int i = 0; i < n; i++)
	{
		node* root = new node;
		input >> nameLength;
		input >> noskipws >> symb;
		for (int k = 0; k < nameLength; k++)
		{
			input >> filename[i][k];
		}
		filename[i] = outfolder + '/' + filename[i];
		ofstream output(filename[i], ios::binary); // loi khong tu tao folder neu khong co folder
		input >> noskipws >> symb;
		string treecode = "", allPath = "";
		int nByte, nChar, nbuffer;
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
		input >> nByte;
		input >> noskipws >> symb;
		for (int i = 0; i < nByte; i++)
		{
			char symbol;
			symbol &= 0x00;
			input.get(symbol);
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

		node* temp = root;
		for (int index = 0; index < allPath.length(); index++)
		{
			if (allPath[index] == '0')
			{
				if (isLeaf(temp->left))
				{
					output << temp->left->symbol;
					temp = root;
				}
				else
					temp = temp->left;
			}
			else if (allPath[index] == '1')
			{
				if (isLeaf(temp->right))
				{
					output << temp->right->symbol;
					temp = root;
				}
				else
					temp = temp->right;
			}
		}
		output.close();
	}
	input.close();
}

Folder::Folder()
{
	name = "";
}

Folder::Folder(string input)
{
	name = input + "\\*";
}