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
    int getX() const { return this->x; }
    int getY() const { return this->y; }
    int setX(int x){ this->x = x; }
    int setY(int y){ this->y = y; }

    //-- print overload
    friend std::ostream & operator<<(std::ostream &os, const Point &point){
        os << "(" << point.getX() << ", " << point.getY() << ")";
        return os;
    }

    //-- General functions
    float distance(Point* destiny) const {
        return sqrt( float((destiny->getX() - this->x)*(destiny->getX() - this->x)) + float((destiny->getY() - this->y)*(destiny->getY() - this->y)) );
    }
};
typedef vector<Point*> vPoint;
typedef vector<float> vFloat;
std::ostream & operator<<(std::ostream &os, const vFloat &vector){
    for(int i=0; i<vector.size(); i++) os << vector[i] << " ";
    return os;
}


vFloat nSquareSolution(vPoint A, vPoint B){
    vFloat distances, temp;
    int size = 0;
    float sum = 0.0f;
    for(int i=0; i<B.size(); i++){
        temp.clear();
        sum = 0.0f;
        for(int j=0; j<A.size(); j++) temp.push_back( B[i]->distance(A[j]) );
        sort(temp.begin(), temp.end());
        size = temp.size() <= 5 ? temp.size() : 5;
        for(int j=0; j<size; j++) sum += temp[j];
        distances.push_back(sum);
    }
    return distances;
}


vFloat nLogNSolution(vPoint A, vPoint B){
    //-- Implement this function
    vFloat distances;
    return distances;
}


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
    cout << nSquareSolution(conjunto1, conjunto2) << endl;
}