/*
File Name: main.cpp
Author: Nikolas Cichosz
nrc170001
Modification history: Nikolas Cichosz 11/14/2021

to compile:     g++ main.cpp -o main
to run:         ./main
*/

#include <iostream>
#include <string>
#include <random>
#include<bits/stdc++.h>
#include <iomanip>
#include <list>
#include <algorithm>

using namespace std;

int LRU(int wss, int data[]);   // Function Prototype
int FIFO(int wss, int data[]);  // Function Prototype
int Clock(int wss, int data[]); // Function Prototype

int main(int argc, char *argv[]){

    default_random_engine generator;                        // Random number generator
    normal_distribution<double> distribution(10.0, 2.0);    // Normal distribution with mean of 10 and standard deviation of 2

    //////////////////////////////////////////////////////////////////////////////

    int LRUResults[21] = { 0 };                             // page faults per working set size
    int FIFOResults[21] = { 0 };                            // page faults per working set size
    int ClockResults[21] = { 0 };                           // page faults per working set size

    for(int i = 0; i < 1000; i++ ){                         // experiments loop

        int data[1000];                                     // Page numbers array

        for(int j = 0; j < 1000; j++ ){                     //Trace loop

            // Generate a random number from a normal distribution
            // with a mean of ten and a standard deviation of two.
            // There are ten regions which have their own base address.

            data[j] = ( 10 * ((int) ( j / 100 )) ) + distribution(generator); // Generates page number
        }

        for(int wss = 4; wss<=20; wss++ ){
            // Determine and accumulate the number of page
            // faults for each algorithm base on the current
            // working set size and the current trace.

            LRUResults[wss] += LRU(wss, data);              // Adds LRU page fault results to the accumulator for that working set size
            FIFOResults [wss] += FIFO ( wss, data );        // Same as above but for FIFO
            ClockResults[wss] += Clock( wss, data );        // Same but for Clock
        }
    }

    for(int wss=4; wss<=20; wss++ ){
        // Labels for output
        cout << "wss: " << setw(2) << left << wss <<  " | " << setw(10) << right << "LRU" << setw(10) << right << "|   FIFO" << setw(10) << right << "|  Clock" << "   |" << endl;
        //output statistics in format
        cout << right << setw(10) << " | " << right << setw(10) << LRUResults[wss]/1000.0;
        cout << right << setw(3) << " |" << setw(7) << right << FIFOResults[wss]/1000.0;
        cout << right << setw(3) << " |" << setw(7) << right << ClockResults[wss]/1000.0 << "   |";
        cout << endl << "--------|-------------|---------|----------|" << endl;
    }

    return 0;
}

int LRU(int wss, int data[]){
    int n = 1000;                                                   // Number of page numbers
    unordered_set<int> s;                                           // Pages in memory
    unordered_map<int, int> indexes;                                // Least recently used
    int page_faults = 0;                                            // Initial page faults

    for (int i=0; i<n; i++){                                        // Loop through page nums
        if (s.size() < wss){                                        // If mem is not full
            if(s.find(data[i])==s.end()){                           // If its not already in memory
                s.insert(data[i]);                                  // Add to mem
                page_faults++;                                      // increment page fault
            }
            indexes[data[i]] = i;                                   // Store LRU index of page
        }else{                                                      // If memory is full
            if (s.find(data[i]) == s.end()){                        // If its not already in memory
                int lru = INT_MAX;                                  // LRU index
                int val;                                            // LRU Value
                for (auto it = s.begin(); it != s.end(); it++){     // Loop through memory
                    if (indexes[*it] < lru){                        // If its less used than current LRU index
                        lru = indexes[*it];                         // change LRU index
                        val = *it;                                  // change LRU value
                    }
                }
                s.erase(val);                                       // Get rid of the page from memory
                s.insert(data[i]);                                  // Add new page num
                page_faults++;                                      // Increment page fault
            }
            indexes[data[i]] = i;                                   // Update the current page index
        }
    }
    return page_faults;                                             // Return page fault number
}

int FIFO(int wss, int data[]){
    int n = 1000;                                       // Page numbers
    unordered_set<int> s;                               // Items in memory
    queue<int> indexes;                                 // Queue for fifo
  
    int page_faults = 0;                                // Initialize page faults
    for (int i=0; i<n; i++){                            // Loop through page numbers
        if (s.size() < wss){                            // If mem is not full
            if (s.find(data[i])==s.end()){              // If its not already in memory
                s.insert(data[i]);                      // Insert page into the mem
                page_faults++;                          // Increment page faults
                indexes.push(data[i]);                  // Add page to queue
            }
        }else{                                          // If mem is full then do FIFO
            if (s.find(data[i]) == s.end()){            // if its not already in mem
                int val = indexes.front();              // Get first page
                indexes.pop();                          // remove first item from queue
                s.erase(val);                           // remove val from the set of pages
                s.insert(data[i]);                      // Add to the set
                indexes.push(data[i]);                  // Add page to queue
                page_faults++;                          // Incrememnt page fault
            }
        }
    }
  
    return page_faults;                                 // return page faults
}

int find_element(vector<int> list, int num){        // returns index at which the element is located
    int i = 0;                                      // index
    bool found = false;                             // Found boolean starts at false
    for(auto it = list.begin(); it != list.end(); it++,i++ ){
        
        if(*it == num) {                            // If the pointer is the number we're looking for
            found = true;                           // Set found to true
            break;                                  // Break out of the loop
        }
    }

    if(found)                                       // If we found the number we want
        return i;                                   // return the index
    else                                            // Didn't find the number
        return -1;                                  // Return -1
}

int Clock(int wss, int data[]){
    int n = 1000;                                   // how many page numbers
    vector<int> s;                                  // vector for memory
    vector<int> bits;                               // Use bits for each memory location
    int page_faults = 0;                            // Number of page faults
    int pointer = 0;                                // Clock hand pointer location

    for(int i = 0; i < n; i++){                     // Loop through each page number given
        if(s.size() < wss && find_element(s, data[i]) == -1){ // Not full yet
            s.push_back(data[i]);                   // Put the page in memory
            bits.push_back(1);                      // Use bit set to 1
            page_faults++;                          // Incremement page fault
        }else{                                      // If mem is full
            bool inserted = false;                  // Inserted starts at
            int index = find_element(s, data[i]);   // Get element index via function find_element()

            if(index != -1){                        // If its already in memory
                bits[index] = 1;                    // Change the use bit for that mem location to 1
            }else{                                  // If its not in memory
                while(!inserted){                   // Until it has been inserted
                    pointer = pointer%wss;          // Make sure pointer is within possible index values and loop around
                    int curr_bit = bits[pointer];   // Current index bit value
                    if(curr_bit == 1){              // If Bit is 1
                        bits[pointer] = 0;          // Set to 0
                    }else{                          // If Bit is 0
                        bits[pointer] = 1;          // Set bit to 1
                        s[pointer] = data[i];       // Swap old page with new page
                        inserted = true;            // Inserted is true
                        page_faults++;              // Increment page fault
                    }
                    pointer++;                      // Increment pointer
                }
            }
        }
    }

    return page_faults;                              // Return page faults
}
