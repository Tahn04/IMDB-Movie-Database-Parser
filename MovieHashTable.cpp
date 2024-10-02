#include<iostream>
#include <vector>
#include "MovieHashTable.hpp"
using namespace std;

/*
MovieHashTable.cpp
A hash table that stors all the movies in a MovieNode data type. It uses chaining to resolve collisions and generates a hash value with an Identikey.
*/

// MovieHashTable - Default constructor for MovieHashTable with default size
// 
// @param none
// @return none
MovieHashTable::MovieHashTable() {
    // TODO
    table_size = DEFAULT_HTABLE_CAPACITY;
    n_collisions = 0;
    table = new MovieNode*[table_size];
}

// MovieHashTable - Constructor for MovieHashTable with given size
// 
// @param none
// @return none
MovieHashTable::MovieHashTable(int s) {
    // TODO
    table_size = s;
    n_collisions = 0;
    table = new MovieNode*[table_size];
}

// ~MovieHashTable - Destructor for MovieHashTable that deletes all nodes in the hash table
// 
// @param none
// @return none
MovieHashTable::~MovieHashTable() {
    // TODO
    MovieNode* temp;
    MovieNode* temp_next;
    for(int i = 0; i < table_size; i++) { // Loops through every index in the table
        temp = table[i];
        while(temp != nullptr) { // Deletes every movie at each index
            temp_next = temp->next;
            delete temp;
            temp = temp_next;
        } 
        table[i] = NULL;
    } 
    delete table; // Frees the table
}

// hash - Hash function for MovieHashTable that returns an index in the hash table for a given movie title.
// 
// @param title
// @return MovieHashTable
int MovieHashTable::hash(string title) { // Sums the ascii values of the name and identikey
    // TODO
        string identikey = "taja6898";
        int hash_key = 0;
        for (int i = 0; i < identikey.length(); i++){ // Gets the ascii value of each char in identikey and sums them
            hash_key += int(identikey[i]);
        }
        int title_sum = 0;
        for (int i = 0; i < title.length(); i++){ // Gets the ascii value of each char in title and sums them
            title_sum += int(title[i]);
        }
        int index = (title_sum * hash_key)%table_size; // The index is the title_sum * hash_key % table_size
        return index;
}

// insert - Inserts a movie node into the hash table with the specified title
// 
// @param title, movie
// @return void
void MovieHashTable::insert(string title, MovieNode* movie) {
    // TODO
    int index = hash(title);

    if (search(title) == nullptr){
        if (table[index] == nullptr){ // If the index is empty
            table[index] = movie;
            return;
        }
        else{ // If there is a collision place the new movie at the old's next
            movie->next = table[index];
            table[index] = movie;
            setCollisions();
            return;
        }
    }
    else{
        delete movie; // If there is a movie with a duplicate name, delete it
    }
} 


// search - Searches for a node in the hash table with the specified title
// 
// @param title
// @return MovieNode
MovieNode* MovieHashTable::search(string title) {
    // TODO
    int index = hash(title); // Gets the hash value for the title
    MovieNode* current = this->table[index];
    if (current == nullptr){
        return nullptr;
    }
    while (current != nullptr){ // Searches the table at the index for the movie
        if (current->title == title){ // Return the desired movie if found
            return current;
        }
        else{
            current = current->next;
        }
    }
    return nullptr;
}

// Returns the number of collisions that have occurred during insertion into the hash table
// 
// @param none
// @return int
int MovieHashTable::getCollisions() {
    // TODO
    return n_collisions; // Returns the number of collisions 
}

// Increments the number of collisions that have occurred during insertion into the hash table
// 
// @param none
// @return void
void MovieHashTable::setCollisions() {
    // TODO
    n_collisions++; // Increment the number of collisions 
}
