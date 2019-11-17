#include <iostream>
#include <fstream>
#include <bitset>
#include <string>
#include <sstream>
#include "AdaptiveHuffman.h"

using namespace std;

int main()
{
	ifstream in("in_file.txt");
	ofstream ou("out_file.txt");
	AdaptiveHuffman encode;
	encode.encode(in, ou);
	return 1;
}