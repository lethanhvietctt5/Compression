#pragma once
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <conio.h>
#include <io.h>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <bitset>

#define INVALID -1

using namespace std;

class Folder
{
private:
	vector<string> filename;
	string name;
public:
	void open();
	void encode(string outputfile);
	void decode(string inputfile, string outfolder);
	Folder();
	Folder(string input);
};
namespace folder {
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

		node(char s, int f, node* l, node* r, bool il)
		{
			symbol = s;
			freq = f;
			left = l;
			right = r;
			isLeaf = il;
		}
	};
	void ListFileInFolder(std::vector<std::string>& list_file, std::string path_folder, std::string path_header);
	bool isLeaf(node* root);
	void sortSymbol(vector<node*>& tree);
	void getSymbolsFromFile(string inputfile, map<char, int>& freq_Symbols, set<char>& allSymbol, string& content);
	void creatHuffmanTree(node* &root, map<char, int>& freq_Symbols);
	string getPathToLeaf(node* crr, char symbol, string path);
	void writePathToFile(ofstream& out, string path);
	bool restoreTree(node* root, string& result);
	bool rebuildTree(node*& root, string& code);
}