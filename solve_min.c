/*                                                                */
/*    経路構築用プログラム                                        */
/*    C code written by K. Ando and K. Sekitani (Shizuoka Univ.)  */
/*              Version 2008.12.9                                 */
/*              Version 2013.05.07                                */
/*              Version 2013.05.22 revised                        */
/*              Version 2014.05.13 revised                        */
/*              Version 2015.06.17 revised                        */
/*    Edited by Tran Cao Nhat Minh (Shizuoka Univ.)                                */
/*                                                                */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "list.h"
#include "construction_method.h"
#include "improve_method.h"

void read_tsp_data(char *filename, struct point p[MAX_N],int *np) {
  FILE *fp;
  int visited[MAX_N]; // 都市iが訪問済みならば1そうでなければ0
  char buff[100];
  int i;

  if ((fp=fopen(filename,"rt")) == NULL) {// 指定ファイルを読み込み用に開く
    fprintf(stderr,"Error: File %s open failed.\n",filename);
    exit(0);
  }   

  while((fgets(buff,sizeof(buff),fp)!=NULL)   // DIMENSION で始まる行に出会う
	&&(strncmp("DIMENSION",buff,9)!=0)) ; // まで読み飛ばす. 
  sscanf(buff,"DIMENSION : %d",np);           // 点の数 *np を読み込む

  while((fgets(buff,sizeof(buff),fp)!=NULL)   // NODE_COORD_SECTIONで始まる
	&&(strncmp("NODE_COORD_SECTION",buff,18)!=0)) ; // 行に出会うまで, 
                                                        // 読み飛ばす. 
  for(i=0;i<*np;i++) {                       // i=0 から i=(*np)-1まで
    if(fgets(buff,sizeof(buff),fp)!=NULL) 
      sscanf(buff,"%*d %d %d",&(p[i].x),&(p[i].y)); // i番目の点の座標を読み込む
  }                                 

  fclose(fp);
}

void write_tour_data(char *filename, int n, int tour[MAX_N]){
  FILE *fp; 
  int i;
 
 // 構築した巡回路をfilenameという名前のファイルに書き出すためにopen
  if((fp=fopen(filename,"wt"))==NULL){ 
    fprintf(stderr,"Error: File %s open failed.\n",filename);
    exit(EXIT_FAILURE);
  }
  fprintf(fp,"%d\n",n);
  for(i=0;i<n; i++){
   fprintf(fp,"%d ",tour[i]);
  }
  fprintf(fp,"\n");
  fclose(fp);
}

//全ての構築法で巡回路を作り、最も短いものを返す。
void all_method_only(struct point p[MAX_N], int n, int tour[MAX_N]){
  int min;
  int tour_tmp[MAX_N];
  int tour_min[MAX_N];

  min = INF;
  for(int i = 0; i < 4; i++){
    switch(i){   //nn、ci、ni、fiを順番に行なう
      case 0:
        nn_all(p, n, tour_tmp);
        break;
      case 1:
        ci(p, n, tour_tmp);
        break;
      case 2:
        ni(p, n, tour_tmp);
        break;
      case 3:
        fi(p, n, tour_tmp);
    }
    if (tour_length(p, n, tour_tmp) >= min) break;
    min = tour_length(p, n, tour_tmp);
    memcpy(tour_min, tour_tmp, MAX_N);
  }

  memcpy(tour, tour_min, MAX_N);
}

//全ての構築法で作った巡回路に2-Optを試みて、最も短い巡回路を返す
void all_method_with_TwoOpt(struct point p[MAX_N], int n, int tour[MAX_N]){
  int min = INF;
  int tour_tmp[MAX_N];
  int tour_min[MAX_N];

  for(int i = 0; i < 4; i++){
    memcpy(tour_tmp, tour, MAX_N);
    switch(i){   //nn、ci、ni、fiを順番に行なう
      case 0:
        nn_all(p, n, tour_tmp);
        break;
      case 1:
        ci(p, n, tour_tmp);
        break;
      case 2:
        ni(p, n, tour_tmp);
        break;
      case 3:
        fi(p, n, tour_tmp);
    }
    TwoOpt(p, n, tour_tmp);
    if (tour_length(p, n, tour_tmp) >= min) continue;
    min = tour_length(p, n, tour_tmp);
    memcpy(tour_min, tour_tmp, MAX_N);
  }

  memcpy(tour, tour_min, MAX_N);
}

int main(int argc, char *argv[]) {
  int  n;                   // 点の数 
  struct point  p[MAX_N];   // 各点の座標を表す配列 
  int tour[MAX_N];   // 巡回路を表現する配列
  
  if(argc != 2) {
    fprintf(stderr,"Usage: %s <tsp_filename>\n",argv[0]);
    exit(EXIT_FAILURE);
  }

  // 点の数と各点の座標を1番目のコマンドライン引数で指定されたファイルから読み込む
  read_tsp_data(argv[1],p,&n);

  //構築法とTwoOptによる改善法を実行し、最も改善した巡回路を導く
  // all_method_only(p, n, tour);
  // all_method_with_TwoOpt(p, n, tour);
  nn_all(p, n, tour);
  
  // 最小木を用いる構築法
  // Create_tour_from_EulerCircuit(p, n, tour);

  // 2opt による改善
  // TwoOpt(p, n, tour);

  // OR-OPTによる改善
  // or_Opt(p , n, tour);

  // SA による改善
  // SA(p, n, tour);

  // 巡回路をテキストファイルとして出力
  sprintf(tourFileName,"tour%08d.dat",++num);
  write_tour_data(tourFileName,n,tour);
  // 巡回路長を画面に出力
  printf("%lf\n",tour_length(p,n,tour));

  exit(EXIT_SUCCESS);
}
