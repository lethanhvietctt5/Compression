#include "AdaptiveHuffman.h"

AdaptiveHuffman::AdaptiveHuffman(const AdaptiveHuffman& copy)
{
	root = new Node(ILLEGAL, 0, 512, nullptr, nullptr, nullptr, true, false);
	NYT = root;
}

Node* AdaptiveHuffman::getNodeOfSymbol(int symb, Node* root)
{
	if (root == nullptr || root->symbol == symb)
		return nullptr;

	Node* symbolLeft = getNodeOfSymbol(symb, root->left);
	return symbolLeft == nullptr ? getNodeOfSymbol(symb, root->right) : symbolLeft;
}

int AdaptiveHuffman::findNodeSameFreq(Node*& crr, Node* root)
{
	if (!crr || root == this->root)
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
		NYT->right = new Node(symb, 1, NYT->order - 1, NYT, nullptr, nullptr, false, true);
		nodeSymb = NYT->right;
		NYT = NYT->left;
	}

	updateTree(nodeSymb);
}

void AdaptiveHuffman::encode(ifstream in, ofstream& out)
{
}

void AdaptiveHuffman::decode(ifstream in, ofstream& out)
{
}

