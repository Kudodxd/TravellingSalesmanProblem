/*    Edited by Tran Cao Nhat Minh                                */
/*    以下はNearest Neighbor,
            Cheapest Insertion,
            Nearsest Insertion,
            Furthest Insertion, 
            最小木を用いる構築法
            のTSPに対する構築法です。                             */

#ifndef CONSTRUCTION
#define CONSTRUCTION
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "list.h"
#define MAX_N 10000   // 点の数の最大値
#define INF 100000000 // 無限大の定義
#define EPSILON 0.00000001 //ε 小さい正の値
#define SWAP(a,b){int temp; temp=(a); (a)=(b); (b)=temp; }   

struct point {
  int x;
  int y;
};

double dist(struct point p, struct point q) { // pとq の間の距離を計算 
  return sqrt((p.x-q.x)*(p.x-q.x)+(p.y-q.y)*(p.y-q.y));
}

double tour_length(struct point p[MAX_N], int n, int tour[MAX_N]) {
  int i;
  double sum=0.0;
  for(i=0;i<n;i++) sum+=dist(p[tour[i]],p[tour[(i+1)%n]]);
  return sum;// 総距離が関数の戻り値
}

//Nearsest Neighbor with "start" point
void nn_method(struct point p[MAX_N], int n, int tour[MAX_N],int start){
  int i;
  int a;
  double min;

  struct list visited;  //訪問済みの都市リスト
  struct list unvisited;//未訪問の都市リスト

  struct cell *nearest;
  //初期化
  initialize(&visited);
  initialize(&unvisited);
  insertBefore(visited.tail, start);    //都市0は訪問済み
  for (i = 0; i < n; i++){
    if (i == start) continue;
    insertBefore(unvisited.tail, i);    //他の都市は未訪問
  }
  tour[0]=start;                        // 最初に訪問する都市は 0 

  for(i = 0; i < n-1; i++) {
    a = tour[i];
    min = INF;          
    //最後に訪問した都市 a == tour[i]から最短距離にある未訪問都市nearestを見つける
    for (struct cell *check = unvisited.head->next; 
         check->next != NULL;
         check = check->next){
      if (dist(p[a],p[check->data]) < min){
        nearest = check;                //都市tour[i]から暫定的に最短距離にある未訪問都市をnearestとする
        min = dist(p[a],p[check->data]);// その暫定最短距離をminとする
      }
    }
    tour[i+1] = nearest->data;// i+1 番目に訪問する都市を nearest にして,
    // nearest を訪問済みとする.
    insertBefore(visited.tail, nearest->data);
    erase(nearest);
  }
  clear(&visited);
  clear(&unvisited);
}

//すべての点を出発点として最近近傍法を行い、最短の巡回路を導く
void nn_all(struct point p[MAX_N], int n, int tour[MAX_N]){
  int i;
  double min = INF;
  int tour_tmp[MAX_N];
  int tour_min[MAX_N];

  for(i = 0; i < n; i++){
    nn_method(p, n, tour_min, i);
    if(tour_length(p, n, tour_min) >= min) continue;
    memcpy(tour, tour_min, MAX_N);
    memcpy(tour_min, tour_tmp, MAX_N);
    min = tour_length(p, n, tour);
  }
}

//Cheapest Insertion with "start" point
void __ci_method(struct point p[MAX_N], int n, int tour[MAX_N], int start){

    //変数宣言
    int i;
    int sbtlen;
    double cost;
    double min;
    struct cell *r;
    struct cell *a;
    struct cell *min_a;
    struct cell *b;
    struct cell *min_b;
    struct cell *nearest;
    struct list visited;
    struct list unvisited;

    //初期化&Step1 任意の都市aを選ぶ
    initialize(&visited);
    insertBefore(visited.tail, start);
    initialize(&unvisited);
    for(i = 0; i < n; i++){
        if(i == start) continue;
        insertBefore(unvisited.tail, i);
    }


    //Step2 aから最も近い都市rを選んで部分巡回路a-r-aを作る
    a = visited.head -> next;
    min = INF;
    for(r = unvisited.head -> next;
        r != unvisited.tail;
        r = r -> next)
    {
        if(dist(p[a -> data], p[r -> data]) >= min) continue;
        nearest = r;
        min = dist(p[a -> data], p[r -> data]);
    }
    sbtlen = 2;
    insertBefore(visited.tail, nearest -> data);
    erase(nearest);

    //Step3 ~ Step5
    while(!isEmpty(&unvisited)){
        //Step3
        //現在の部分巡回路に含まれる枝(a,b)と現在の部分巡回路に含まれない点rで、
        //dist(a,r)+dist(b,r)-dist(a,b)が最小になる(a,b)とrを選ぶ
        min = INF;
        for(r = unvisited.head -> next;
            r != unvisited.tail;
            r = r -> next)
        {
            for(a = visited.head -> next;
                a != visited.tail;
                a = a -> next)
            {
                if(a -> next == visited.tail) b = visited.head -> next;
                else b = a -> next;
                cost = dist(p[a -> data], p[r -> data])
                     + dist(p[b -> data], p[r -> data])
                     - dist(p[a -> data], p[b -> data]);
                if(cost >= min) continue;
                min = cost;
                nearest = r;
                min_a = a;
                min_b = b;
            }
        }
        r = nearest;
        a = min_a;
        b = min_b;

        //Step4 (a,b)の間にrを挿入して新しい部分巡回路を作る。
        insertAfter(a, r -> data);
        erase(r);

        //Step5 現在の部分巡回路が全ての点を含んでいないならば、３へ戻る。そうでなければ終了する
    }

    //visitedに部分巡回路が保存されているため、tourに移す
    r = visited.head -> next;
    for(i = 0; i < n; i++){
        tour[i] = r -> data;
        r = r -> next;
    }

    return;
}

void ci(struct point p[MAX_N], int n, int tour[MAX_N]){
    __ci_method(p, n, tour, 0);
    return;
}

void ci_all(struct point p[MAX_N], int n, int tour[MAX_N]){
    
    //変数宣言
    int i;
    double min;
    int tour_tmp[MAX_N];

    //処理
    min = INF;
    memcpy(tour_tmp, tour, MAX_N);
    for(i = 0; i < n; i++){
        __ci_method(p, n, tour_tmp, i);
        if(tour_length(p, n, tour_tmp) >= min) continue;
        memcpy(tour, tour_tmp, MAX_N);
        min = tour_length(p, n, tour);
    }
}

void __ni_method(struct point p[MAX_N], int n, int tour[MAX_N], int start){
  //変数宣言
    int i;
    int sbtlen;
    double min;
    double dist_a_r_b;
    double dist_a_r_c;
    struct cell *r;
    struct cell *a;
    struct cell *min_a;
    struct cell *b;
    struct cell *c;
    struct cell *nearest;
    struct list visited;
    struct list unvisited;

    //初期化&Step1 任意の都市aを選ぶ
    initialize(&visited);
    insertBefore(visited.tail, start);
    initialize(&unvisited);
    for(i = 0; i < n; i++){
        if(i == start) continue;
        insertBefore(unvisited.tail, i);
    }


    //Step2 aから最も近い都市rを選んで部分巡回路a-r-aを作る
    a = visited.head -> next;
    min = INF;
    for(r = unvisited.head -> next;
        r != unvisited.tail;
        r = r -> next)
    {
        if(dist(p[a -> data], p[r -> data]) >= min) continue;
        nearest = r;
        min = dist(p[a -> data], p[r -> data]);
    }
    sbtlen = 2;
    insertBefore(visited.tail, nearest -> data);
    erase(nearest);

    //Step3 ~ Step5
    while(!isEmpty(&unvisited)){

        //Step3
        //部分巡回路に含まれない都市 r で部分巡回路までの
        //距離d(r)=min{dist(r,a)|aは部分巡回路内の都市}を最小化するものを選ぶ
        min = INF;
        for(r = unvisited.head -> next;
            r != unvisited.tail;
            r = r -> next)
        {
            for(a = visited.head -> next;
                a != visited.tail;
                a = a -> next)
            {
                if(dist(p[r -> data], p[a -> data]) >= min) continue;
                min = dist(p[r -> data], p[a -> data]);
                nearest = r;
                min_a = a;
            }
        }

        //Step4 上で選んだ都市をrとしてaをd(r)=dist(r,a)となる部分巡回路内の都市とする
        r = nearest;
        a = min_a;

        //Step5 部分巡回路内で都市aの前後にある都市をb,cとする
        if(a -> next == visited.tail) b = visited.head -> next;
        else b = a -> next;
        if(a -> prev == visited.head) c = visited.tail -> prev;
        else c = a -> prev;

        //Step6 dist(r,a)+dist(r,b)-dist(a,b)< dist(r,c)+dist(r,a)-dist(a,c)ならば，
        //(a,b)の間にrを挿入して新しい部分巡回路を作る。そうでなければ， 
        //(a,c)の間にrを挿入して新しい部分巡回路を作る。
        dist_a_r_b = dist(p[r -> data], p[a -> data])
                   + dist(p[r -> data], p[b -> data])
                   - dist(p[a -> data], p[b -> data]);
        dist_a_r_c = dist(p[r -> data], p[a -> data])
                   + dist(p[r -> data], p[c -> data])
                   - dist(p[a -> data], p[c -> data]);
        if(dist_a_r_b < dist_a_r_c) insertAfter(a, r -> data);
        else insertBefore(a, r -> data);
        erase(r);
        //Step7 現在の部分巡回路が全ての点を含んでいないならば、３へ戻る。そうでなければ終了。
    }

    //visitedに部分巡回路が保存されているため、tourに移す
    r = visited.head -> next;
    for(i = 0; i < n; i++){
        tour[i] = r -> data;
        r = r -> next;
    }

    return;
}

void ni(struct point p[MAX_N], int n, int tour[MAX_N]){
    __ni_method(p, n, tour, 0);
    return;
}

void ni_all(struct point p[MAX_N], int n, int tour[MAX_N]){
    
    //変数宣言
    int i;
    double min;
    int tour_tmp[MAX_N];

    //処理
    min = INF;
    memcpy(tour_tmp, tour, MAX_N);
    for(i = 0; i < n; i++){
        __ni_method(p, n, tour_tmp, i);
        if(tour_length(p, n, tour_tmp) >= min) continue;
        memcpy(tour, tour_tmp, MAX_N);
        min =tour_length(p, n,tour);
    }
}

void __fi_method(struct point p[MAX_N], int n, int tour[MAX_N], int start){
  //変数宣言
    int i;
    int sbtlen;
    double max;
    double dist_a_r_b;
    double dist_a_r_c;
    struct cell *r;
    struct cell *a;
    struct cell *max_a;
    struct cell *b;
    struct cell *c;
    struct cell *nearest;
    struct list visited;
    struct list unvisited;

    //初期化&Step1 任意の都市aを選ぶ
    initialize(&visited);
    insertBefore(visited.tail, start);
    initialize(&unvisited);
    for(i = 0; i < n; i++){
        if(i == start) continue;
        insertBefore(unvisited.tail, i);
    }


    //Step2 aから最も遠い都市rを選んで部分巡回路a-r-aを作る
    a = visited.head -> next;
    max = 0;
    for(r = unvisited.head -> next;
        r != unvisited.tail;
        r = r -> next)
    {
        if(dist(p[a -> data], p[r -> data]) <= max) continue;
        nearest = r;
        max = dist(p[a -> data], p[r -> data]);
    }
    sbtlen = 2;
    insertBefore(visited.tail, nearest -> data);
    erase(nearest);

    //Step3 ~ Step5
    while(!isEmpty(&unvisited)){

        //Step3
        //部分巡回路に含まれない都市 r で部分巡回路までの
        //距離d(r)=min{dist(r,a)|aは部分巡回路内の都市}を最大化するものを選ぶ
        max = 0;
        for(r = unvisited.head -> next;
            r != unvisited.tail;
            r = r -> next)
        {
            for(a = visited.head -> next;
                a != visited.tail;
                a = a -> next)
            {
                if(dist(p[r -> data], p[a -> data]) <= max) continue;
                max = dist(p[r -> data], p[a -> data]);
                nearest = r;
                max_a = a;
            }
        }

        //Step4 上で選んだ都市をrとしてaをd(r)=dist(r,a)となる部分巡回路内の都市とする
        r = nearest;
        a = max_a;

        //Step5 部分巡回路内で都市aの前後にある都市をb,cとする
        if(a -> next == visited.tail) b = visited.head -> next;
        else b = a -> next;
        if(a -> prev == visited.head) c = visited.tail -> prev;
        else c = a -> prev;

        //Step6 dist(r,a)+dist(r,b)-dist(a,b)< dist(r,c)+dist(r,a)-dist(a,c)ならば，
        //(a,b)の間にrを挿入して新しい部分巡回路を作る。そうでなければ， 
        //(a,c)の間にrを挿入して新しい部分巡回路を作る。
        dist_a_r_b = dist(p[r -> data], p[a -> data])
                   + dist(p[r -> data], p[b -> data])
                   - dist(p[a -> data], p[b -> data]);
        dist_a_r_c = dist(p[r -> data], p[a -> data])
                   + dist(p[r -> data], p[c -> data])
                   - dist(p[a -> data], p[c -> data]);
        if(dist_a_r_b < dist_a_r_c) insertAfter(a, r -> data);
        else insertBefore(a, r -> data);
        erase(r);
        //Step7 現在の部分巡回路が全ての点を含んでいないならば、３へ戻る。そうでなければ終了。
    }

    //visitedに部分巡回路が保存されているため、tourに移す
    r = visited.head -> next;
    for(i = 0; i < n; i++){
        tour[i] = r -> data;
        r = r -> next;
    }

    return;
}

void fi(struct point p[MAX_N], int n, int tour[MAX_N]){
    __fi_method(p, n, tour, 0);
    return;
}

void fi_all(struct point p[MAX_N], int n, int tour[MAX_N]){
    
    //変数宣言
    int i;
    double min;
    int tour_tmp[MAX_N];

    //処理
    min = INF;
    memcpy(tour_tmp, tour, MAX_N);
    for(i = 0; i < n; i++){
        __fi_method(p, n, tour_tmp, i);
        if(tour_length(p, n, tour_tmp) >= min) continue;
        memcpy(tour, tour_tmp, MAX_N);
        min =tour_length(p, n,tour);
    }
}

/*最小木を用いる構築法
1.　n個の都市を点集合とする完全グラフで，各枝の重みはその枝によって結ばれる2都市間のユークリッド距離とする．
2.　最小木𝑇を求める．
3.　𝑇のすべての枝のコピーを𝑇に追加したグラフを𝑇_2とする．
4.  𝑇_2のオイラー閉路を𝐶_2とする．
5.　オイラー閉路𝐶_2上の各点を，𝐶_2に最初に現れる順番で並べて得られる巡回路を𝐶′として，これを出力する．
*/

//caculate sum of array
int sum_array(int a[MAX_N], int n){
    int sum = 0;
    for (int i = 0; i < n; i++){
        sum += a[i];
    }
    return sum;
}

//完全グラフに対するPrimアルゴリズム
void Prim(struct point V[MAX_N], int q[MAX_N], int vn, int start){  // q[MAX_N]は部分最小木における親
    double p[MAX_N];  // 各点への最短距離（ポテンシャル）
                
    // 変数の初期化
    for (int i = 0; i < vn; i++) {
        p[i] = INF;
    }

    int U[vn], u = start, v;
    for (int i = 0; i < vn; i++){
        U[i] = 1;
    }
    p[start] = 0;
    U[start] = 0;
    while (sum_array(U, vn) != 0){
        for (v = 0; v < vn; v++){
            if (U[v] == 1){
                if (p[v] > dist(V[u], V[v])){
                    p[v] = dist(V[u], V[v]);
                    q[v] = u;
                }
            }
        }
        int min_p = INF;
        for (v = 0; v < vn; v++){
            if (U[v] == 1){
                if (p[v] < min_p){
                    u = v; 
                    min_p = p[v];
                }
            }
        }
        U[u] = 0;
    }
}

struct Node {
    int vertex;
    struct Node* next;
};

struct Graph{
    int numVertices;
    struct Node** adjLists; //array of pointers to nodes, representing the adjacency lists.
};

struct Graph* createGraph(int vertices){
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph -> numVertices = vertices;
    graph -> adjLists = (struct Node**)malloc(vertices * sizeof(struct Node*));

    for (int i = 0; i < vertices; i++){
        graph -> adjLists[i] = NULL;
    }

    return graph;
}

void addEdge(struct Graph* graph, int start, int dest){
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode -> vertex = dest;
    newNode -> next = graph -> adjLists[start];

    //add an edge in the other direction for undirected graph
    newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode -> vertex = start;
    newNode -> next = graph -> adjLists[dest];
    graph -> adjLists[dest] = newNode;
}

//euler_tourにノードを追加する
void push(int a[2 * MAX_N], int* n, int value){
    a[*n] = value;
    (*n)++;
}

int etn = 0;

//深さ優先探索
void dfs(struct Graph* graph, int vertex, int* visited, int euler_tour[2 * MAX_N]){
    visited[vertex] = 1;
    push(euler_tour, &etn, vertex);

    struct Node* tmp = graph -> adjLists[vertex];
    while (tmp != NULL){
        if (!visited[tmp -> vertex]){
            dfs(graph, tmp -> vertex, visited, euler_tour);
        }
        push(euler_tour, &etn, vertex);
        tmp = tmp -> next;
    } 
}

void Eulerian_circuit(int q[MAX_N], int vn, int start, int euler_tour[2 * MAX_N]){
    struct Graph* graph = createGraph(vn);

    for (int v = 0; v < vn; v++){
        if (v == start) continue;
        addEdge(graph, v, q[v]);
    }
    
    int* visited = (int *)malloc(graph -> numVertices * sizeof(int));
    for (int i = 0; i < graph -> numVertices; i++){
        visited[i] = 0;
    }

    dfs(graph, start, visited, euler_tour);
    
    free(visited);
    free(graph -> adjLists);
    free(graph);
}

void Create_tour_from_EulerCircuit(struct point p[MAX_N], int n, int tour[MAX_N]){
    int euler_tour[2 * MAX_N];// オイラー閉路
    int q[MAX_N];             // 部分最小木における親
    int s;                    // スタートポイント
    int min = INF;
    int tour_tmp[MAX_N];

    for (s = 0; s < n; s++){

        // 最小木をつくる
        Prim(p, q, n, s);

        // 最小木からオイラー閉路をつくる
        Eulerian_circuit(q, n, s, euler_tour); 

        for (int i = 0; i < n; i++){
            tour[i] = euler_tour[i];
            for (int j = i + 1; j < etn - 1; j++){
                if (euler_tour[j] == tour[i]){
                    removeAtIndex(euler_tour, &etn, j);
                }
            }
        }

        if (tour_length(p, n ,tour) < min){
            min = tour_length(p, n ,tour);
            memcpy(tour_tmp, tour, MAX_N);
        }
    }

    memcpy(tour, tour_tmp, MAX_N);
}

#endif