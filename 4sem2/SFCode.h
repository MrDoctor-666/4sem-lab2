#pragma once
#include "RBtree.h"
#include "PriorityList.h"

class SFCode
{
private:
	class Node {
	public:
		PriorityList<char>* symbols;
		int allSymsbols;
		bool ifLeaf;
		Node* left;
		Node* right;
		Node* parent;

		Node() { 
			this->left = nullptr; this->right = nullptr; this->parent = nullptr; 
			this->symbols = new PriorityList<char>();
			this->allSymsbols = 0;
			this->ifLeaf = false;
		}
	};
	class SymbCode {
	public: 
		bool* code;
		int length;

		SymbCode(bool* code = nullptr, int length = 0) { this->code = code; this->length = length; }
	};
	
	Node* root;
	char* inputString;
	int inputStringLength;
	RedBlackTree<char, SymbCode*>* codes; //key - symbol, value - code
	bool* codedString;
	int codedStringLength;

	PriorityList<char>* CountProbability(char *, int*);
	void buildTree(Node*); //build codes tree
	void Code(Node*); //connect letters and codes

public:
	SFCode(char* string, int string_length) {
		this->codedString = NULL;
		this->codedStringLength = 0;
		this->inputString = string;
		this->inputStringLength = string_length;
		if (string_length == 0) { root = nullptr;}
		else {
			root = new Node();
			root->symbols = CountProbability(string, &string_length);
			root->allSymsbols = string_length;
			if (root->symbols->get_size() == 1) root->ifLeaf = true;
			else { root->ifLeaf = false; buildTree(root); }
		}
		Code(root);
	}

	bool* CodeString(); //get coded string
	int GetCodeLength() { return codedStringLength; } //for test purposes
	char* DecodeString(); //get decoded string
	void CodesOut(); //write codes in console
	double Coeff() { return (double)sizeof(char) * inputStringLength * 8 / codedStringLength; } //how compressed it is

	~SFCode() {};
};