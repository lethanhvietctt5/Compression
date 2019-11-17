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

Node* AdaptiveHuffman::getNodeOfSymbol(int symb, Node* root)
{
	if (root == nullptr || root->symbol == symb)
		return root;

	Node* symbolLeft = getNodeOfSymbol(symb, root->left);
	return symbolLeft == nullptr ? getNodeOfSymbol(symb, root->right) : symbolLeft;
}

int AdaptiveHuffman::findNodeSameFreq(Node*& crr, Node* root)
{
	if (!root || crr == this->root)
		return 0;

	if (crr->freq == root->freq && crr->parent != root && crr->order < root->order)
		crr = root;

	findNodeSameFreq(crr, root->left);
	findNodeSameFreq(crr, root->right);
	return 1;
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
	findNodeSameFreq(crr, root);

	if (crr != currentNode)
		swapNode(crr, currentNode);

	currentNode->freq++;
	updateTree(currentNode->parent);

	return 1;
}

void AdaptiveHuffman::addSymbol(int symb)
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
	while (in >> character)
	{
		int sy = character;
		Node* node = getNodeOfSymbol(sy, root);
		if (node)
		{
			string path = getPathtoSymbol(node, root, "");
			out << path << " ";
			addSymbol(sy);
		}

		else
		{
			string path = getPathtoSymbol(NYT, root, "");
			bitset<8> bit(sy);
			out << path << " " << bit << " ";
			addSymbol(sy);
		}
	}
}

void AdaptiveHuffman::decode(ifstream& in, ofstream& out)
{
}

