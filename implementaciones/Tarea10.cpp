#include <bits/stdc++.h>
using namespace std;


class Point {
private:
    int x;
    int y;
public:
    Point(int x, int y){
        this->x = x;
        this->y = y;
    }

    //-- getters & setters
    int getX(){ return this->x; }
    int getY(){ return this->y; }
    int setX(int x){ this->x = x; }
    int setY(int y){ this->y = y; }
};
typedef vector<Point*> vPoint;



int main(){
    //-- Declare variables
    int nConjunto1, nConjunto2;
    vPoint conjunto1, conjunto2;

    //-- Read from stdin
    int x, y;
    cin >> nConjunto1;
    conjunto1.resize(nConjunto1, NULL);
    for(int i=0; i<nConjunto1; i++){ 
        cin >> x; cin >> y; 
        conjunto1[i] = new Point(x, y); 
    }

    cin >> nConjunto2;
    conjunto2.resize(nConjunto2, NULL);
    for(int i=0; i<nConjunto2; i++){ 
        cin >> x; cin >> y; 
        conjunto2[i] = new Point(x, y); 
    }

    //-- Do some stuff
    
}