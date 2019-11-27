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
	Huffman huff("abc.exe", "output.dat");
	huff.encode();

	Huffman h("output.dat", "decode.exe");
	h.decode();
	return 1;
}