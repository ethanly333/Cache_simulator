/* Lab 2 - Cache Simulator
Ethan Ly
Wes Fortner
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>

#define DIRECT_MAPPED   0
#define SET_ASSOC       1

using namespace std ;

struct cacheLine
{
    int validBit ;
    int tagBits ;
};//end struct line

void calcAndprintResults(int, int, int, int, int) ;

int main(int argc, const char *argv[])
{
    int blockSize, numBlocks ;
    int linesPerSet = 1 ;
    int associativity = 0 ;
    int tHit, tMiss ; 
    int numIdxBits, numOffsetBits ;
    int i = 0 ;

    cout << "Enter block size (in words): " ;
    cin >> blockSize ;

    cout << "Enter number of blocks in cache: " ;
    cin >> numBlocks ;

    cout << "Enter Associativity [0 - Direct Mapped] [1 - Set Associative]: " ;
    cin >> associativity ;

    if(associativity == SET_ASSOC)
    {
        cout << "How many lines per set?: " ;
        cin >> linesPerSet ;
    }//end if

    cout << "Enter hit time (in cycles): " ;
    cin >> tHit ;

    cout << "Enter miss time (in cycles): " ;
    cin >> tMiss ;

    //create array of blocks to build cache
    struct cacheLine cache[numBlocks][linesPerSet] ;

    if(associativity == DIRECT_MAPPED) 
    {
        //calculate bits required to represent blocks
        numIdxBits = ceil(log2(numBlocks)) ;    
        //calculate bits required to represent byte offset
        numOffsetBits = ceil(log2(blockSize)) ;

        //Initialize Cache
        while(i<numBlocks)
        {
            cache[i][0].validBit = 0 ;
            cache[i][0].tagBits = 0 ;
            i++ ;
        }//end while
    }//end if
    else if(associativity == SET_ASSOC)   
    {
        int numSets = numBlocks/linesPerSet ;

        //calculate bits required to represent blocks
        numIdxBits = ceil(log2(numSets)) ;    
        //calculate bits required to represent byte offset
        numOffsetBits = ceil(log2(blockSize)) ;

        //create array of blocks to build cache
        struct cacheLine cache[numSets][linesPerSet] ;

        //initialize Cache
        while(i<numSets)
        {
            for(int j=0; j<linesPerSet; j++)
            {
                cache[i][j].validBit = 0 ;
                cache[i][j].tagBits = 0 ;
            }//end for j
            i++ ;
        }//end while
    }//end else if 
    else
    {
        cerr << "ERROR: Associativity must be 0 or 1. Usage: [0 - Direct Mapped] [1 - Set Associative]" ;
        return 1 ;
    }
    
    //open file for reading
    ifstream fRead ;
    fRead.open("addresses.txt") ;

    //check if file is empty 
    if(fRead.peek() == EOF)
    {
        cerr << "ERROR: File is empty\n" ;
        return 1 ;
    }//end if

    int numHit = 0 ;
    int numMiss = 0 ;
    int numAddrRead = 0 ;
    int addr, index, tag ;

    if(fRead.is_open())
    {
        while(fRead >> hex >> addr)
        {
            //keep track of the addresses read from file
            numAddrRead++ ;
            bool shouldReplace = true ;
            bool didHit = false ;

            //Direct Mapped            
            if(associativity == DIRECT_MAPPED)
            {
                //find index from address
                index = (addr/blockSize) % numBlocks ;
                //find tag from address
                tag = addr >> (numIdxBits + numOffsetBits) ;

                //the validity of the data at this line must be checked along with the tag
                //cache hit is only true when validBit==1 && tags are equal
                if((cache[index][0].validBit==1) && (cache[index][0].tagBits == tag))
                {
                    numHit++ ;
                }//end if
                else if((cache[index][0].validBit==1) && (cache[index][0].tagBits != tag))
                {
                    numMiss++ ;
                    //replace
                    cache[index][0].tagBits = tag ;
                }//end if
                else if(cache[index][0].validBit == 0)
                {
                    numMiss++ ;
                    cache[index][0].tagBits = tag ;
                    cache[index][0].validBit = 1 ;
                }//end else if
            }//end if
        
            else if(associativity == SET_ASSOC)
            {
                srand(time(NULL)) ;
                //find index(set) from address
                index = (addr/blockSize) % (numBlocks/linesPerSet) ;
                //find tag from address
                tag = addr >> (numIdxBits + numOffsetBits) ;
                
                //must check each line within the set for validity and tag
                for(i=0; i<linesPerSet; i++)
                {
                    if(cache[index][i].validBit && (cache[index][i].tagBits==tag))
                    {
                        didHit = true ;
                        numHit++ ;
                    }//end if
                }//end for i

                if(!didHit)
                {
                    numMiss++ ;
                    for(i=0; i<linesPerSet; i++)
                    {
                        if(!cache[index][i].validBit)
                        {
                            cache[index][i].validBit = 1 ;
                            cache[index][i].tagBits = tag ;
                            shouldReplace = false ;
                            break ;
                        }//end if 
                    }//end for i   

                    //validBit == 1 but tags do not match
                    if(shouldReplace)
                    {
                        int randOffset = rand() % linesPerSet ;
                        cache[index][randOffset].validBit = 1 ;
                        cache[index][randOffset].tagBits = tag ;
                    }//end if
                }//end if
            }//end else
        }//end while
    }//end if

    calcAndprintResults(numMiss, numHit, numAddrRead, tMiss, tHit) ;

    return 0 ;
}//end main

void calcAndprintResults(int nMiss, int nHit, int nAddrRead, int missTime, int hitTime)
{
    double missRate, hitRate ;
    int amat ;

    //missRate = (nMiss/(double)nAddrRead) * 100 ;
    hitRate = (nHit/(double)nAddrRead) * 100 ;
    missRate = 100 - hitRate ;
    amat = ((missRate*missTime)/100) + hitTime ;

    cout << "Hit Rate(%): " << hitRate << '\n' ;
    cout << "Miss Rate(%): " << missRate << '\n' ;
    cout << "AMAT(cycles): " << amat << '\n' ;
}//end printResults




