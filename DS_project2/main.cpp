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


class ans{
public:
    ans* next;
    int x;
    int y;
    ans(int a,int b){
        x=a;
        y=b;
    }
    ans(){}
    void output(){
        cout<<x<<" "<<y<<endl;
    }
};

ans* ans_first=new ans;
ans* ans_tmp=ans_first;
int ans_num=1;

void output_ans(){
    ans* tmp=ans_first->next;
    cout<<ans_num<<endl;
    while (ans_num>1) {
        tmp->output();
        tmp=tmp->next;
        ans_num--;
    }
    ans_first->next->output();
    
}

// location:每個位置

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

//輸出整張地圖

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

// 輸出所有目標位置ㄝ，A[0]為充電站，A[1]~A[num]為路

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

// all pairs shortest path, collect in B[num+1][num+1]

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
//Floyd-Warshall algorithm
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

bool energy_left(location a){
    if (energy<a.distance_charge) {
        return false;
    }
    else{
        return true;
    }
}

bool is_any_undone(){
    if (undone>=1) {
        return true;
    }
    else{
        return false;
    }
}

int goal_index;

void walk(int start,int end){
    for (int i=0; i<=num; i++) {
        all[i].distance_now = B[end][i];
    }
    
    //find next,undone is better
    int next=tmp.index;
    bool check;
    for (int count= all[start].distance_now-1 ; count>=0; count--) {
        check=0;
        ans_tmp->next=new ans(tmp.x,tmp.y);
        ans_tmp=ans_tmp->next;
        ans_num++;
        if (all[tmp.index].finish==0) {
            all[tmp.index].finish=1;
            undone--;
        }
        //find next
        for (int i=0; i<=num; i++) {
            if (all[i].distance_now==count) {
                if ( (abs(all[i].x-tmp.x) + abs(all[i].y-tmp.y)) ==1 ) {
                    if (all[i].finish==0) {
                        check=1;
                        next=i;
                    }
                    else{
                        next=i;
                    }
                }
            }
            if (check==1)  break;
        }
        energy--;
        tmp=all[next];
    }
}

void comeback(){
    walk(tmp.index, 0);
}

void go_near(){
    for (int i=0; i<=num; i++) {
        all[i].distance_now = all[i].distance_charge - B[tmp.index][i];
    }
    
    while (is_any_undone()==1) {
        goal_index=tmp.index;
        for (int i=0; i<=num; i++) {
            if(all[i].finish==0){
                if (all[i].distance_now>all[goal_index].distance_now) {
                    if(energy>=B[tmp.index][goal_index]+all[goal_index].distance_charge){
                        goal_index=i;
                    }
                }
            }
        }
        if (tmp.index==goal_index) {
            break;
        }
        walk(tmp.index,goal_index);
    }
}

void go_farest(){
    goal_index=0;
    for (int i=0; i<=num; i++) {
        if (all[i].distance_charge>all[goal_index].distance_charge) {
            if (all[i].finish==0) {
                goal_index=i;
            }
        }
    }
    walk(0,goal_index);
}

/////////////////// main

int main() {
    
    freopen("testcase.data", "r", stdin);
    cin>>row>>column>>energy_max;
    char input;
    
    // initialize A=整張圖, A[x][y] = 一般座標位置
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
    // input finished  0:可走 1:不可走 2:充電站
    
    // collect all locations in  all[]
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
    
    // B[][] store all pairs distance
    calculate_distance();
    
    for (int i=0; i<=num; i++) {
        all[i].distance_charge=B[0][i];
    }
    
    // Start travesal
    tmp=all[0];
    energy=energy_max;
    
    while(undone>0){
        go_farest();
        go_near();
        comeback();
        energy=energy_max;
    }
    // End
    
    output_ans();
    
    return 0;
}
