#include<iostream>
#include <vector>
#include "DirectorSkipList.hpp"
using namespace std;

/*
DirectorSkipList.cpp
A skip list that stors all the director information in the form of DirectorSLNode. 
*/

// DirectorSkipList - Default constructor for DirectorSkipList with default capacity and levels
//
// @param none
// @return none
DirectorSkipList::DirectorSkipList() {
    // TODO
    capacity = DEFAULT_CAPACITY;           // the default capacity of the skip list
    levels = DEFAULT_LEVELS;             // the default number of levels in the skip list
    size = 0;  
}

// DirectorSkipList - Constructor for DirectorSkipList with given capacity and levels
//
// @param none
// @return none
DirectorSkipList::DirectorSkipList(int _cap, int _levels) {
    // TODO
    capacity = _cap;
    levels = _levels;
    size = 0;  
}

// ~DirectorSkipList - Destructor for DirectorSkipList that deletes all nodes in the skip list (but not the MovieNodes, which are shared with other data structures)
//
// @param none
// @return none
DirectorSkipList::~DirectorSkipList() {
    DirectorSLNode* curr = head->next[0]; // Sets curr to the first director in the list
    DirectorSLNode* next;
    while (curr != nullptr){ // Loops through deletes all directors 
        next = curr->next[0];
        delete curr;
        curr = next;
    }
    delete head; // Frees the head
}

// insert - Inserts a movie node into the skip list with the specified director
//
// @param director, _movie
// @return void
void DirectorSkipList::insert(string director, MovieNode* _movie) {
    // TODO
    if (head == nullptr){ //Calls the parameterized DirectorSLNode constructor when the first director is added
        head = new DirectorSLNode("", levels);
    }
    DirectorSLNode* search_direct = search(director); //Need to check if director already exists
    if (search_direct != nullptr){
        search_direct->addMovie(_movie); // If the director is already in the skiplist, add the movie
        return;
    }
	int heads_tails = rand()%2; // Randomly generates 1 or 0
    int lvl = 1;
    while (lvl < levels && heads_tails == 1){// Counts the number of 1s in a row to be the level of the director
        lvl++;
        heads_tails = rand()%2;
    }
    DirectorSLNode* current = head;
    DirectorSLNode* prev[levels];
    for (int i = levels - 1; i >= 0; i--){ // Loops through the levels of the skiplist much like the search 
        while (current->next[i] != nullptr && current->next[i]->director < director){ // Checks until the next is null or the next director is less
            current = current->next[i];
        }
        prev[i] = current; // Records the previous node to know what will point to the new director
    }
    current = current->next[0]; // current should be the spot before want to insert so iterate to the next
    if (current == nullptr || current->director != director){ 
        DirectorSLNode* new_direc = new DirectorSLNode(director, lvl); // Creates a new DirectorSLNode
        new_direc->addMovie(_movie);
        for(int i = 0; i < lvl; i++){ // Inserts the new DirectorSLNode at every level using the prev
            new_direc->next[i] = prev[i]->next[i];
            prev[i]->next[i] = new_direc;
        }
        size++; // Keeps track of the number of elements 
    }
}

// search - Searches for a node in the skip list with the specified director
//
// @param director
// @return DirectorSLNode
DirectorSLNode *DirectorSkipList::search(string director) {
    // TODO
    DirectorSLNode* current = head;
    for (int i = levels - 1; i >= 0; i--){ // Loops throught the levels
        while (current->next[i] != nullptr && current->next[i]->director < director){ // Checks until the next is null or the next director is less
            current = current->next[i];
        }
    }
    current = current->next[0]; // current should be the spot before want to insert so iterate to the next

    if (current != nullptr && current->director == director){ // Checks that current is the desired node and returns it if is
        return current;
    }
    return nullptr;
}

// prettyPrint - Pretty-prints the skip list by Looping through the levels and printing each director on it
//
// @param none
// @return void
void DirectorSkipList::prettyPrint() {
    cout << "-----Director Skip List-----" << endl;
    cout << size << " directors." << endl;
    for(int i = levels; i >= 0; i--){ // Loops through the levels
        cout <<  i << ". ";
        DirectorSLNode* _director = head;
        while(_director != nullptr){
            cout << _director->director; // Prints the direcor
            _director = _director->next[i];
            cout << " -> ";
        }
        cout << "NULL" << endl;;
    }
}
