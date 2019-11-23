#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include "Huffman.h"

using namespace std;

int main()
{
	Huffman huff("input.txt", "output.txt");
	huff.encode();
	return 1;
}