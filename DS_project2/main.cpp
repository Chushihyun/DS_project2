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
    ans* nextans;
    int x;
    int y;
    ans(int a,int b){
        x=a;
        y=b;
    }
    
    void output(){
        cout<<row-y<<" "<<x-1<<endl;
    }
};

ans* ans_first=new ans(-1,-1);
ans* ans_tmp=ans_first;
int ans_num=0;

void output_ans(){
    ans* tmp=ans_first->nextans;
    cout<<ans_num<<endl;
    while (ans_num>0) {
        tmp->output();
        tmp=tmp->nextans;
        ans_num--;
    }
    ans_first->nextans->output();
    
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
    
    location(int a,int b,int c){
        x=a;
        y=b;
        this->type=c;
        distance_charge=-1;
        if (type==0) {
            finish=false;
        }
        else{
            finish=true;
        }
        distance_now=-1;
        index=-1;
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
location** F;

void check_matrix(){
    for (int m=row+1; m>=0; m--) {
        for (int n=0; n<column+2; n++) {
            cout<<C[n][m].type<<" ";
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

// all pairs shortest path, collect in B[num+1][num+1] is an adjencency matrix

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
    location* E=new location[num+3];
    int now=0;
    int amount=0;
    
    void push(location i){
        E[now+amount]=i;
        amount++;
    };
    location pop(){
        now++;
        amount--;
        return E[now-1];
    }
    void show_queue(){
        E[now].show();
        return;
    }
    queue(){}
};

int* D=new int[num+1];

//single source shortest path by DFS
queue q[1];
/*
void DFS(int m,int n){
    
    //queue* q=new queue;
    q[0].now=0;
    q[0].amount=0;
    q[0].push(C[m][n]);
    int distance=0;
    C[m][n].distance_charge=distance;
    int count;
    
    check_matrix();
    while (q[0].amount>0) {
        count=q[0].amount;
        distance++;
        cout<<endl<<"distance "<<distance<<":";
        for (int j=1; j<=count; j++) {
            location v=q[0].pop();
            if ((C[(v.x)+1][v.y].type!=1)&&(C[(v.x)+1][v.y].distance_charge==-1)) {
                C[v.x+1][v.y].distance_charge=distance;
                q[0].push(C[v.x+1][v.y]);
                cout<<"("<<v.x+1<<","<<v.y<<")";
            }
            if ((C[(v.x)-1][v.y].type!=1)&&(C[(v.x)-1][v.y].distance_charge==-1)) {
                C[v.x-1][v.y].distance_charge=distance;
                q[0].push(C[v.x-1][v.y]);
                cout<<"("<<v.x-1<<","<<v.y<<")";
            }
            if ((C[v.x][(v.y)+1].type!=1)&&(C[v.x][(v.y)+1].distance_charge==-1)) {
                C[v.x][v.y+1].distance_charge=distance;
                q[0].push(C[v.x][v.y+1]);
                cout<<"("<<v.x<<","<<v.y+1<<")";
            }
            if ((C[v.x][(v.y)-1].type!=1)&&(C[v.x][(v.y)-1].distance_charge==-1)) {
                C[v.x][v.y-1].distance_charge=distance;
                q[0].push(C[v.x][v.y-1]);
                cout<<"("<<v.x<<","<<v.y-1<<")";
            }
            //v旁邊還沒被加入的點push進去,amount++,distance=i
        }
        //cout<<endl<<"now="<<q[0].now<<", amount"<<q[0].amount<<endl;
    }
    //delete q;
    
}
 */
void BFS(int m,int n){
    q[0].now=0;
    q[0].amount=0;
    q[0].push(C[m][n]);
    int distance=0;
    C[m][n].distance_charge=distance;
    while (q[0].amount>0){
        location v=q[0].pop();
        if ((C[(v.x)+1][v.y].type!=1)&&(C[(v.x)+1][v.y].distance_charge==-1)) {
            C[v.x+1][v.y].distance_charge = C[v.x][v.y].distance_charge + 1;
            q[0].push(C[v.x+1][v.y]);
            //cout<<"("<<v.x+1<<","<<v.y<<")";
        }
        if ((C[(v.x)-1][v.y].type!=1)&&(C[(v.x)-1][v.y].distance_charge==-1)) {
            C[v.x-1][v.y].distance_charge = C[v.x][v.y].distance_charge + 1;
            q[0].push(C[v.x-1][v.y]);
            //cout<<"("<<v.x-1<<","<<v.y<<")";
        }
        if ((C[v.x][(v.y)+1].type!=1)&&(C[v.x][(v.y)+1].distance_charge==-1)) {
            C[v.x][v.y+1].distance_charge = C[v.x][v.y].distance_charge + 1;
            q[0].push(C[v.x][v.y+1]);
            //cout<<"("<<v.x<<","<<v.y+1<<")";
        }
        if ((C[v.x][(v.y)-1].type!=1)&&(C[v.x][(v.y)-1].distance_charge==-1)) {
            C[v.x][v.y-1].distance_charge = C[v.x][v.y].distance_charge + 1;
            q[0].push(C[v.x][v.y-1]);
            //cout<<"("<<v.x<<","<<v.y-1<<")";
        }
    }
}

void DFS_1(int i){
    // do DFS on C, 直接改C
    
    for (int y=row+1; y>=0; y--) {
        for (int x=0; x<column+2; x++) {
            /*
            C[x][y].x=x;
            C[x][y].y=y;
            //cout<<endl<<x<<y<<A[x][y]<<endl;
            C[x][y].type=A[x][y];
            C[x][y].distance_charge=-1;
            if (C[x][y].type==0) {
                C[x][y].finish=false;
            }
            else{
                C[x][y].finish=true;
            }
             */
            C[x][y]=F[x][y];
        }
    }
    //check_matrix();
    BFS(all[i].x,all[i].y);
    
    for (int m=0; m<=num; m++) {
        D[m]=C[all[m].x][all[m].y].distance_charge;
        //cout<<D[m]<<endl;
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
ans* tmp1;

void walk(int start,int end){
    //cout<<"start walk:"<<all[start].x<<" "<<all[start].y<<";"<<all[end].x<<" "<<all[end].y<<endl;
    next_is_charge=0;
    DFS_1(end);
    for (int i=0; i<=num; i++) {
        all[i].distance_now = D[i];
    }
    
    //find next,undone is better
    int next=tmp.index;
    bool check;
    for (int count= all[start].distance_now-1 ; count>=0; count--) {
        check=0;
        // put location into ans[], and count ans_num
        tmp1=new ans(tmp.x,tmp.y);
        ans_tmp->nextans=tmp1;
        ans_tmp=ans_tmp->nextans;
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
        energy--;
        tmp=all[next];
        //cout<<tmp.x<<" "<<tmp.y<<endl;
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
        
    }
}

void comeback(){
    //cout<<"start comeback"<<endl;
    walk(tmp.index, 0);
}

void go_near(){
    //cout<<"start go near"<<endl;
    while (is_any_undone()==1) {
        DFS_1(tmp.index);
        for (int i=0; i<=num; i++) {
            all[i].distance_now = all[i].distance_charge - D[i];
        }
        goal_index=0;
        for (int i=0; i<=num; i++) {
            if(all[i].finish==0){
                if (all[i].distance_now>all[goal_index].distance_now) {
                    if(energy>=D[i]+all[i].distance_charge){
                        goal_index=i;
                    }
                }
            }
        }
        if ((tmp.index==goal_index)||(goal_index==0)) {
            break;
        }
        walk(tmp.index,goal_index);
        if (next_is_charge==1) {
            break;
        }
    }
}

void go_farest(){
    //cout<<"start go farest"<<endl;
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

/*
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
*/

/////////////////// main

int main() {
    
    freopen("testcase2.data", "r", stdin);
    freopen("final.path", "w", stdout);
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
    undone=num;
    //check_matrix();
    //check_location();
    C =new location*[column+3];
    for (int i=0; i<column+3; i++) {
        C[i]=new location[row+3];
    }
    F =new location*[column+3];
    for (int i=0; i<column+3; i++) {
        F[i]=new location[row+3];
    }
    for (int y=row+1; y>=0; y--) {
        for (int x=0; x<column+2; x++) {
            F[x][y]=location(x,y,A[x][y]);
        }
    }
    
    if (num<=14500) {
        // B[][] store all pairs distance
        //calculate_distance();
        DFS_1(0);
        for (int i=0; i<=num; i++) {
            all[i].distance_charge=D[i];
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
            cout<<"done"<<endl;
        }
    }
    else{
        cout<<"num>1500"<<endl;
        
    }
    /*
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
     */
    return 0;
}
