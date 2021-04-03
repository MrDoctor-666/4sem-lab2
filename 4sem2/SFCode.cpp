#include "SFCode.h"
#include"Stack.h"

PriorityList<char>* SFCode::CountProbability(char* string, int* length) {
	PriorityList<char>* symb = new PriorityList<char>();
	//symbols
	int chars[256] = { 0 };
	int count = 0;
	for (int i = 0; (string[i] != '\n' && i < *length); i++)
	{
		chars[string[i]]++; count++;
	}
	for (int i = 0; i < 256; i++) {
		//if symbol exist in string then add it
		if (chars[i] != 0) symb->push((char)i, chars[i]);
	}
	*length = count;
	return symb;
}


void SFCode::buildTree(Node* current) {
	if (current->symbols->get_size() == 1) { current->ifLeaf = true; return; }
	PriorityList<char>* symb = new PriorityList<char>();
	symb = current->symbols->Copy();
	Node* left = new Node();
	Node* right = new Node();
	int probLeft, probRight;
	//split in half
	probLeft = symb->head_priority(); left->symbols->push(symb->pop_front(), probLeft);
	probRight = symb->tail_priority(); right->symbols->push(symb->pop_back(), probRight);
	//try to split in more or less same parts
	while (!symb->isEmpty()) {
		int probCur;
		if (probLeft > probRight) {
			//if left is bigger need to increase right
			probCur = symb->tail_priority();
			right->symbols->push(symb->pop_back(), probCur);
			probRight += probCur;
		}
		else {
			probCur = symb->head_priority();
			left->symbols->push(symb->pop_front(), probCur);
			probLeft += probCur;
		}
	}
	//making nodes
	left->allSymsbols = probLeft;
	right->allSymsbols = probRight;
	current->left = left;
	current->right = right;
	left->parent = current;
	right->parent = current;
	//call recursively for left and right
	buildTree(left);
	buildTree(right);
}

void SFCode::Code(Node* root) {
	if (root == nullptr){
		this->codes = nullptr;
		return;
	}
	Stack<Node*>* stack = new Stack<Node*>();
	stack->push(root);
	RedBlackTree<char, SymbCode*>* codes = new RedBlackTree<char, SymbCode*>();
	bool* curCode = new bool[root->allSymsbols / 2 + 1];
	Node* cur = root;
	int curDepth = 0;
	if (root->ifLeaf) { curDepth = 1; curCode[0] = 0; }
	//go in tree like in dft
	while (!stack->isEmpty()) {
		if (cur->left != NULL) {
			//go left
			curCode[curDepth] = 1;
			curDepth++;
			if (cur->right != NULL) stack->push(cur->right);
			cur = cur->left;
		}
		else if (cur->right != NULL) {
			//go right
			curCode[curDepth] = 0;
			curDepth++;
			cur = cur->right;
		}
		else {
			//write code
			bool* finalCode = new bool[curDepth];
			for (int i = 0; i < curDepth; i++) finalCode[i] = curCode[i];

			SymbCode* Code = new SymbCode(finalCode, curDepth);
			codes->insert((int)cur->symbols->get_front(), Code);

			//fck go back
			while (stack->get_top() != cur->right) {
				cur = cur->parent;
				curDepth--;
				if (cur == nullptr) break;
			}
			cur = stack->pop();
			curCode[curDepth] = 0;
			curDepth++;
		}
	}
	this->codes = codes;
}

bool* SFCode::CodeString() {
	if (root == nullptr) return NULL;
	if (codedString != NULL) return codedString; //if we did this already
	PriorityList<char>* symb = new PriorityList<char>();
	symb = root->symbols->Copy();
	
	int prior, codeLength = 0; char curSymb;
	//counting length of the final code
	while (!symb->isEmpty()) {
		prior = symb->head_priority();
		curSymb = symb->pop_front();
		SymbCode* curSymbCode = codes->find(curSymb);
		codeLength += prior*curSymbCode->length;
	}

	this->codedStringLength = codeLength;
	codedString = new bool[codeLength];

	int curi = 0, j = 0;
	//find code
	for (int j = 0; j < inputStringLength; j++) {
		curSymb = inputString[j];
		SymbCode* curSymbCode = codes->find(curSymb);
		for (int i = 0; i < curSymbCode->length; i++) {
			codedString[curi] = curSymbCode->code[i]; curi++;
		}
	}

	return codedString;
}

char* SFCode::DecodeString() {
	if (codedString == NULL) return NULL;
	int curLength = 0;
	Node* cur = root;
	//assume string is the same lenght as code
	//it can't be bigger
	char* string = new char[codedStringLength];
	for (int i = 0; curLength < codedStringLength && i < codedStringLength;) {
		while (!cur->ifLeaf) {
			if (codedString[i] == 1) { 
				//go left; 
				cur = cur->left;
			}
			else {
				//go right
				cur = cur->right;
			}
			i++;
		}
		string[curLength] = cur->symbols->get_front(); 
		curLength++;
		cur = root;
	}
	//if string is smaller than code
	for (int i = 0; i < curLength + 1; i++) cout << string[i];
	char* finalString = new char[curLength + 1];
	for (int i = 0; i < curLength + 1; i++) finalString[i] = string[i];
	return finalString;
}

void SFCode::CodesOut() {
	//codes->print();
	char* chars = codes->get_keys();
	SymbCode** Symbs = codes->get_values();
	for (int i = 0; i < codes->get_size(); i++)
	{
		cout << endl << chars[i] << " ";
		for (int j = 0; j < Symbs[i]->length; j++)
			cout <<	Symbs[i]->code[j];
	}
}