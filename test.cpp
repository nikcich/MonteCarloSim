#include <iostream>
#include <string>
#include <random>
#include<bits/stdc++.h>
#include <iomanip>
#include <list>

using namespace std;

int main(int argc, char *argv[]){

    list<int> s;

    s.push_back(1);
    s.push_back(2);
    s.push_back(3);
    s.push_back(5);
    s.push_back(4);
    
    s.remove(5);

    list<int>::iterator it2 = s.begin();
    advance(it2, 3);
    s.insert(it2, 99);

    list <int> :: iterator it;
    for(it = s.begin(); it != s.end(); ++it)
        cout << '\t' << *it;
    cout << '\n';

    

    return 0;
}