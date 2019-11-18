#include "AdaptiveHuffman.h"
#include <iostream>
#include <fstream>
#include <string>
#include <bitset>

using namespace std;

AdaptiveHuffman::AdaptiveHuffman()
{
	root = new Node(ILLEGAL, 0, MAX_ORDER, nullptr, nullptr, nullptr, true, false);
	NYT = root;
}

//AdaptiveHuffman::AdaptiveHuffman(const AdaptiveHuffman& copy)
//{
//}

Node* AdaptiveHuffman::getNodeOfSymbol(char symb, Node* root)
{
	if (root == nullptr || root->symbol == symb)
		return root;

	Node* symbolLeft = getNodeOfSymbol(symb, root->left);
	return symbolLeft == nullptr ? getNodeOfSymbol(symb, root->right) : symbolLeft;
}

Node* AdaptiveHuffman::findNodeSameFreq(Node* crr, Node* root)
{
	Node* result = crr;

	if (root->freq > result->freq && !root->isLeaf)
	{
		Node* replaceLeft = findNodeSameFreq(result, root->left);
		if (replaceLeft)
			result = replaceLeft;
		Node* replaceRight = findNodeSameFreq(result, root->right);
		if (replaceRight)
			result = replaceRight;
	}
	else if (root->freq == result->freq && root->order > result->order)
		result = root;
	return (result != crr) ? result : nullptr;
}

int AdaptiveHuffman::swapNode(Node* node1, Node* node2)
{
	if (node1 == nullptr || node2 == nullptr)
		return -1;

	if (node1->parent == node2 || node2->parent == node1)
		return -1;

	int temp = node1->order;
	node1->order = node2->order;
	node2->order = temp;

	if (node1->parent->left == node1)
		node1->parent->left = node2;
	else
		node1->parent->right = node2;

	if (node2->parent->left == node2)
		node2->parent->left = node1;
	else
		node2->parent->right = node1;

	Node* tmp = node1->parent;
	node1->parent = node2->parent;
	node2->parent = tmp;

	return 1;
}

int AdaptiveHuffman::updateTree(Node* currentNode)
{
	if (!currentNode)
		return -1;
	Node* crr = currentNode;
	crr = findNodeSameFreq(crr, root);

	if (crr != currentNode)
		swapNode(crr, currentNode);

	currentNode->freq++;
	updateTree(currentNode->parent);

	return 1;
}

void AdaptiveHuffman::addSymbol(char symb)
{
	Node* nodeSymb = getNodeOfSymbol(symb, root);
	if (!nodeSymb)
	{
		NYT->isNYT = false;
		NYT->left = new Node(ILLEGAL, 0, NYT->order - 2, NYT, nullptr, nullptr, true, false);
		NYT->right = new Node(symb, 0, NYT->order - 1, NYT, nullptr, nullptr, false, true);
		nodeSymb = NYT->right;
		NYT = NYT->left;
	}

	updateTree(nodeSymb);
}

string AdaptiveHuffman::getPathtoSymbol(Node* crr, Node* root, string path)
{
	if (crr == root)
		return path;
	if (root == nullptr)
		return "";

	string leftPath = getPathtoSymbol(crr, root->left, path + "0");
	if (leftPath == "")
		return getPathtoSymbol(crr, root->right, path + "1");
	else
		return leftPath;
}

void AdaptiveHuffman::encode(ifstream& in, ofstream& out)
{
	// Testing
	char character;
	string result;
	while (in >> noskipws >> character)
	{
		Node* node = getNodeOfSymbol(character, root);
		if (node)
		{
			string path = getPathtoSymbol(node, root, "");
			result.append(path);
			addSymbol(character);
		}

		else
		{
			string path = getPathtoSymbol(NYT, root, "");
			bitset<8> bit(character);
			result.append(path);
			result.append(bit.to_string());
			addSymbol(character);
		}
	}

	bool not_full = false;
	int count_byte = result.length() / 8;
	if ((count_byte * 8) < result.length())
		not_full = true;
	for (int index = 0; index < count_byte; index++)
	{
		string tmp = "00000000";
		int i = 0;
		for (int index_2 = 0; index_2 < 8; index_2++)
		{
			tmp[i] = result[index * 8 + index_2];
			i++;
		}

		bitset<8> bit(tmp);
		int bit_int = bit.to_ulong();
		char bit_char = (char)(bit_int);
		out << bit_char;
	}

	if (not_full == true)
	{
		string tmp = "00000000";
		int i = 0;
		for (int index = 0; index < (result.length() - 8 * count_byte); index++)
		{
			tmp[8 + 8 * count_byte - result.length() + i] = result[count_byte * 8 + index];
			i++;
		}

		bitset<8> bit(tmp);
		int bit_int = bit.to_ulong();
		char bit_char = (char)(bit_int);
		out << bit_char;
	}
}

Node* AdaptiveHuffman::getNodebyPath(Node* &from, char path)
{
	if (path == '0')
	{
		from = from->left;
		return from;
	}
	else if (path == '1')
	{
		from = from->right;
		return from;
	}
	else
		return nullptr;
}

void AdaptiveHuffman::decode(ifstream& in, ofstream& out)
{
	// Status : Doing
	string bitOfAllSymbols;
	char symb;
	while (in.get(symb))
	{
		bitset<8> bit(symb);
		bitOfAllSymbols.append(bit.to_string());
	}

	cout << "length of bit decode :" << bitOfAllSymbols.length();

	string tmp = "00000000";
	int index;
	for (index = 0; index < 8; index++)
		tmp[index] = bitOfAllSymbols[index];

	bitset<8> bitOfOneSymbol(tmp);
	char symbol = (char)((int)(bitOfOneSymbol.to_ulong()));
	out << symbol;
	addSymbol((int)(symbol));
	Node* current = root;
	while (index < bitOfAllSymbols.length())
	{
		char path;
		path = bitOfAllSymbols[index++];
		Node* newCurr = getNodebyPath(current, path);

		if (newCurr->isLeaf && index<bitOfAllSymbols.length())
		{
			/*string temp = "00000000";
			for (int i = 0; i < 8; i++)
				temp[i] = bitOfAllSymbols[index++];

			bitset<8> ch(temp);
			char c = (char)(ch.to_ulong());*/
			out << (char)(newCurr->symbol);
			updateTree(newCurr);
			current = root;
		}
		else if (newCurr->isNYT && index < bitOfAllSymbols.length())
		{
			string temp = "00000000";
			for (int i = 0; i < 8; i++)
				temp[i] = bitOfAllSymbols[index++];

			bitset<8> ch(temp);
			char c = (char)((int)(ch.to_ulong()));
			out << c;
			addSymbol((int)(c));
			current = root;
		}

		if (index >= bitOfAllSymbols.length())
			break;
	}
}