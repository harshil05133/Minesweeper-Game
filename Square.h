#include <vector>
#include <iostream>
using namespace std;

struct Square{

    int number;
    bool isMine;
    bool isRevealed;
    bool isFlagged;
    vector<float> coordinates;
    vector<Square*> neighbors;
    int neighborCount;

    Square(bool a, float b, float c, int d){
        number = d;
        isMine = a;
        isRevealed = false;
        isFlagged = false;
        coordinates.push_back(b);
        coordinates.push_back(c);
        neighborCount = 0;
    }

    void changeFlagging(){
        if(!isRevealed){
            isFlagged = !isFlagged;
        }
    }

    void reveal(){
        isRevealed = true;
        if(neighborCount == 0){
            for(int i = 0; i < neighbors.size(); i++){
                if(neighbors[i] != nullptr && !(neighbors[i]->isRevealed) && !(neighbors[i]->isFlagged)){
                    neighbors[i]->reveal();
                }
            }
        }
    }

    void setNeighborCount(){
        int sum = 0;
        for(int i = 0; i < neighbors.size(); i++){
            if(neighbors[i] != nullptr && neighbors[i]->isMine){
                sum++;
            }
        }
        neighborCount = sum;
    }



};
