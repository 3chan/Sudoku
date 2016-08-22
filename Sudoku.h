#ifndef __SUDOKU_H__
#define __SUDOKU_H__

void init();  // 配列の初期化を行う

void ReadProblem();  // ファイルから問題を読み込む
void PrintProblem(double prob[][9]);  // 数独の配列を標準出力
void SaveLinePattern();  // 行ごとに取り得る1～9の並びの組み合わせを調べる
void Combine1to9(double prob[], int line, int index);  // 1～9の並びの組み合わせを調べる
int CheckDuplication(double prob[], int num, int index);  // 列内に数字のの重複が無いか調べる
void CopyProblem(double prob[][9]);  // 問題の配列を他の配列にコピーする

#endif
