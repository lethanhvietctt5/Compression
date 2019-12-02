#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <bitset>

#define INVALID 0

using namespace std;

struct node
{
	char symbol;	//ký tự
	int freq;		//tần số xuất hiện của ký tự trong dữ liệu
	node* left;		// node trái
	node* right;	// node phải
	bool isLeaf;	// kiểm tra node lá

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
	string inputfile;	// tên file input
	string outputfile;	// tên file output
	node* root;			// node gốc của cây Huffman
	int freq_Symbols[256] = { 0 };	// bảng tần số các ký tự
	string pathOfallSymbols[256] = { "" };	// bảng mã hóa ký tự thành chuỗi bit
public:
	Huffman();
	Huffman(string in, string out);
	// Encode
	void sortSymbol(vector<node*> &tree);	// sort các node theo tần số tăng dần
	void getSymbolsFromFile();				// lấy các ký tự khác nhau xuất hiện trong dữ liệu
	void creatHuffmanTree();				// tạo cây Huffman
	string getPathToLeaf(node* crr, char symbol, string path);		// Lấy chuỗi bit mã hóa cho các ký tự ở node lá
	void encodeAFileinFolder(ostream& output);
	void encode();		// Nén dữ liệu

	// Decode
	void redefineTree(node* newTree);
	void decodeFolder(string outfolder);
	void decode();		// Giải nén dữ liệu đã nén
	void deleteTree(node* root);	// Delete cây Huffman
	void clear();		// giải phóng bộ nhớ
	~Huffman();
};

void decodeBuffer(char* symb, int bufferSize, node* root, node*& temp, int missing, ofstream& output);
bool getBit(char byte, int pos);
bool rebuildTree(node*& root, string& code);
bool restoreTree(node* root, string& result);