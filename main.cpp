#include <iostream>
#include <string>
#include <random>
#include<bits/stdc++.h>
#include <iomanip>
#include <list>

#include <algorithm>

using namespace std;

int LRU(int wss, int data[]);
int FIFO(int wss, int data[]);
int Clock(int wss, int data[]);

int main(int argc, char *argv[]){

    default_random_engine generator;
    normal_distribution<double> distribution(10.0, 2.0);

    /////////////////////////////////////////////////////////////////////////////////////

    int LRUResults[21] = { 0 }; // page faults per working set size
    int FIFOResults[21] = { 0 }; // page faults per working set size
    int ClockResults[21] = { 0 }; // page faults per working set size

    for(int i = 0; i < 1000; i++ ){ // experiments loop

        int data[1000];

        for(int j = 0; j < 1000; j++ ){ //Trace loop
            // Generate a random number from a normal distribution
            // with a mean of ten and a standard deviation of two.
            // There are ten regions which have their own base address.
            data[j] = ( 10 * ((int) ( j / 100 )) ) + distribution(generator); // added my normal distribution generator
        }

        for(int wss = 4; wss<=20; wss++ ){
            // Determine and accumulate the number of page
            // faults for each algorithm base on the current
            // working set size and the current trace.

            LRUResults[wss] += LRU(wss, data);
            FIFOResults [wss] += FIFO ( wss, data );
            ClockResults[wss] += Clock( wss, data );
        }
    }

    for(int wss=4; wss<=20; wss++ ){
        cout << "wss: " << setw(2) << left << wss <<  " | " << setw(10) << right << "LRU" << setw(10) << right << "|   FIFO" << setw(10) << right << "|  Clock" << "   |" << endl;
        //output statistics
        cout << right << setw(10) << " | " << right << setw(10) << LRUResults[wss];
        cout << right << setw(3) << " |" << setw(7) << right << FIFOResults[wss];
        cout << right << setw(3) << " |" << setw(7) << right << ClockResults[wss] << "   |";
        cout << endl << "--------|-------------|---------|----------|" << endl;
    }

    return 0;
}

int LRU(int wss, int data[]){
    int n = 1000;

    // To represent set of current pages. We use
    // an unordered_set so that we quickly check
    // if a page is present in set or not

    // Set of pages in memory
    unordered_set<int> s;
 
    // To store least recently used indexes
    // of pages.

    // Least recently used indexes
    unordered_map<int, int> indexes;
 
    // Start from initial page
    int page_faults = 0;

    for (int i=0; i<n; i++){
        // Check if the set can hold more pages
        if (s.size() < wss){
            // Insert it into set if not present
            // already which represents page fault
            if(s.find(data[i])==s.end()){
                s.insert(data[i]);
                // increment page fault
                page_faults++;
            }
 
            // Store the recently used index of
            // each page
            indexes[data[i]] = i;
        }
 
        // If the set is full then need to perform lru
        // i.e. remove the least recently used page
        // and insert the current page
        else{
            // Check if current page is not already
            // present in the set
            if (s.find(data[i]) == s.end()){
                // Find the least recently used pages
                // that is present in the set
                int lru = INT_MAX;
                int val;
                for (auto it = s.begin(); it != s.end(); it++){
                    if (indexes[*it] < lru){
                        lru = indexes[*it];
                        val = *it;
                    }
                }
 
                // Remove the indexes page
                s.erase(val);
 
                // insert the current page
                s.insert(data[i]);
 
                // Increment page faults
                page_faults++;
            }
 
            // Update the current page index
            indexes[data[i]] = i;
        }
    }
 
    return page_faults;
}

int FIFO(int wss, int data[]){

    //int n = sizeof(data)/sizeof(data[0]);
    int n = 1000; // i think?

    // data = pages[]
    // n = n
    // wss = capacity


    // To represent set of current pages. We use
    // an unordered_set so that we quickly check
    // if a page is present in set or not
    unordered_set<int> s;
  
    // To store the pages in FIFO manner
    queue<int> indexes;
  
    // Start from initial page
    int page_faults = 0;
    for (int i=0; i<n; i++){
        // Check if the set can hold more pages
        if (s.size() < wss){
            // Insert it into set if not present
            // already which represents page fault
            if (s.find(data[i])==s.end()){
                // Insert the current page into the set
                s.insert(data[i]);
  
                // increment page fault
                page_faults++;
  
                // Push the current page into the queue
                indexes.push(data[i]);
            }
        }
  
        // If the set is full then need to perform FIFO
        // i.e. remove the first page of the queue from
        // set and queue both and insert the current page
        else{
            // Check if current page is not already
            // present in the set
            if (s.find(data[i]) == s.end()){
                // Store the first page in the 
                // queue to be used to find and
                // erase the page from the set
                int val = indexes.front();
                  
                // Pop the first page from the queue
                indexes.pop();
  
                // Remove the indexes page from the set
                s.erase(val);
  
                // insert the current page in the set
                s.insert(data[i]);
  
                // push the current page into
                // the queue
                indexes.push(data[i]);
  
                // Increment page faults
                page_faults++;
            }
        }
    }
  
    return page_faults;
}

int Clock(int wss, int data[]){
    //pages in memory
    list<int> s;
    list<int> bits;
    int n = 1000;
    unordered_set<int> setList;

    // Start from initial page
    int page_faults = 0;
    int pointer = 0;

    for (int i=0; i<n; i++){
        if (s.size() < wss){ // if its not full
            s.push_back(data[i]); // add to mem

            setList.insert(data[i]); // adds to set

            bits.push_back(1); // set use bit to 1
            page_faults++; // incrememnt page faults
        }
        
        else{ // if its full
            bool inserted = false;
            if(setList.find(data[i]) == setList.end()){ // not in mem
                while(!inserted){
                    pointer = pointer%wss;
                    list<int>::iterator current = bits.begin(); // bits pointer
                    advance(current, pointer);

                    if(*current == 1){ // If its use bit is a 1
                        bits.remove(pointer); // gets rid of the 1 bit
                        if(pointer < bits.size()-1){ // if its not the last element
                            list<int>::iterator new_curr = bits.begin(); // bits pointer
                            advance(new_curr, pointer);
                            
                            bits.insert(new_curr, 0); // replaces it with a 0
                        }else{
                            bits.push_back(0); // replaces with 0
                        }
                    }else{ // use bit is a zero same as above but replace with a 1 thijs time.
                        inserted = true;
                        bits.remove(pointer); // gets rid of the 0 bit
                        if(pointer < bits.size()-1){ // if its not the last element
                            list<int>::iterator new_curr = bits.begin(); // bits pointer
                            advance(new_curr, pointer);
                            
                            bits.insert(new_curr, 1); // replaces it with a 1
                        }else{
                            bits.push_back(1); // replaces with 1
                        }

                        list<int>::iterator currList = s.begin();

                        s.remove(pointer);

                        if(pointer < s.size()-1){
                            list<int>::iterator new_curr = s.begin(); // list pointer
                            advance(new_curr, pointer);
                            s.insert(new_curr, data[i]); // replaces it with a 1
                        }else{
                            s.push_back(data[i]);
                        }
                    }
                    pointer++;
                }
                setList.insert(data[i]);
            }else{ // already in mem
                std::list<int>::iterator it;
                int index = 0;
                for (it = s.begin(); it != s.end(); ++it){
                    if(*it == data[i]){
                        break;
                    }
                    index++;
                }

                list<int>::iterator current = bits.begin(); // bits pointer
                advance(current, index);

                bits.remove(index); // gets rid of the 1 bit
                if(index < bits.size()-1){ // if its not the last element
                    list<int>::iterator new_curr = bits.begin(); // bits pointer
                    advance(new_curr, index);    
                    bits.insert(new_curr, 0); // replaces it with a 0
                }else{
                    bits.push_back(1);
                }

            } 
            
        }
    }

    return page_faults;
}