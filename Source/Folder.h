#pragma once
#include "Huffman.h"
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string>
#include <vector>
#include <bitset>
#define INVALID 0

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