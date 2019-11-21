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


#include <iostream>
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
    ans(){}
    void output(){
        cout<<row-y<<" "<<x-1<<endl;
    }
};

ans* ans_first=new ans;
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
int** index_chart;
location* all;

void check_matrix(){
    for (int m=row; m>=1; m--) {
        for (int n=1; n<column+1; n++) {
            cout<<all[index_chart[n][m]].distance_now<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}

// 輸出所有目標位置ㄝ，A[0]為充電站，A[1]~A[num]為路

int num=0;

void check_location(){
    cout<<"x y done"<<endl;
    for (int i=0; i<=num; i++) {
        cout<<all[i].x<<" "<<all[i].y<<" "<<all[i].finish<<endl;
    }
    cout<<endl;
}

//queue
class queue{
public:
    location* F=new location[num+3];
    int now=0;
    int amount=0;
    
    void push(location i){
        F[now+amount]=i;
        amount++;
    };
    location pop(){
        now++;
        amount--;
        return F[now-1];
    }
    queue(){
        now=0;
        amount=0;
    }
    ~queue(){
        delete[] F;
    }
};

// all pairs shortest path, collect in B[num+1][num+1]

int** B;

void BFS(location** D,int start){
    queue q;
    D[all[start].x][all[start].y].distance_charge=0;
    q.push(D[all[start].x][all[start].y]);
    while (q.amount>0){
        location v=q.pop();
        if ((D[(v.x)+1][v.y].type!=1)&&(D[(v.x)+1][v.y].distance_charge==-1)) {
            D[v.x+1][v.y].distance_charge = D[v.x][v.y].distance_charge + 1;
            q.push(C[v.x+1][v.y]);
        }
        if ((D[(v.x)-1][v.y].type!=1)&&(D[(v.x)-1][v.y].distance_charge==-1)) {
            D[v.x-1][v.y].distance_charge = D[v.x][v.y].distance_charge + 1;
            q.push(C[v.x-1][v.y]);
        }
        if ((D[v.x][(v.y)+1].type!=1)&&(D[v.x][(v.y)+1].distance_charge==-1)) {
            D[v.x][v.y+1].distance_charge = D[v.x][v.y].distance_charge + 1;
            q.push(C[v.x][v.y+1]);
        }
        if ((D[v.x][(v.y)-1].type!=1)&&(D[v.x][(v.y)-1].distance_charge==-1)) {
            D[v.x][v.y-1].distance_charge = D[v.x][v.y].distance_charge + 1;
            q.push(C[v.x][v.y-1]);
        }
    }
}
int bfs_num=0;
int* G;

void calculate_B(int start){
    bfs_num++;
    location** D;
    D=new location*[column+2];
    for (int i=0; i<column+2; i++) {
        D[i]=new location[row+2];
    }
    for (int x=0; x<=column+1; x++) {
        for (int y=0; y<=row+1; y++) {
            D[x][y]=C[x][y];
        }
    }
    BFS(D,start);
    
    for (int i=0; i<=num; i++) {
        G[i]=(D[all[i].x][all[i].y]).distance_charge;
    }
    for (int i=0; i<column+2; i++) {
        delete D[i];
    }
    delete D;
}

void calculate_all_distance(){
    for (int i=0; i<=num; i++) {
        calculate_B(i);
    }
    /*
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
     */
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

/////////////////////////// for normal case

void walk(int start,int end){
    
    next_is_charge=0;
    if (end!=0) {
        calculate_B(end);
        for (int i=0; i<=num; i++) {
            all[i].distance_now = G[i];
        }
    }
    else{
        for (int i=0; i<=num; i++) {
            all[i].distance_now = all[i].distance_charge;
        }
    }
    
    //find next,undone is better
    int next=tmp.index;
    bool check;
    int z;
    
    for (int count= all[start].distance_now-1 ; count>=0; count--) {
        check=0;
        // put location into ans[], and count ans_num
        ans_tmp->nextans=new ans(tmp.x,tmp.y);
        ans_tmp=ans_tmp->nextans;
        ans_num++;
        //find next

        /*
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
         */
         if (check==0) {
             if (A[tmp.x-1][tmp.y]!=1) {
                 z=index_chart[tmp.x-1][tmp.y];
                 if(all[z].distance_now==count){
                     next=z;
                     if (all[z].finish==0){
                         check=1;
                     }
                 }
             }
         }
         if (check==0) {
             if (A[tmp.x][tmp.y-1]!=1) {
                 z=index_chart[tmp.x][tmp.y-1];
                 if(all[z].distance_now==count){
                     next=z;
                     if (all[z].finish==0){
                         check=1;
                     }
                 }
             }
         }
        if (check==0) {
            if (A[tmp.x+1][tmp.y]!=1) {
                z=index_chart[tmp.x+1][tmp.y];
                if(all[z].distance_now==count){
                    next=z;
                    if (all[z].finish==0){
                        check=1;
                    }
                }
            }
        }
       if (check==0) {
           if (A[tmp.x][tmp.y+1]!=1) {
               z=index_chart[tmp.x][tmp.y+1];
               if(all[z].distance_now==count){
                   next=z;
                   if (all[z].finish==0){
                       check=1;
                   }
               }
           }
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
    }
}

void comeback(){
    walk(tmp.index, 0);
}

void go_near(){
    during_near=1;
    while (is_any_undone()==1) {
        
        for (int i=0; i<=num; i++) {
            all[i].distance_now = all[i].distance_charge - G[i];
        }
        goal_index=0;
        for (int i=0; i<=num; i++) {
            if(all[i].finish==0){
                if (all[i].distance_now>all[goal_index].distance_now) {
                    if(energy>=G[i]+all[i].distance_charge){
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
            during_near=0;
            break;
        }
    }
    during_near=0;
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

/////////////////////  for big case

int go_around(){
    
    int output=-1;
    int z;
    int priority=0;
    
    //check 上下左右還沒走過的 distance大的優先
    if (A[tmp.x-1][tmp.y]!=1) {
        z=index_chart[tmp.x-1][tmp.y];
        if(all[z].finish==false){
            if (energy>=1+all[z].distance_charge) {
                if (all[z].distance_charge>=priority) {
                    output=z;
                    priority=all[z].distance_charge;
                }
            }
        }
    }
    if (A[tmp.x][tmp.y-1]!=1) {
       z=index_chart[tmp.x][tmp.y-1];
       if(all[z].finish==false){
           if (energy>=1+all[z].distance_charge) {
               if (all[z].distance_charge>=priority) {
                   output=z;
                   priority=all[z].distance_charge;
               }
           }
       }
    }
    if (A[tmp.x+1][tmp.y]!=1) {
        z=index_chart[tmp.x+1][tmp.y];
        if(all[z].finish==false){
            if (energy>=1+all[z].distance_charge) {
                if (all[z].distance_charge>=priority) {
                    output=z;
                    priority=all[z].distance_charge;
                }
            }
        }
    }
    if (A[tmp.x][tmp.y+1]!=1) {
        z=index_chart[tmp.x][tmp.y+1];
        if(all[z].finish==false){
            if (energy>=1+all[z].distance_charge) {
                if (all[z].distance_charge>=priority) {
                    output=z;
                    priority=all[z].distance_charge;
                }
            }
        }
    }
    // 輸出下一個要走的點
    return output;
}

void walk_one_step(int goal){
    // walk tmp to direction d
    int next=goal;
    
    // put location into ans[], and count ans_num
    ans_tmp->nextans=new ans(tmp.x,tmp.y);
    ans_tmp=ans_tmp->nextans;
    ans_num++;
    
    energy--;
    tmp=all[next];
    if (all[tmp.index].finish==0) {
        all[tmp.index].finish=1;
        undone--;
    }
    if (during_near==1) {
        if (next==0) {
            next_is_charge=1;
        }
    }
}

void big_go_near(){
    during_near=1;
    while (is_any_undone()==1) {
        int d=go_around();
        if (d!=-1) {
            walk_one_step(d);
        }
        else{
            calculate_B(tmp.index);
            for (int i=0; i<=num; i++) {
                all[i].distance_now = all[i].distance_charge - G[i];
            }
            goal_index=0;
            for (int i=0; i<=num; i++) {
                if(all[i].finish==0){
                    if (all[i].distance_now>all[goal_index].distance_now) {
                        if(energy>=G[i]+all[i].distance_charge){
                            goal_index=i;
                        }
                    }
                }
            }
            if ((tmp.index==goal_index)||(goal_index==0)) {
                break;
            }
            walk(tmp.index,goal_index);
        }
        if (next_is_charge==1) {
            during_near=0;
            return;
        }
    }
    during_near=0;
}

//////////////////////////for very big case

class mystack{
public:
    int top=-1;
    int* X=new int[num+3];
    
    bool is_empty(){
        if(top==-1){
            return true;
        }
        else{
            return false;
        }
    }
    void push(int a){
        top++;
        X[top]=a;
    }
    int pop(){
        top--;
        return X[top+1];
    }
    mystack(){
        top=-1;
    }
    ~mystack(){
        delete []X;
    }
};

void walk_inverse(int start){
    
    for (int i=0; i<=num; i++) {
        all[i].distance_now = all[i].distance_charge;
    }
    tmp=all[start];
    //find next,undone is better
    int next=tmp.index;
    bool check;
    int z;
    mystack s;
    
    for (int count= all[start].distance_now-1 ; count>=0; count--) {
        check=0;
        if (all[tmp.index].finish==0) {
            all[tmp.index].finish=1;
            undone--;
        }
        //find next
         if (check==0) {
             if (A[tmp.x-1][tmp.y]!=1) {
                 z=index_chart[tmp.x-1][tmp.y];
                 if(all[z].distance_now==count){
                     next=z;
                     if (all[z].finish==0){
                         check=1;
                     }
                 }
             }
         }
         if (check==0) {
             if (A[tmp.x][tmp.y-1]!=1) {
                 z=index_chart[tmp.x][tmp.y-1];
                 if(all[z].distance_now==count){
                     next=z;
                     if (all[z].finish==0){
                         check=1;
                     }
                 }
             }
         }
        if (check==0) {
            if (A[tmp.x+1][tmp.y]!=1) {
                z=index_chart[tmp.x+1][tmp.y];
                if(all[z].distance_now==count){
                    next=z;
                    if (all[z].finish==0){
                        check=1;
                    }
                }
            }
        }
       if (check==0) {
           if (A[tmp.x][tmp.y+1]!=1) {
               z=index_chart[tmp.x][tmp.y+1];
               if(all[z].distance_now==count){
                   next=z;
                   if (all[z].finish==0){
                       check=1;
                   }
               }
           }
       }
        energy--;
        tmp=all[next];
        s.push(next);
    }
    // put location into ans[], and count ans_num
    int p;
    while (s.is_empty()==0) {
        p=s.pop();
        
        tmp=all[p];
        ans_tmp->nextans=new ans(tmp.x,tmp.y);
        ans_tmp=ans_tmp->nextans;
        ans_num++;
    }
    tmp=all[start];
    return;
}

void go_farest_inverse(){
    // pick farest
    goal_index=0;
    for (int i=0; i<=num; i++) {
        if (all[i].distance_charge>all[goal_index].distance_charge) {
            if (all[i].finish==0) {
                goal_index=i;
            }
        }
    }
    //walk farest to charge
    //store in stack and walk by pop
    walk_inverse(goal_index);

}
void very_big_go_near(){
    //only go around;
    during_near=1;
    while (is_any_undone()==1) {
        int d=go_around();
        if (d!=-1) {
            walk_one_step(d);
        }
        else{
            during_near=0;
            return;
        }
        if (next_is_charge==1) {
            during_near=0;
            return;
        }
    }
    during_near=0;
}

void initial(){
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
    index_chart=new int*[column+1];
    for (int i=0; i<column+1; i++) {
        index_chart[i]=new int[row+1];
    }
    for (int x=1; x<=column; x++) {
        for (int y=1; y<=row; y++) {
            index_chart[x][y]=-1;
        }
    }
    // collect all locations in  all[]
    all=new location[row*column+1];
    
    for (int x=1; x<=column; x++) {
        for (int y=1; y<=row; y++) {
            if (A[x][y]==0) {
                num++;
                all[num]=location(x, y);
                all[num].index=num;
                index_chart[x][y]=num;
            }
            else if (A[x][y]==2){
                all[0]=location(x, y);
                all[0].finish=true;
                all[0].index=0;
                index_chart[x][y]=0;
            }
        }
    }
    undone=num;
    
    C =new location*[column+2];
    for (int i=0; i<column+2; i++) {
        C[i]=new location[row+2];
    }
    for (int y=row+1; y>=0; y--) {
        for (int x=0; x<column+2; x++) {
            C[x][y]=location(x,y,A[x][y]);
        }
    }
    G=new int[num+1];
    
    calculate_B(0);
    for (int i=0; i<=num; i++) {
        all[i].distance_charge=G[i];
    }
}

/////////////////// main

int main() {
    
    freopen("floor.data", "r", stdin);
    freopen("final.path", "w", stdout);
    cin>>row>>column>>energy_max;
    
    initial();
        
    // Start travesal
    tmp=all[0];
    energy=energy_max;
    
    if (num<=100000) {
        //不用動的case
        if (num==0){
            cout<<"0"<<endl;
            ans(all[0].x,all[0].y).output();
            return 0;
        }
        
        while(undone>0){
            go_farest();
            go_near();
            comeback();
            energy=energy_max;
        }
        // End
    }
    else if(num<=600000){
        while(undone>0){
            go_farest();
            big_go_near();
            comeback();
            energy=energy_max;
        }
    }
    else{
        while(undone>0){
            go_farest_inverse();
            very_big_go_near();
            comeback();
            energy=energy_max;
        }
    }
    
    cout<<bfs_num<<endl;
    output_ans();
    
    return 0;
}
