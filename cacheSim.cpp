/* Lab 2 - Cache Simulator
Ethan Ly
Wes Fortner
*/

#include <iostream>

using namespace std ;

int main(int argc, const char *argv[])
{
    int blockSize, numBlocks ;
    //int associativity ;
    int tHit, tMiss ; 

    cout << "Enter block size (in words):\n" ;
    cin >> blockSize ;

    cout << "Enter number of blocks in cache:\n" ;
    cin >> numBlocks ;

    //Work on this if there is time
    //cout << "Enter Associativity [0 - Direct] [1 - Fully Associative]:\n" ;
    //cin >> associativity ;

    cout << "Enter hit time (in cycles):\n" ;
    cin >> tHit ;

    cout << "Enter miss time (in cycles):\n" ;
    cin >> tMiss ;

    
    

}//end main



