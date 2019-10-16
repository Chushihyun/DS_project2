//
//  main.cpp
//  DS_project2
//
//  Created by 朱世耘 on 2019/10/16.
//  Copyright © 2019 sy_chu. All rights reserved.
//

#include <iostream>
using namespace std;

int row, column, energy_max;
int** A;


void check_matrix(){
    for (int m=row+1; m>=0; m--) {
        for (int n=0; n<column+2; n++) {
            cout<<A[n][m]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}



int main() {
    
    freopen("testcase.data", "r", stdin);
    cin>>row>>column>>energy_max;
    char input;
    
    // A[x][y] = 一般座標位置
    A=new int*[column+2];
    for (int i=0; i<column+2; i++) {
        A[i]=new int[row+2];
    }
    for (int y=row+1; y>=0; y--) {
        for (int x=0; x<column+2; x++) {
            if ((y==0)||(y==row+1)||(x==0)||(x==column+1)){
                A[x][y]=1;
            }
            else{
                cin>>input;
                if(input=='1')  A[x][y]=1;
                else if (input=='0')    A[x][y]=0;
                else if (input=='R')    A[x][y]=2;
            }
        }
    }
    /////////////////// initial finished
    
    //find charge_point
    
    //collect all unfinish
    
    //calculate all relative distance
    
    //from the farest
    //do second weighted_farest
    //if no_energy then return charge_point
    
    check_matrix();
    
    return 0;
}
