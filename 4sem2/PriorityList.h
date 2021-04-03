#pragma once
#include <iostream>
using namespace std;

template <class T>
class PriorityList
{
private:
    class Node
    {
    public:
        Node(T content, int prior = 0, Node* before = nullptr, Node* next = nullptr)
        {
            this->content = content;
            this->priority = prior;
            this->before = before;
            this->next = next;
        };
        ~Node()
        {  }
        T content;
        int priority;
        Node* next;
        Node* before;
    private:

    };

    Node* head; //head list
    Node* tail;  //tail list
    size_t size;  //list size

public:
    PriorityList(Node* head = nullptr, Node* tail = nullptr) {
        this->head = head;
        this->tail = tail;
        size = 0;
    };
    void push(T, int); // add item to end
    size_t get_size(); // get list size
    T pop_front(); // delete first item
    T pop_back(); // delete last item
    T get_front();
    int head_priority();
    int tail_priority();
    void clear(); // clear list
    bool isEmpty(); // test for emptiness
    PriorityList<T>* Copy();
    ~PriorityList()
    {
        while (head->next != nullptr) //while we can go next
        {
            head = head->next;
            delete head->before;
        }
        size = 0;
        delete head;
    };
};

template <class T>
void PriorityList<T>::push(T add, int prior) // add item to end
{
	if (size == 0) //if list is empty
	{
		head = new Node(add, prior); //create head
		tail = head;
	}
	else
	{
		Node* newNod = new Node(add, prior);
		Node* cur = head;
		while ((cur->next != NULL) && (newNod->priority < cur->priority))
		{
			cur = cur->next;
		}
		if ((newNod->priority <= cur->priority) && (cur->next == NULL)) {
			newNod->before = cur;
			newNod->next = NULL;
			cur->next = newNod;
			tail = newNod;
		}
		else if ((newNod->priority >= cur->priority) && (cur->before == NULL))
		{
			newNod->before = NULL;
			newNod->next = cur;
			cur->before = newNod;
			head = newNod;
		}
		else {
			newNod->next = cur;
			newNod->before = cur->before;
			cur->before->next = newNod;
			cur->before = newNod;

		}
	}
	size++;
}

template <class T>
size_t PriorityList<T>::get_size() // get list size
{
	return size;
}

template <class T>
T PriorityList<T>::pop_front() // delete first item
{
	T cont;
	if (size == 1)
	{
		cont = head->content;
		size = 0;  //setup zero position
		head = NULL;
		tail = NULL;
	}
	else if (size > 1) //if list is not empty
	{
		cont = head->content;
		head = head->next; //set new head
		delete head->before;
		head->before = NULL;
		size--;
	}
	else //else return error
		throw out_of_range("Segmentation fault");
	return cont;
}

template <class T>
T PriorityList<T>::pop_back() // delete last item
{
	T cont;
	if (size == 1)
	{
		cont = tail->content;
		size = 0;  //setup zero position
		head = NULL;
		tail = NULL;
	}
	else if (size > 1) //if list is not empty
	{
		cont = tail->content;
		tail = tail->before;
		delete tail->next;
		tail->next = NULL;
		size--;
	}
	else //else return error
		throw out_of_range("Segmentation fault");
	return cont;
}

template <class T>
int PriorityList<T>::head_priority() {
	return head->priority;
}

template <class T>
int PriorityList<T>::tail_priority() {
	return tail->priority;
}

template <class T>
void PriorityList<T>::clear() // clear list
{
	if (head != NULL)
	{
		while (head->next != NULL)  //delete all item
		{
			head = head->next;
			delete head->before;
		}
		size = 0;  //setup zero position
		head = NULL;
		tail = NULL;
	}
}

template <class T>
bool PriorityList<T>::isEmpty() // test for emptiness
{
	if (head == NULL)
		return true;
	return false;
}

template <class T>
PriorityList<T>* PriorityList<T>::Copy() {
	PriorityList<T>* newQueue = new PriorityList<T>();
	Node* cur = head;
	while (cur != nullptr) {
		if (cur == NULL) break;
		newQueue->push(cur->content, cur->priority);
		cur = cur->next;
	}
	return newQueue;
}

template <class T>
T PriorityList<T>::get_front() {
	return head->content;
}