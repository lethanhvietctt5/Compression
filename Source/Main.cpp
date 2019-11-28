#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>
#include <unordered_map>
#include <utility>
#include "Huffman.h"

using namespace std;

int main()
{
	Huffman huff("input.txt", "output.dat");
	huff.encode();
	huff.clear();

	Huffman h("output.dat", "decode.txt");
	h.decode();
	//huff.clear();
	return 1;
}