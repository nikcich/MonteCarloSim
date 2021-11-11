#include <iostream>
#include <string>
#include <random>
#include<bits/stdc++.h>
#include <iomanip>
#include <list>
#include <algorithm>

using namespace std;

int Clock(int wss, int data[]);

int main(int argc, char *argv[]){

    default_random_engine generator;
    normal_distribution<double> distribution(10.0, 2.0);

    int ClockResults[21] = { 0 }; // page faults per working set size

    for(int i = 0; i < 1000; i++ ){ // experiments loop

        int data[1000];

        for(int j = 0; j < 1000; j++ ){ //Trace loop
            data[j] = ( 10 * ((int) ( j / 100 )) ) + distribution(generator); // added my normal distribution generator
        }

        for(int wss = 4; wss<=20; wss++ ){
        
            ClockResults[wss] += Clock( wss, data );
        }
    }

    for(int wss=4; wss<=20; wss++ ){
        cout << "wss: " << setw(2) << left << wss <<  " | " << setw(10) << right << "|  Clock" << "   |" << endl;
        cout << right << setw(3) << " |" << setw(7) << right << ClockResults[wss] << "   |";
        cout << endl << "--------|-------------|---------|----------|" << endl;
    }

    return 0;
}


int find_element(vector<int> list, int num){ // returns index at which the element is located
    int i = 0;
    bool found = false;
    for(auto it = list.begin(); it != list.end(); it++,i++ )    {
        // found nth element..print and break.
        if(*it == num) {
            found = true;
            break;
        }
    }
    if(found)
        return i;
    else
        return -1;
}

int Clock(int wss, int data[]){
    //pages in memory
    int n = 1000;
    vector<int> s;
    vector<int> bits;

    //int num = find_element(s, 2);
    int page_faults = 0;
    int pointer = 0;

    for(int i = 0; i < n; i++){
        if(s.size() < wss && find_element(s, data[i]) == -1){ // Not full yet
            s.push_back(data[i]);
            bits.push_back(1);
            page_faults++;
        }else{ // Full
            bool inserted = false;
            int index = find_element(s, data[i]);

            if(index != -1){ // Already in memory
                bits[index] = 1;
            }else{ // not in memory
                while(!inserted){
                    pointer = pointer%wss;


                    int curr_bit = bits[pointer];

                    if(curr_bit == 1){  // Bit is 1
                        bits[pointer] = 0;
                    }else{              // Bit is 0
                        bits[pointer] = 1;
                        s[pointer] = data[i];
                        inserted = true;
                        page_faults++;
                    }


                    pointer++;
                }
            }
        }
    }

    return page_faults;
}