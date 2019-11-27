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
	Huffman huff("static_HUFF.exe", "output.txt");
	huff.encode();

	Huffman h("output.txt", "decode.exe");
	h.decode();
	return 1;
}