#include <stdio.h>
#include <stdlib.h>
#include "Sudoku.h"

double problem[9][9];  // 問題を保存する配列。0: 空欄。1～9: 問題であらかじめ決められた値。
double defaultNums[9][9];  // 各マスに問題であらかじめ決められた値が入っているかを登録しておく配列。0: 空欄、1: 問題であらかじめ値が決められているマス。
double linePattern[9][9 ^ 9];  // 各行における1～9の並べ方を保存する配列。一行の最大パターン数が9^9通りあるためint型の配列では足りない。9 ^ 9 = 387420489。

int pcnt;  // 配列linePattern内のどこまでパターンが保存されているかを指すポインタ


void init() {
  int i, j;
  
  for (i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++) {
      problem[i][j] = 0;
      defaultNums[i][j] = 0;
    }
  }

  for (i = 0; i < 9; i++) {
    for (j = 0; j < (9 * 9); j++) linePattern[i][j] = 0;
  }
}


int main() {
  /* 配列の初期化 */
  init();

  /* 問題の読み込み */
  ReadProblem();

  /* 各行における1～9の並び方をあらかじめ調べる */
  SaveLinePattern();
  
  return 0;
}


void ReadProblem() {
  FILE *fp;
  char *fname = "test2.txt";
  double temp1[9 * 9], temp2;
  int i, j, k;
  
  /* ファイルオープン */
  if ((fp = fopen(fname, "r")) == NULL) {
    printf("Failed to open file \"%s\"\n", fname);
    exit(1);
  }

  /* ファイル読み込み */
  j = 0;
  for (i = 0; i < (10 * 9); i++) {  // (一行9マス + 改行) * 9行
    temp2 = getc(fp);
    if (i % 10 != 9) {  // 改行は読まない (飛ばす)
      temp1[j] = temp2;
      j++;
    }
  }
  /* [デバッグ] 読み込んだファイルを出力
  for (i = 0; i < (9 * 9); i++) {
    printf("%.0f  ", temp1[i]);
    if (i % 9 == 8) printf("\n");
  }
  */

  /* 問題を配列に保存 */
  k = 0;
  for (i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++) {
      problem[i][j] = temp1[k] - 48;  // 16進数から10進数に戻す処理
      if (problem[i][j] != 0) defaultNums[i][j] = 1;  // 空欄でないマスを登録
      k++;
    }
  }

  /* デバッグ */
  PrintProblem(problem);
  printf("defaultNums: \n");
  PrintProblem(defaultNums);

  /* ファイルクローズ */
  fclose(fp);
   
  return;
}


void PrintProblem(double prob[][9]) {
  int i, j;
  
  /* 問題を出力 */
  for (i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++) printf("%.0f  ", prob[i][j]);
    printf("\n");
 } 
}

 
void SaveLinePattern() {
  double prob[9][9];
  int i, j,  k;

  CopyProblem(prob);  // 配列problemの値をローカルの配列probにコピー  <= 必要か??

  /* デバッグ */
  printf("CopiedProblem: \n");
  PrintProblem(prob);
  printf("\n");
  
  Combine1to9(prob[1], 1, 0);  // ひとまず1行目のみ考える

  /* 2行目以降はのちに実装
  for (i = 0; i < 9; i++) {
    pcnt = 0;  // 行毎にポインタpcntを初期化
    Combine1to9(prob[i], i, 0);
  }
  */
  
  return;
}
 
 
void Combine1to9(double prob[], int line, int index) {
  int i, j, k, l;

  if (defaultNums[line][index] == 1) {  // あらかじめ空欄でなければ飛ばす
    index++;
    Combine1to9(prob, line, index);
  }
  
  for (i = 0; i < 9; i++) {
    
    for (j = 0; j < 9; j++) {  // 行内に既にその数字が無いか調べる
      if (index != j && prob[j] == i) {
	i++;
	break;
      }
    }
    
    if (8 < i) return;
    else if (index == 8) {  // パターンを保存
      for (k = 0; k < 9; k++) {
	linePattern[line][pcnt] = prob[k];
	pcnt++;
      }
      /* デバッグ */
      printf("prob = {");
      for (l = 0; l < 9; l++) printf("%.0f, ", prob[l]);
      printf("}  line = %d, index = %d, i = %d, pcnt = %d\n", line, index, i, pcnt);  // デバッグ
    }
    else if (index < 8) {
      prob[index] = i;
      index++;
      Combine1to9(prob, line, index);
    }
  }
  
  return;
}


void CopyProblem(double prob[][9]) {
  int i, j;

  for (i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++) {
      prob[i][j] = problem[i][j];
    }
  }

  return;
}
