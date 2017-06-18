#include "definitions.h"
#include "events.h"

#include <iostream>
using namespace std;


// Functions for class LLNODE.  LLNODE is the list node for each element in the list.
// It contains a pointer to the actual data and a pointer to the next node in the list.

LLNODE::LLNODE(EVENT *e) {
	ev = e;
	next = NULL;
}

LLNODE::~LLNODE() {
    delete ev;
}

// Functions for class LIST.  LIST contains the pointer to the front of the list.

LIST::LIST() {
	front = NULL;
}

LIST::~LIST() {
	LLNODE *tmp, *tmpNext;
	
	tmp = front;
	while(tmp!=NULL) {
		tmpNext = tmp->next;
		delete tmp;
		tmp = tmpNext;
	}
}

void LIST::display() {
	LLNODE *tmp;  int k;
	
	tmp = front;   k = 1;
    cout << "Displaying Event list." << endl;
	while(tmp!=NULL) {
        cout << "Node " << k << ":" << endl;
		(tmp->ev)->display();
		tmp = tmp->next;
		k += 1;
	}
}

EVENT *LIST::getFirstEvent() {
	if(front==NULL)  return NULL;
	else             return front->ev;
}

void  LIST::removeFirstEvent() {
	LLNODE *tmp;
	
	if(front==NULL) return;		// If empty, do nothing.
	
	tmp = front;			// Else remember thee front node,
	front = front->next;		//    move ffront to the second node,
	delete tmp;			//    and then delete the old front node.
}

void LIST::insertEvent(EVENT *e) {
    LLNODE* left;
    LLNODE* right;
    LLNODE* newnode;
	int etime, tmptime;
	
	newnode = new LLNODE(e);      // Create the new node.
	etime = e->getProcessTime();
	
	// If the list is empty, then the new node just becomes the front 
	// of the list.
	if(front==NULL) {
		front = newnode;
		return;
	}
	
	// Otherwise, first find where the new node goes.
	left = NULL;
    right = front;
	while(right!=NULL) {
		tmptime = (right->ev)->getProcessTime();
		if(etime<tmptime) break;
		left = right;
		right = right->next;
	}
	
	// There are two cases.  If left is still NULL, the new node goes at the front
	// of the list.  Otherwise, the new node goes between left and right.  The latter
	// case includes the case when the new node goes at the end (right==NULL).
	if(left!=NULL) {
		// The new node goes between left and right, including goes at the
		// end (right==NULL).
		left->next = newnode;
		newnode->next = right;
	}
	else {
		// The new node goes at the front.
		newnode->next = front;
		front = newnode;
	}
}

void LIST::removeEventsNamed(string n){
    LLNODE* currnode = front;
    LLNODE* prevnode = NULL;
    while (currnode && currnode->ev->getDeviceName() != n){
        prevnode = currnode;
        currnode = currnode->next;
    }
    if (currnode){
        if (prevnode){
            prevnode->next = currnode->next;
            delete currnode;
        }
        else{
            front = currnode->next;
            delete currnode;
        }
    }
}

// Functions for class EVENT.  

EVENT::EVENT(string n, int v, int pt) {
    device = n;
    value = v;
	processTime = pt;
}

void EVENT::display() {

	cout << "EVENT: Device = " << device <<
    "\nDevice code: " << value << ". Scheduled to be process at : " << processTime << endl;
}

int EVENT::getProcessTime() {
	return processTime;
}

string EVENT::getDeviceName() {
    return device;
}

int EVENT::getValue() {
    return value;
}
