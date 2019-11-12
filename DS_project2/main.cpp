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


#include <iostream>
#include <cstdlib>
using namespace std;


int row, column, energy_max;

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
        cout<<row-y<<" "<<x-1<<endl;
    }
};

ans* ans_first=new ans;
ans* ans_tmp=ans_first;
int ans_num=0;

void output_ans(){
    ans* tmp=ans_first->next;
    cout<<ans_num<<endl;
    while (ans_num>0) {
        tmp->output();
        tmp=tmp->next;
        ans_num--;
    }
    ans_first->next->output();
    
}

// location:每個位置
int energy;
int undone;

class location{
public:
    
    int x;
    int y;
    bool finish;// false = unfinish, true = finish
    int distance_charge;
    int distance_now;
    int index;  //index in all
    int type;   //0:可走 1:不可走 2:充電站
    
    location(){}
    location(int a,int b){
        x=a;
        y=b;
        finish=false;
    }
    
    location(int a,int b,int type){
        x=a;
        y=b;
        this->type=type;
        distance_charge=-1;
        if (type==0) {
            finish=false;
        }
        else{
            finish=true;
        }
    }
    void output(){
        cout<<"("<<x<<","<<y<<")"<<endl;
    }
    void show(){
        cout<<"x = "<<x<<" ,y = "<<y<<endl;
        cout<<"energy left = "<<energy<<", distance = "<<distance_charge<<", undone : "<<undone<<endl;
    }
};

//輸出整張地圖

int** A;
location** C;

void check_matrix(){
    for (int m=row+1; m>=0; m--) {
        for (int n=0; n<column+2; n++) {
            cout<<C[n][m].distance_charge<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}

// 輸出所有目標位置ㄝ，A[0]為充電站，A[1]~A[num]為路

location* all;
int num=0;

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



//queue
class queue{
public:
    location* D=new location[num+3];
    int now=0;
    int amount=0;
    
    void push(location i){
        D[now+amount]=i;
        amount++;
    };
    location pop(){
        now++;
        amount--;
        return D[now-1];
    }
    void show_queue(){
        D[now].show();
        return;
    }
};



//single source shortest path by DFS
void DFS(int x,int y){
    queue q;
    q.push(C[x][y]);
    int distance=0;
    C[x][y].distance_charge=distance;
    int count;
    while (q.amount>0) {
        count=q.amount;
        distance++;
        for (int j=1; j<=count; j++) {
            location v=q.pop();
            if ((C[v.x+1][v.y].type!=1)&&(C[v.x+1][v.y].distance_charge==-1)) {
                q.push(C[v.x+1][v.y]);
                C[v.x+1][v.y].distance_charge=distance;
            }
            if ((C[v.x-1][v.y].type!=1)&&(C[v.x-1][v.y].distance_charge==-1)) {
                q.push(C[v.x-1][v.y]);
                C[v.x-1][v.y].distance_charge=distance;
            }
            if ((C[v.x][v.y+1].type!=1)&&(C[v.x][v.y+1].distance_charge==-1)) {
                q.push(C[v.x][v.y+1]);
                C[v.x][v.y+1].distance_charge=distance;
            }
            if ((C[v.x][v.y-1].type!=1)&&(C[v.x][v.y-1].distance_charge==-1)) {
                q.push(C[v.x][v.y-1]);
                C[v.x][v.y-1].distance_charge=distance;
            }
            //v旁邊還沒被加入的點push進去,amount++,distance=i
        }
    }
}


location tmp;

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
bool next_is_charge;
bool during_near;

void walk(int start,int end){
    next_is_charge=0;                               // debug cout<<"start walk"<<endl;
    for (int i=0; i<=num; i++) {
        all[i].distance_now = B[end][i];
    }
    
    //find next,undone is better
    int next=tmp.index;
    bool check;
    for (int count= all[start].distance_now-1 ; count>=0; count--) {
        check=0;
        // put location into ans[], and count ans_num
        ans_tmp->next=new ans(tmp.x,tmp.y);
        ans_tmp=ans_tmp->next;
        ans_num++;                              //check tmp status // debug all[tmp.index].show(); cout<<" , direction : ("<<all[end].x<<","<<all[end].y<<")"<<endl;
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
        if (all[tmp.index].finish==0) {
            all[tmp.index].finish=1;
            undone--;
        }
        if (during_near==1) {
            if (next==0) {
                next_is_charge=1;
                break;
            }
        }
        
    }                                       // debug cout<<"end walk"<<endl;
}

void comeback(){                            // debug cout<<"start come back"<<endl;
    walk(tmp.index, 0);                     // debug cout<<"end come back"<<endl;
}

void go_near(){                                 // debug cout<<"start go near"<<endl;
    while (is_any_undone()==1) {
        for (int i=0; i<=num; i++) {
            all[i].distance_now = all[i].distance_charge - B[tmp.index][i];
        }
        goal_index=0;
        for (int i=0; i<=num; i++) {
            if(all[i].finish==0){                                        // debug cout<<"all[goal].distance_now =  "<<all[goal_index].distance_now<<endl;
                if (all[i].distance_now>all[goal_index].distance_now) {
                    if(energy>=B[tmp.index][i]+all[i].distance_charge){
                        goal_index=i;                                       // debug cout<<"change goal into "<<i<<endl;
                    }
                }
            }
        }                                                               // debug cout<<"tmp= "<<tmp.index<<" , goal= "<<goal_index<<endl;
        if ((tmp.index==goal_index)||(goal_index==0)) {
            break;
        }                                                           // debug cout<<"call walk"<<endl;
        walk(tmp.index,goal_index);
        if (next_is_charge==1) {
            break;
        }
    }                                                               // debug cout<<"end go near"<<endl;
}

void go_farest(){                                                   // debug cout<<"start go farest"<<endl;
    goal_index=0;
    for (int i=0; i<=num; i++) {
        if (all[i].distance_charge>all[goal_index].distance_charge) {
            if (all[i].finish==0) {
                goal_index=i;
            }
        }
    }
    walk(0,goal_index);                                                 // debug cout<<"end go farest"<<endl;
}


void big_walk(int start,int end){

    for (int i=0; i<=num; i++) {
        all[i].distance_now = B[end][i];
    }
    
    //find next,undone is better
    int next=tmp.index;
    bool check;
    for (int count= all[start].distance_now-1 ; count>=0; count--) {
        check=0;
        // put location into ans[], and count ans_num
        ans_tmp->next=new ans(tmp.x,tmp.y);
        ans_tmp=ans_tmp->next;
        ans_num++;
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
        tmp=all[next];
        if (all[tmp.index].finish==0) {
            all[tmp.index].finish=1;
            undone--;
        }
    }                                       // debug cout<<"end walk"<<endl;
}



void big_comeback(){
    big_walk(tmp.index, 0);
}

void big_go_farest(){
    goal_index=0;
    for (int i=0; i<=num; i++) {
        if (all[i].distance_charge>all[goal_index].distance_charge) {
            if (all[i].finish==0) {
                goal_index=i;
            }
        }
    }
    big_walk(0,goal_index);                                                 // debug cout<<"end go farest"<<endl;
}


/////////////////// main

int main() {
    
    freopen("testcase_big.data", "r", stdin);
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
        for (int y=1; y<=row; y++) {
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
    undone=num;                         //check_matrix();   //check_location();

    
    if (num<=1450) {
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
            during_near=1;
            go_near();
            during_near=0;
            comeback();
            energy=energy_max;
        }
        // End
        
        if (num==0){                            //不用動的case
            cout<<"0"<<endl;
            ans(all[0].x,all[0].y).output();
        }
        else{
            output_ans();
        }
    }
    else{                                    //if num>1450, calculate distance by DFS
        
    // do DFS on C, 直接改C
        C =new location*[column+2];
        for (int i=0; i<column+2; i++) {
            C[i]=new location[row+2];
        }
        for (int y=row+1; y>=0; y--) {
            for (int x=0; x<column+2; x++) {
                C[x][y]=location(x,y,A[x][y]);
            }
        }
        
        
        DFS(all[0].x,all[0].y);
        
        
        all=new location[num+1];
        int e=0;
        
        for (int x=1; x<=column; x++) {
            for (int y=1; y<=row; y++) {
                if (C[x][y].type==0) {
                    e++;
                    all[e]=C[x][y];
                    all[e].index=e;
                }
                else if (C[x][y].type==2){
                    all[0]=C[x][y];
                    all[0].finish=true;
                    all[0].index=0;
                }
            }
        }
        
    // let's go
        tmp=all[0];
        
        while(undone>0){
            big_go_farest();
            big_comeback();
        }
        // End
        
        output_ans();
        
    }
    return 0;
}
