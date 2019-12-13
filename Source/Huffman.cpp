#include "Huffman.h"

using namespace std;

Huffman::Huffman()
{
	root = new node;
	inputfile = "";
	outputfile = "";
}

Huffman::Huffman(string in, string out)
{
	root = new node;
	inputfile = in;
	outputfile = out;
}

void fixWhiteSpace(std::string &path)
{
	path.insert(path.begin(), '\"');
	for (int i = 0; i < path.size(); i++)
	{
		if (path[i] == '\\')
		{
			path.insert(path.begin() + i, '\"');
			path.insert(path.begin() + i + 2, '\"');
			i++;
		}
	}
	path += '\"';
}

void Huffman::encodeAFileinFolder(ostream& output)
{	// hàm nén một file con vào 1 tập tin nén folder
	string newName = inputfile;
	inputfile = inputfile.substr(1, inputfile.size() - 1);
	ifstream input(inputfile, ios::binary);
	output << newName.size() << " " << newName << " ";
	// in ra chiều dài đường dẫn và đường dẫn đến file con
	getSymbolsFromFile(); // tạo bảng tần số xuất hiện của các kí tự
	creatHuffmanTree(); // tạo cây mã hóa Huffman bằng bảng tần số đã đọc 

	int nChar, buffer = 0;
	string Hufftree = "";
	restoreTree(root, Hufftree); // mã hóa cây thành dãy nhị phân lưu vào chuỗi Hufftree
	while (Hufftree.size() % 8)
	{	// bù vào các bit còn thiếu để đủ 8 bit
		Hufftree += '0';
		buffer++;
	}
	nChar = Hufftree.size() / 8;
	output << nChar; // in ra chiều dài đoạn kí tự mã hóa cây
	output << ' ';
	output << buffer; // in ra số lượng bit thêm vào để đủ 8 bit
	output << ' ';
	while (Hufftree.size() != 0)
	{
		bitset<8> character(Hufftree.substr(0, 8));
		output << (char)((int)(character.to_ulong()));
		Hufftree = Hufftree.substr(8, Hufftree.size() - 8);
	}
	for (int index = 0; index < 256; index++)
	{	// tạo mảng các đường dẫn đến kí tự lưu vào biến pathOfAllSymbols
		if (freq_Symbols[index] != 0)
			pathOfallSymbols[index] = getPathToLeaf(root, (char)(index - 128), "");
	}
	int outfilesize = 0;
	for (int i = 0; i < 256; i++)
	{	// tính toán độ lớn của file
		outfilesize += freq_Symbols[i] * pathOfallSymbols[i].size();
	}
	if (outfilesize % 8 == 0) // ghi lại độ lớn của file vào file output
		output << ' ' << (outfilesize / 8 + 1) << ' ';
	else
		output << ' ' << (outfilesize / 8 + 2) << ' ';

	input.seekg(0, ios_base::end);
	int bufferSize = 1024 * 8, outBufferSize = 1024 * 8, fileSize = input.tellg();
	input.seekg(0, ios_base::beg);
	char* temp = new char[outBufferSize]; // tạo một buffer để in ra bằng hàm write
	int curBit = 0;
	while (fileSize != 0)
	{	// nếu số kí tự còn lại trong file không đủ cho 1 buffer thì giảm size của buffer xuống
		bufferSize = (bufferSize > fileSize) ? fileSize : bufferSize;
		char* symb = new char[bufferSize]; // tạo một buffer char để đọc vào bằng hàm read
		fileSize -= bufferSize; // tính toán độ dài còn lại của file
		input.read(symb, bufferSize);
		for (int i = 0; i < bufferSize; i++)
		{
			for (int k = 0; k < pathOfallSymbols[(int)symb[i] + 128].size(); k++)
			{	// xử dụng phép toán trên bit để viết kí tự theo bit
				temp[curBit / 8] <<= 1;
				if (pathOfallSymbols[(int)symb[i] + 128][k] - '0')
					temp[curBit / 8] ^= 0x01;
				curBit++;
				if (curBit / 8 == outBufferSize)
				{	// nếu chiều dài của buffer output đã đủ thì in ra file output
					output.write(temp, outBufferSize);
					delete[] temp; curBit = 0;
					temp = new char[outBufferSize];
				}
			}
		}
		delete[] symb;
	}
	int missing = 0;
	if (curBit != 0)
	{	// nếu buffer còn dư chưa in ra hết thì thêm vào sao cho đủ 8 bit rồi in ra
		while (curBit % 8 != 0)
		{
			temp[curBit / 8] <<= 1;
			missing++;
			curBit++;
		}
		output.write(temp, curBit / 8);
	}
	delete[] temp;
	output << (char)(missing); // in ra file output số bit thêm vào để đủ 8 bit
}

void Huffman::sortSymbol(vector<node*> &tree)
{
	//TODO: Sắp xếp các node chứa các ký tự theo tần số tăng dần
	for (int index = 0; index < tree.size(); index++)
	{
		for (int index_2 = index + 1; index_2 < tree.size(); index_2++)
		{
			// Hai ký tự có tần suất khác nhau hoặc nếu bằng nhau thì so sánh thứ tự trong bảng mã ascii
			if ((tree[index]->freq > tree[index_2]->freq) || (tree[index]->freq == tree[index_2]->freq && tree[index]->symbol >tree[index_2]->symbol))
			{
				node* tmp = tree[index];
				tree[index] = tree[index_2];
				tree[index_2] = tmp;
			}
		}
	}
}

void Huffman::getSymbolsFromFile()
{
	//TODO: Lấy các ký tự khác nhau xuất hiện trong dữ liệu
	ifstream input(inputfile, ios::binary);

	input.seekg(0, ios_base::end);
	int bufferSize = 1024 * 8, fileSize = input.tellg();	// Lấy chiều dài nội dung của dữ liệu
	input.seekg(0, ios_base::beg);
	while (fileSize != 0)
	{
		// đọc bufferSize ký tự từ file và tăng tần số cho các ký tự
		bufferSize = (bufferSize > fileSize) ? fileSize : bufferSize;
		char* symb = new char[bufferSize];
		fileSize -= bufferSize;
		input.read(symb, bufferSize);
		for (int i = 0; i < bufferSize; i++)
			freq_Symbols[(int)symb[i] + 128]++;
		delete[] symb;
	}
	input.close();
}

void Huffman::creatHuffmanTree()
{
	//TODO: tạo cây Huffman

	vector<node*> tree;
	
	//Tạo các node chứa các ký tự
	for (int index = 0; index < 256; index++)
	{
		if (freq_Symbols[index] != 0)
		{
			char s = (char)(index - 128);
			node* newNode(new node(s, freq_Symbols[index], nullptr, nullptr, true));
			tree.push_back(newNode);
		}
	}

	if (tree.size() == 1)
	{
		node* first = tree.front();
		root = new node(INVALID, first->freq + first->freq, first, first, false);
		tree.erase(tree.begin());
	}

	sortSymbol(tree);	// sắp xếp theo tần số tăng dần

	while (tree.size() > 1)
	{
		// Lấy hai node có tần số nhỏ nhất gộp thành một, xóa bỏ hai node đó và thêm node mới vào tree
		// Lặp lại cho đến khi tree chỉ còn 1 node duy nhất
		node* first = tree.front();
		tree.erase(tree.begin());

		node* second = tree.front();
		tree.erase(tree.begin());

		root = new node(INVALID, first->freq + second->freq, first, second,false);

		tree.push_back(root);
		sortSymbol(tree);
	}
}

string Huffman::getPathToLeaf(node* crr, char symbol, string path)
{
	//TODO: Lấy chuỗi bit mã hóa cho các ký tự ở node lá
	if (crr == nullptr)
		return "";
	else
	{
		// Đến node lá thì return chuỗi bit
		if (crr->symbol == symbol && crr->isLeaf)
			return path;
		else
		{
			string leftPath = getPathToLeaf(crr->left, symbol, path + "0");
			if (leftPath != "")
				return leftPath;
			else
				return getPathToLeaf(crr->right, symbol, path + "1");
		}
	}
}

bool restoreTree(node* root, string& result)
{
	if (root->isLeaf)
	{	// nếu node là node lá thì ta ghi số 1 để biểu thị node lá và 8 bit để biểu thị kí tự của node
		result += '1';
		bitset<8> temp(root->symbol);
		result += temp.to_string();
		return true;
	}
	else
	{
		// nếu node không phải là node lá thì ta ghi số 0 sau đó đệ quy để tiếp tục lưu theo thứ tự Node Left Right
		result += '0';
		if (restoreTree(root->left, result))
		{
			restoreTree(root->right, result);
		}
	}
}

bool rebuildTree(node*& root, string& code)
{	// đọc chuỗi nhị phân theo thứ tự Node Left Right
	while (code.size() != 0)
	{
		if (code[0] == '1')
		{	// nếu là 1 thì node hiện tại đang xét là node lá, 8 bit sau biểu thị cho kí tự của node
			bitset<8> symb(code.substr(1, 8));
			root = new node((char)((int)(symb.to_ulong())), 0, nullptr, nullptr, true);
			if (code.size() > 8)
				code = code.substr(9, code.size() - 9);
			else code = "";
			return true;
		}
		else
		{	// nếu là 0 thì node hiện tại không phải là node lá, ta đệ quy vào left và right để tiếp tục lưu cây
			root = new node(INVALID, 0, nullptr, nullptr, false);
			code = code.substr(1, code.size() - 1);
			rebuildTree(root->left, code);
			rebuildTree(root->right, code);
			return true;
		}
	}
	return true;
}

void Huffman::encode()
{
	//TODO: Nén dữ liệu

	// Mở file input và output
	ifstream input(inputfile, ios::binary);
	ofstream output(outputfile, ios::binary);

	getSymbolsFromFile();	// Lấy ký tự và tạo bảng tần số
	creatHuffmanTree();		// Tạo cây Huffman từ bảng tần số

	int nChar, buffer = 0;
	string Hufftree = "";
	restoreTree(root, Hufftree);	// Lưu trữ thông tin cây Huffman để giải nén
	while (Hufftree.size() % 8)
	{
		Hufftree += '0';
		buffer++;
	}
	nChar = Hufftree.size() / 8;
	output << nChar;	// Lưu trữ số ký tự
	output << ' ';
	output << buffer;	// Lưu trữ số mã bit bị thiếu 
	output << ' ';
	while (Hufftree.size() != 0)
	{
		// Quy đổi chuỗi bit thông tin cây Huffman thành các ký tự để lưu trữ vào file nén
		bitset<8> character(Hufftree.substr(0, 8));
		output << (char)((int)(character.to_ulong()));
		Hufftree = Hufftree.substr(8, Hufftree.size() - 8);
	}
	for (int index = 0; index < 256; index++)
	{
		// Lấy chuỗi bit mã hóa của các ký tự
		if (freq_Symbols[index] != 0)
			pathOfallSymbols[index] = getPathToLeaf(root, (char)(index - 128), "");
	}

	input.seekg(0, ios_base::end);
	int bufferSize = 1024 * 8, outBufferSize = 1024 * 8, fileSize = input.tellg();
	input.seekg(0, ios_base::beg);
	char* temp = new char[outBufferSize]; // tạo một buffer để in ra bằng hàm write
	int curBit = 0;
	while (fileSize != 0)
	{	// nếu số kí tự còn lại không đủ cho 1 buffer thì ta giảm độ lớn của buffer xuống
		bufferSize = (bufferSize > fileSize) ? fileSize : bufferSize;
		char* symb = new char[bufferSize];
		fileSize -= bufferSize; // tính toán độ lớn còn lại của buffer
		input.read(symb, bufferSize);
		for (int i = 0; i < bufferSize; i++)
		{
			for (int k = 0; k < pathOfallSymbols[(int)symb[i] + 128].size(); k++)
			{   // ghi ra char bằng các phép tính trên bit
				temp[curBit / 8] <<= 1;
				if (pathOfallSymbols[(int)symb[i] + 128][k] - '0')
					temp[curBit / 8] ^= 0x01;
				curBit++;
				if (curBit / 8 == outBufferSize)
				{	// nếu đã đầy 1 buffer thì in ra file output bằng hàm write
					output.write(temp, outBufferSize);
					delete[] temp; curBit = 0;
					temp = new char[outBufferSize];
				}
			}
		}
		delete[] symb;
	}
	int missing = 0;
	if (curBit != 0)
	{	// nếu buffer còn dư kí tự chưa in ra thì ta thêm vào sao cho đủ 8 bit sau đó ghi ra phần dư
		while (curBit % 8 != 0)
		{
			temp[curBit / 8] <<= 1;
			missing++;
			curBit++;
		}
		output.write(temp, curBit / 8);
	}
	delete[] temp;
	output << (char)(missing); // ghi ra output file số lượng bit thêm vào để đủ 8 bit
}

void Huffman::redefineTree(node* newTree)
{
	root = newTree;
}

void Huffman::decodeFolder(string outfolder)
{
	ifstream input(inputfile, ios::binary);
	int n, nameLength;
	char symb;
	input >> n; // đọc vào số lượng file (và folder rỗng) chứa trong file nén
	input >> noskipws >> symb;
	for (int i = 0; i < n; i++)
	{
		string filename;
		node* root = new node;
		input >> nameLength; // đọc chiều dài đường dẫn đến file hoặc folder rỗng
		input >> noskipws >> symb;
		filename.resize(nameLength);
		for (int k = 0; k < nameLength; k++)
		{   // đọc đường dẫn
			input >> filename[k];
		}
		if (filename[0] == '0')
		{   // nếu là đường dẫn đến file thì ta mở file và bắt đầu giải nén
			filename = filename.substr(1, filename.size() - 1);
			filename = outfolder + '\\' + filename;
			ofstream output(filename, ios::binary);
			if (output.fail())
			{   // nếu đọc file thất bại thì ta tạo 1 đường dẫn đến file bằng đường dẫn đã đọc vào sau đó tạo file để bắt đầu giải nén
				string temp = filename.substr(0, filename.rfind('\\'));
				fixWhiteSpace(temp);
				string cmd = "mkdir " + temp;
				system(cmd.c_str());
				output.open(filename, ios::binary);
			}
			input >> noskipws >> symb;
			string treecode = "", allPath = "";
			int fileSize, nChar, nbuffer;
			char ctree;
			input >> nChar; // đọc vào số kí tự của đoạn nhị phân lưu cây
			input >> noskipws >> ctree;
			input >> nbuffer; // đọc vào số bit thêm vào để đủ 8 bit
			input >> noskipws >> ctree;
			for (int i = 0; i < nChar; i++)
			{
				input >> noskipws >> ctree;
				bitset<8> character(ctree);
				treecode += character.to_string();
			}
			treecode = treecode.substr(0, treecode.size() - nbuffer);
			rebuildTree(root, treecode); // tạo lại cây Huffman bằng đoạn mã nhị phân đã ghi ở đầu file
			input >> noskipws >> symb;
			input >> fileSize; // đọc vào độ lớn của file đang giải nén
			input >> noskipws >> symb;

			int bufferSize = 1024 * 8;
			node* temp = root;
			while (fileSize != 0)
			{
				int missing = 0;
				char* symb = new char[bufferSize];
				if (bufferSize > fileSize)
				{	// nếu đã đọc đến đoạn cuối của file
					bufferSize = fileSize; //giảm buffer xuống để đọc vào vừa đủ các kí tự còn lại của file
					input.read(symb, bufferSize);
					missing = (int)symb[bufferSize - 1]; // đọc số lượng bit thêm vào để đủ 8 bit
					bufferSize--; // giảm độ lớn của buffer xuống 1 để không đọc lại số lượng bit thêm vào
					fileSize = 0;
				}
				else
				{	//nếu file vẫn còn nhiều hơn 1 buffer thì ta đọc như bình thường và tính toán lại độ lớn file
					input.read(symb, bufferSize);
					fileSize -= bufferSize;
				}
				decodeBuffer(symb, bufferSize, root, temp, missing, output);
				delete[] symb;
			}
			output.close();
		}
		else if (filename[0] == '1')
		{
			filename = filename.substr(1, filename.size() - 1);
			filename = outfolder + '\\' + filename;
			fixWhiteSpace(filename);
			string cmd = "mkdir " + filename;
			system(cmd.c_str());
			input >> noskipws >> symb;
		}
	}
	input.close();
}

bool getBit(char byte, int pos)
{
	return (byte >> pos) & 0x1;
}

void decodeBuffer(char* symb, int bufferSize, node* root, node* &temp, int missing, ofstream &output)
{	// hàm giải nén 1 buffer char
	for (int i = 0; i < bufferSize - 1; i++)
	{	// từ buffer 1 đến buffer bufferSize - 1 thì ta giải nén như bình thường
		for (int index = 7; index >= 0; index--)
		{
			if (getBit(symb[i], index) == 0)
			{	// nếu đọc vào 0 thì ta đi sang bên trái
				if (temp->left->isLeaf)
				{	// nếu đến node lá thì ta ghi kí tự ở đó ra file
					output.write(&temp->left->symbol, 1);
					temp = root;
				}
				else
					temp = temp->left;
			}
			else if (getBit(symb[i], index) == 1)
			{	// nếu đọc vào 1 thì ta đi sang bên phải
				if (temp->right->isLeaf)
				{	// nếu đến node lá thì ta ghi kí tự ở đó ra file
					output.write(&temp->right->symbol, 1);
					temp = root;
				}
				else
					temp = temp->right;
			}
		}
	}
	for (int i = 7; i >= missing; i--)
	{	// ở kí tự cuối, nếu ta có thêm vào (missing) bit để đủ 8 bit thì bỏ qua (missing) bit cuối cùng sau đó làm tương tự như trên
		if (getBit(symb[bufferSize - 1], i) == 0)
		{
			if (temp->left->isLeaf)
			{
				output.write(&temp->left->symbol, 1);
				temp = root;
			}
			else
				temp = temp->left;
		}
		else if (getBit(symb[bufferSize - 1], i) == 1)
		{
			if (temp->right->isLeaf)
			{
				output.write(&temp->right->symbol, 1);
				temp = root;
			}
			else
				temp = temp->right;
		}
	}
}

void Huffman::decode()
{
	ifstream input(inputfile, ios::binary);
	ofstream output(outputfile, ios::binary);

	string treecode = "";
	int nChar, nbuffer;
	char ctree;
	input >> nChar; // đọc vào số lượng kí tự để lưu lại cây Huffman
	input >> noskipws >> ctree;
	input >> nbuffer; // đọc vào số lượng bit thêm vào để đủ 8 bit
	input >> noskipws >> ctree;
	for (int i = 0; i < nChar; i++)
	{
		input >> noskipws >> ctree;
		bitset<8> character(ctree);
		treecode += character.to_string();
	}

	treecode = treecode.substr(0, treecode.size() - nbuffer);
	node* newTree = new node;
	rebuildTree(newTree, treecode); // tạo lại cây từ đoạn nhị phân đã được mã hóa
	redefineTree(newTree);

	int here = input.tellg();
	input.seekg(0, ios_base::end);
	int bufferSize = 1024 * 8, fileSize = input.tellg();
	fileSize -= here; // tính toán độ dài còn lại của file
	input.seekg(here, ios_base::beg);
	node* temp = root;
	while (fileSize != 0)
	{
		int missing = 0;
		char* symb = new char[bufferSize];
		if (bufferSize > fileSize)
		{	// nếu số kí tự còn lại không đủ cho 1 buffer thì ta giảm số lượng buffer xuống
			bufferSize = fileSize;
			input.read(symb, bufferSize);
			missing = (int)symb[bufferSize - 1]; // đọc vào số bit thêm vào để đủ 8 bit
			bufferSize--; // giảm bufferSize xuống để không đọc lại số lượng bit thêm vào nữa
			fileSize = 0;
		}
		else
		{
			input.read(symb, bufferSize);
			fileSize -= bufferSize;
		}		
		decodeBuffer(symb, bufferSize, root, temp, missing, output);
		delete[] symb;
	}
	input.close();
	output.close();
}

void Huffman::deleteTree(node* root)
{
	if (root != nullptr)
	{
		deleteTree(root->left);
		deleteTree(root->right);
		delete root;
	}
}

void Huffman::clear()
{
	deleteTree(root);
	inputfile.clear();
	outputfile.clear();

	for (int index = 0; index < 256; index++)
		pathOfallSymbols[index].clear();
}

Huffman::~Huffman()
{
}
