//
//  main.cpp
//  DS_project2
//
//  Created by 朱世耘 on 2019/10/16.
//  Copyright © 2019 sy_chu. All rights reserved.
//
// location:紀錄每個位置內容的type
// A:整張地圖
// input: row,column,energy_max
// all:存所有location的array    all[0]為充電站
// num:要走的location數量
// undone:尚未走過的點
// energy:當下剩餘電力
// B:store all pairs distance

// question:充電完出來位置？充電站需不需要走

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
    int index;
    
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
int undone;

void check_location(){
    cout<<"x y done"<<endl;
    for (int i=0; i<=num; i++) {
        cout<<all[i].x<<" "<<all[i].y<<" "<<all[i].finish<<endl;
    }
    cout<<endl;
}

//// all pair shortest path, collect in B[num+1][num+1]

int** B;
void calculate_distance(){
    B=new int*[num+1];
    for (int i=0; i<num+1; i++) {
        B[i]=new int[num+1];
    }
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
// print B
/*
    for (int i=0; i<=num; i++) {
        for (int j=0; j<=num; j++) {
            cout<<B[i][j]<<" ";
        }
        cout<<endl;
    }
*/
}

location tmp;
int energy;

bool energy_left(){
    if (energy==tmp.distance_charge) {
        return false;
    }
    else{
        return true;
    }
}

int goal_index;

void walk(int start,int end){
    for (int i=0; i<=num; i++) {
        all[i].distance_now = B[end][i];
    }
    
    //keep find next,undone is better       if undone then go else 保留再看看
    while (tmp.index!=end) {
        <#statements#>
    }
    
    
    //if energy_left==0 then break
    
    
}

void go_near(){
    for (int i=0; i<=num; i++) {
        all[i].distance_now = all[i].distance_now - B[tmp.index][i];
    }
    
    goal_index=0;
    for (int i=0; i<=num; i++) {
        if (all[i].distance_now>all[goal_index].distance_now) {
            goal_index=i;
        }
    }
    walk(tmp.index,goal_index);
    
}

void go_farest(){
    goal_index=0;
    for (int i=0; i<=num; i++) {
        if (all[i].distance_charge>all[goal_index].distance_charge) {
            goal_index=i;
        }
    }
    walk(0,goal_index);
    
}
void comeback(){
    walk(tmp.index, 0);
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
                all[num].index=num;
            }
            else if (A[x][y]==2){
                all[0]=location(x, y);
                all[0].finish=true;
                all[0].index=0;
            }
        }
    }
    undone=num;

    calculate_distance();
    // B[][] store all pairs distance
    for (int i=0; i<=num; i++) {
        all[i].distance_charge=B[0][i];
    }
    
    tmp=all[0];
    while(undone>0){
        //go_to_farest();
        //come_back();
        
        
        
        
    }
    //from the farest
    
    //do second weighted_farest
    //if no_energy then return charge_point
    
    return 0;
}
