#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "MovieHashTable.hpp"
#include "DirectorSkipList.hpp"

using namespace std;

/*
A program that reads a csv file into a hash table of movies and a skip list of directors. There are 5 options which include:
find the director of a movie, find the number of movies by a director, find the description of a movie, list movies by a director, and exit.
*/

// Function prototypes
MovieNode* parseMovieLine(string line);
void readMovieCSV(string filename,  MovieHashTable &movieTable, DirectorSkipList &directorList);
void display_menu();


// Main function
int main(int argc, char* argv[]) {
    // TODO
    if (argc != 4){
        cout << "Invalid arguments." << endl;
        cout << "Usage: ./<program file> <csv file> <hashTable size> <skiplist size>" << endl; 
        return 0;
    }
    string file_ = argv[1];
    int hashT_size = atoi(argv[2]);
    int skipL_size = atoi(argv[3]);
    if (hashT_size < 0 || skipL_size < 0){
        cout << "Invalid arguments." << endl;
        cout << "Usage: ./<program file> <csv file> <hashTable size> <skiplist size>" << endl; 
        return 0;
    }
    // MovieHashTable* MHT = new MovieHashTable(); //default constructors
    // DirectorSkipList* DSL = new DirectorSkipList();
    MovieHashTable* MHT = new MovieHashTable(hashT_size);
    DirectorSkipList* DSL = new DirectorSkipList(skipL_size, 10);
    readMovieCSV(argv[1], *MHT, *DSL);
    int collitions = MHT->getCollisions();


    string menu_choice = "";
    DirectorSLNode* temp_direct;
    MovieNode* temp_movie;
    bool exit = false;
    while (exit == false){// Loops while exit is not called
        cout << "Number of Collisions: " << collitions << endl;
        display_menu();
        getline(cin, menu_choice);
        if (menu_choice.length() > 1 || menu_choice[0] < 49 || menu_choice[0] > 53){// Checks that the input is within the right parameters. 
            cout << "Invalid input." << endl; 
        }
        else {
            string movie_name = "";
            string direct_name = "";
            switch (stoi(menu_choice))
            {
            case 1: // Find the director of a movie
                cout << "Enter movie name: ";
                getline(cin, movie_name);
                temp_movie = MHT->search(movie_name); // Finds the movie with the given name
                if (temp_movie != nullptr){
                    cout << "The director of " << movie_name << " is " << temp_movie->director << endl;
                }
                else{
                    cout << "Invalid movie name." << endl;
                }
                break;
            case 2: // Find the number of movies by a director
                cout << "Enter director's name: ";
                getline(cin, direct_name);
                temp_direct = DSL->search(direct_name); // Finds the DirectorSLNode with the given name
                if (temp_direct != nullptr){
                    cout << direct_name << " directed " << temp_direct->movies.size() << " movies." << endl;
                }
                else{
                    cout << "Invalid director name." << endl;
                }
                break;
            case 3: // Find the description of a movie
                cout << "Enter movie name: ";
                getline(cin, movie_name);
                temp_movie = MHT->search(movie_name); // Finds the movie with the given name
                if (temp_movie != nullptr){
                    cout << "Summery: " << movie_name << " is a " << temp_movie->year << " (" << temp_movie->genre << ") film featuring \"" << temp_movie->actors << "\". " << endl;
                    cout << "Plot: \"" << temp_movie->description << "\"" << endl;
                }
                else{
                    cout << "Invalid movie name." << endl;
                }
                break;
            case 4: // List movies by a director
                cout << "Enter director name: ";
                getline(cin, direct_name);
                temp_direct = DSL->search(direct_name); // Finds the DirectorSLNode with the given name
                if (temp_direct != nullptr){
                    cout << direct_name << " directed the following movies:" << endl;
                    for (int i = 0; i < temp_direct->movies.size(); i++){
                        cout << i << ": " << temp_direct->movies.at(i)->title << endl;
                    }
                }
                else{
                    cout << "Invalid director name." << endl;
                }
                break;
            case 5: // Quit
                delete MHT;// Frees the memory 
                delete DSL; 
                cout << "Goodbye."<<endl;
                exit = true;
                break;
            
            default:
                cout << "Invalid input." << endl;
                break;
            }
        }    
    }
    return 0;
}

// parseMovieLine - Function to parse a CSV line into a MovieNode object
//
// @param line
// @return MovieNode
MovieNode* parseMovieLine(string line) {
    stringstream ss(line);
    vector<string> fields;
    string field;

    // Loop through the characters in the line
    bool in_quotes = false;
    for (size_t i = 0; i < line.length(); ++i) {
        char c = line[i];
        if (c == '\"') {
            in_quotes = !in_quotes;
        } else if (c == ',' && !in_quotes) {
            // add field to vector and reset for next field
            fields.push_back(field);
            field.clear();
        } else {
            field += c;
            // handle double quotes inside a quoted field
            if (in_quotes && c == '\"' && i < line.length() - 1 && line[i+1] == '\"') {
                field += '\"'; // add the second double quote and skip it
                ++i;
            }
        }
    }
    fields.push_back(field);

    if (fields.size() != 12) {
        cerr << "Error: Invalid movie line format" << line << endl;
        return nullptr;
    }

    int rank = stoi(fields[0]);
    string title = fields[1];
    string genre = fields[2];
    string description = fields[3];
    string director = fields[4];
    string actors = fields[5];
    int year = stoi(fields[6]);
    int runtime = stoi(fields[7]);
    float rating = stof(fields[8]);
    int votes = stoi(fields[9]);
    float revenue = stof(fields[10]);
    int metascore = stoi(fields[11]);

    // Create a new MovieNode object with the parsed fields
    MovieNode* movie = new MovieNode(rank, title, genre, description, director, actors, year, runtime, rating, votes, revenue, metascore);
    return movie;
}

// readMovieCSV - Function to read a CSV file into a vector of MovieNode objects
//
// @param filename, movieTable, directorList
// @return void
void readMovieCSV(string filename,  MovieHashTable &movieTable, DirectorSkipList &directorList) {
    // TODO
    srand(time(0)); // Used to generate random numbers for the skiplist
    ifstream movie_file; 
    movie_file.open(filename); // Opens file
    if (movie_file.is_open()){
        string movie;
        getline(movie_file, movie); 
        while (getline(movie_file, movie)){ // Uses the parseMovieLine() function to get a MovieNode from each line of the csv
            MovieNode* temp_movie;
            temp_movie = parseMovieLine(movie);
            movieTable.insert(temp_movie->title, temp_movie); // Inserts into the hash table 
            directorList.insert(temp_movie->director, temp_movie); // Inserts into the skiplist
        }
        //directorList.prettyPrint(); // testing
    }
    else{
        cout << "Could not open file!" << endl;
        return;
    }
}

// display_menu - Function to display the menu options
//
// @param collitions
// @return void
void display_menu() {
    // TODO
    cout << "Please select an option: " << endl;
    cout << "1. Find the director of a movie" << endl; 
    cout << "2. Find the number of movies by a director" << endl; 
    cout << "3. Find the description of a movie" << endl; 
    cout << "4. List movies by a director" << endl; 
    cout << "5. Quit" << endl;
    cout << endl;
    cout << "Enter an Option: ";
}
