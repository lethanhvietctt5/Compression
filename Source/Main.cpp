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
	Huffman huff("input.txt", "output.txt");
	huff.encode();

	Huffman h("output.txt", "decode.txt");
	h.decode();
	return 1;
}