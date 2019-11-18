#include <iostream>
#include <fstream>
#include <bitset>
#include <string>
#include "AdaptiveHuffman.h"

using namespace std;

int main()
{
	ifstream in("in_file.txt");
	ofstream ou("out_file.txt");
	AdaptiveHuffman encode;
	encode.encode(in, ou);

	//ifstream i("out_file.txt");
	//ofstream o("o.txt");
	//AdaptiveHuffman decode;
	//decode.decode(i, o);
	return 1;
}