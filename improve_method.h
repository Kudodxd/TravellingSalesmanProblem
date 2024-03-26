/*    Edited by Tran Cao Nhat Minh (Shizuoka Univ.)               */
/*    以下はTwoOpt,
            OR-OPT,
            焼きなまし法
            のTSPに対する改善法                                   */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "list.h"
//#include "construction_method.h"
#define MAX_N 10000   // 点の数の最大値
#define INF 100000000 // 無限大の定義
#define EPSILON 0.00000001 //ε 小さい正の値
#define SWAP(a,b){int temp; temp=(a); (a)=(b); (b)=temp; }   

int num = 0;
char tourFileName[20];

//kadai7-2
void TwoOpt_2(struct point p[MAX_N], int n, int tour[MAX_N]){
  int a,b,c,d;
  int i,j,k,l,g,h;
  int trig = 1;
  double diff;
  double shortest_diff = EPSILON;

  for (i = 0; i <= n-3; i++){
    j = i+1;
    for (k = i+2; k <= n-1; k++){
      l = (k+1) % n;
      a = tour[i]; b = tour[j];
      c = tour[k]; d = tour[l];

      diff = dist(p[a],p[b]) + dist(p[c],p[d])
           - dist(p[a],p[c]) - dist(p[b],p[d]);
      if (diff >= 0){
        g=j; h=k;
        while (g<h){
          SWAP(tour[g],tour[h]);
          g++;h--;
        }
        trig = 0;
      }
    }
    if (i==n-3){
      if (trig) return;
      else {
        i=-1;
        trig=1;
      }
    }
  }
}

//Endo-sanのTwoOpt
void TwoOpt(struct point p[MAX_N], int n, int tour[MAX_N]){
  int a,b,c,d;
  int i,j,k,l,g,h;
  double most_shortened_dist, shortened_dist;

  while(1){

    most_shortened_dist = 0;

    for(i = 0; i <= n - 3; i++){
      j = i + 1;
      for(k = i + 2; k <= n - 1; k++){
        l = (k + 1) % n;
        a = tour[i];
        b = tour[j];
        c = tour[k];
        d = tour[l];

        shortened_dist = dist(p[a], p[b]) + dist(p[c], p[d]) 
                       - dist(p[a], p[c]) - dist(p[b], p[d]);

        if(shortened_dist < most_shortened_dist) continue;

        most_shortened_dist = shortened_dist;
        
        g = j;
        h = k;
      }
    }

    if(most_shortened_dist < EPSILON) break;
    while(g < h){
      SWAP(tour[g], tour[h]);
      g++;
      h--;
    }
  }
}

//焼きなまし法 (Simulated Annealing)
#include <time.h>
#define MAX_ITERATIONS 300000
void Random_TwoOpt(int n, int tour[MAX_N]){
  srand((unsigned int)time(NULL));
  int i = rand() % n;
  int j = rand() % n;
  
  if (i > j) SWAP(i, j);

  while (i < j){
    SWAP(tour[i], tour[j]);
    i++; j--;
  }
}

double Decay_Temperature(int iter){
  return 10000*(exp(-0.0001*iter));
}

void SA(struct point p[MAX_N], int n, int tour[MAX_N]){
  int current_tour[MAX_N];
  int neighbor_tour[MAX_N];

  //初期化
  memcpy(current_tour, tour, n);
  double current_tour_len = tour_length(p, n, current_tour);
  double temperature;

  for (int i = 0; i < MAX_ITERATIONS; i++){
    temperature = Decay_Temperature(i);

    //current_tourをneighbor_tourにコピーする
    memcpy(neighbor_tour, current_tour, n);

    //新しいneigbor_tourをつくる
    Random_TwoOpt(n, neighbor_tour);

    double neighbor_tour_len = tour_length(p, n, neighbor_tour);
    if (neighbor_tour_len < current_tour_len || 
        rand() / (double)RAND_MAX < exp(current_tour_len - neighbor_tour_len) / temperature){
      //current_tourを更新する
      memcpy(current_tour, neighbor_tour, n);
      current_tour_len = neighbor_tour_len;
    }
  }

  //大域的最適解
  memcpy(tour, current_tour, n);
}

//OR-OPT (include 2 functions insert and removeAtIndex to work probably)
void or_Opt(struct point p[MAX_N], int n, int tour[MAX_N]){
  int i0, i1, j1, i, j, jj, ii, jj1;
  int dist1, dist2, dist3, dist4;
  double diff, larghest_diff;

  while (1){
    larghest_diff = 0;
    for (i = 0; i < n; i++){
      
      // i番目の都市を(j) - (j1)の経路に挿入する
      i0 = (i - 1 + n) % n;
      i1 = (i + 1) % n;

      for (j = 0; j < n; j++){
        j1 = (j + 1) % n;
        
        if (j != i && j1 != i){
          dist1 = dist(p[tour[i0]], p[tour[i]]) + dist(p[tour[i]], p[tour[i1]]); //i0 - i - i1
          dist2 = dist(p[tour[j]], p[tour[j1]]);                                 //j - j1 
          dist3 = dist(p[tour[i0]], p[tour[i1]]);                                //i0 - i1
          dist4 = dist(p[tour[j]], p[tour[i]]) + dist(p[tour[i]], p[tour[j1]]);  //j - i - j1

          diff = dist1 + dist2 - dist3 - dist4;
          if (diff <= larghest_diff) continue;
          larghest_diff = diff;
          ii = i;
          jj = j;
          jj1 = j1;
        }
      }      
    }
    
    if (larghest_diff < EPSILON) break;
    int tmp = tour[ii];

    //iを i0 - i1 に抜ける
    removeAtIndex(tour, &n, ii);

    //iを j - j1 に追加する
    if (ii < jj) insert(tour, &n, jj, tmp);
    else insert(tour, &n, jj1, tmp);
  }
}