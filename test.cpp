#include<iostream>
#include <vector>
using namespace std;
int main()
{
    int levels = 10;
	srand((unsigned) time(NULL)); // Providing a seed value
	int heads_tails = rand()%2; // Get a random number
    int lvl = 0;
    while (lvl < levels && heads_tails == 1){
        lvl++;
        heads_tails = rand()%2;
    }

    cout << lvl << endl;
	return 0;
}