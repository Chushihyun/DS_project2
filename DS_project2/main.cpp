//
//  main.cpp
//  DS_project2
//
//  Created by 朱世耘 on 2019/10/16.
//  Copyright © 2019 sy_chu. All rights reserved.
//

#include <iostream>
#include <cstdlib>
using namespace std;

//// class:每個位置

class location{
    public:
    
    int x;
    int y;
    bool finish;// false = unfinish, true = finish
    int distance_charge;
    int distance_now;
    
    location(){}
    location(int a,int b){
        x=a;
        y=b;
        finish=false;
    }
    void output(){
        cout<<"("<<x<<","<<y<<")"<<endl;
    }
};

////輸出整張地圖

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

//// 輸出所有目標位置ㄝ，A[0]為充電站，A[1]~A[num]為路

location* all;
int num=0;

void check_location(){
    cout<<"x y done"<<endl;
    for (int i=0; i<=num; i++) {
        cout<<all[i].x<<" "<<all[i].y<<" "<<all[i].finish<<endl;
    }
    cout<<endl;
}

//// all pair shortest path, collect in B[num+1][num+1]

void calculate_distance(){
    int B[num+1][num+1];
    for (int i=0; i<=num; i++) {
        for (int j=0; j<=num; j++) {
            if (i==j) {
                B[i][j]=0;
            }
            else if ( (abs(all[i].x-all[j].x) + abs(all[i].y-all[j].y)) ==1 ){
                B[i][j]=1;
            }
            else{
                B[i][j]=2*(row+column);
            }
        }
    }
////Floyd-Warshall algorithm
    for (int k=0; k<=num; k++) {
        for (int i=0; i<=num; i++) {
            for (int j=0; j<=num; j++) {
                if (B[i][j]>B[i][k]+B[k][j]) {
                    B[i][j]=B[i][k]+B[k][j];
                }
            }
        }
    }
    
    
    for (int i=0; i<=num; i++) {
        for (int j=0; j<=num; j++) {
            cout<<B[i][j]<<" ";
        }
        cout<<endl;
    }
    
    
}

///// main
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
    /////////////////// input finished  0:可走 1:不可走 2:充電站
    
    all=new location[row*column+1];
    
    for (int x=1; x<=column; x++) {
        for (int y=1; y<row; y++) {
            if (A[x][y]==0) {
                num++;
                all[num]=location(x, y);
            }
            else if (A[x][y]==2){
                all[0]=location(x, y);
                all[0].finish=true;
            }
        }
    }
    
    check_location();

    //calculate all relative distance
    calculate_distance();
    
    //from the farest
    //do second weighted_farest
    //if no_energy then return charge_point
    
    check_matrix();
    
    return 0;
}
