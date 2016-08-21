#include <stdio.h>
#include <stdlib.h>
#include "Sudoku.h"

double problem[9][9];  // 問題を保存する配列。0: 空欄。1～9: 問題であらかじめ決められた値。
double linePattern[9][9 ^ 9];  // 各行における1～9の並べ方を保存する配列。一行の最大パターン数が9^9通りあるためint型の配列では足りない。9 ^ 9 = 387420489。


void init() {
  int i, j;
  
  for (i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++) problem[i][j] = 0;
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
  char *fname = "test.txt";
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
      k++;
    }
  }

  /* 問題を出力 */
  for (i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++) printf("%.0f  ", problem[i][j]);
    printf("\n");
  }

  /* ファイルクローズ */
  fclose(fp);
   
  return;
}


void SaveLinePattern() {

  return;
}
