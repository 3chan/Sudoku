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

  pcnt = 0;
  Combine1to9(prob[1], 0, 0);  // ひとまず1行目のみ考える

  /* 2行目以降はのちに実装
  for (i = 0; i < 9; i++) {
    pcnt = 0;  // 行毎にポインタpcntを初期化
    Combine1to9(prob[i], i, 0);
  }
  */
  
  return;
}
 
 
void Combine1to9(double prob[], int line, int index) {
  int i = 1, j;
  while (1) {
    if (defaultNums[line][index] == 1) {  // 空マスでないなら
      /* デバッグ */
      printf("空マスではない\n");
      
      if (index < 8) {  // 空マスでない、かつ右端ではないなら右隣のマスを調べる
	Combine1to9(prob, line, index + 1);
	if (index == 0) {
	  /** 戻ってきたとき何しよう **/
	}
      }
      else if (index == 8) {  // 空マスでない、かつ右端なら現在の並びを保存する
	/* 保存 */
	for (j = 0; j < 9; j++) {
	  linePattern[line][pcnt] = prob[j];
	  pcnt++;
	}
      }
      printf("戻ります(空マスでない, 右端かは関係なし)\n");
      return;
    }
    else {  // 空マスなら
      while (CheckDuplication(prob, i, index) == 1) {  // 今入れようとしている数字と横ラインに数字の重複がある場合
	if (i == 9) {  // 入れようとしている数字が9ならば
	  // 重複がある、かつ数字が9、ならマスを空（初期化）して戻る（右端かどうかは関係ない。保存しないのだから）
	  prob[index] = 0;
	  printf("戻ります(空マス, 重複有, i = 9, 右端かは関係なし)\n");
	  return;
	}
	else {  // 入れようとしている数字が1～8の場合
	  i++;  // 調べる数をインクリメント
	}
      }
      
      // ここに来た == 入れようとしている数字と横ラインの数字に重複がない状態
      printf("重複なし, index = %d,  i = %d", index, i);
      prob[index] = i;  // 数字を入れる
      if (index < 8) {  // 空マスに数字を入れた、かつ右端でないなら右隣を調べる
	Combine1to9(prob, line, index + 1);
	/** 戻ってきたとき何しよう **/
      }
      else if (index == 8) {  // 空マスに数字を入れた、かつ右端なら現在の並びを保存する
	/* 保存 */
	for (j = 0; j < 9; j++) {
	  linePattern[line][pcnt] = prob[j];
	  pcnt++;
	}
	printf("戻ります(空マスに数字を入れた, 重複なし, 右端である, i = %d)\n", i);
	return;
      }
    }

    // 空マスか否か + その内部の処理終えたらここに来る
  }
  // while を break で抜けたらここに来る
}
      
      
int CheckDuplication(double prob[], int num, int index) {  // 1: 重複あり、0: 重複なし
  int i;

  for (i = 0; i < 9; i++) {
    if (i != index) {  // 自分自身とは比較しない
      if (num == prob[i]) return 1;
    }
  }
  return 0;
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
