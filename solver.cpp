/*
    Copyright (C) 2016 Ton.
    All rights reserved.

    This file is part of NPSfG.

    NPSfG is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    NPSfG is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with NPSfG.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "solver.h"

using namespace std;

// 初期化
void Solver::init()
{
	emptyCellCount = BOARD_SIZE * BOARD_SIZE;
	candidateCount = BOARD_SIZE * BOARD_SIZE * BOARD_SIZE;
	answer = vector<int>(BOARD_SIZE * BOARD_SIZE, 0);

	candidates = vector< vector< bitset<BOARD_SIZE + 1> > >(BOARD_SIZE, vector< bitset<BOARD_SIZE + 1> >(BOARD_SIZE, (1 << (BOARD_SIZE + 1)) - 1));

	boxCandidateCount = vector< vector<int> >(BOARD_SIZE, vector<int>(BOARD_SIZE + 1, BOARD_SIZE));
	rowCandidateCount = vector< vector<int> >(BOARD_SIZE, vector<int>(BOARD_SIZE + 1, BOARD_SIZE));
	columnCandidateCount = vector< vector<int> >(BOARD_SIZE, vector<int>(BOARD_SIZE + 1, BOARD_SIZE));
	cellCandidateCount = vector< vector<int> >(BOARD_SIZE, vector<int>(BOARD_SIZE, BOARD_SIZE));

	errorFlag = false;
}

Solver::Solver()
{
	init();
}
Solver::Solver(const string& question)
{
	setQuestion(question);
}
Solver::Solver(const vector<int>& question)
{
	setQuestion(question);
}

// 問題代入
void Solver::setQuestion(const string& question)
{
	init();
	for (unsigned int i = 0; i < question.length() && i < BOARD_SIZE * BOARD_SIZE; ++i){
		int n = question[i] - '0';
		if (1 <= n && n <= BOARD_SIZE)
			addClue(i, n);
	}
}
void Solver::setQuestion(const vector<int>& question)
{
	init();
	for (unsigned int i = 0; i < question.size() && i < BOARD_SIZE * BOARD_SIZE; ++i)
		if (1 <= question[i] && question[i] <= BOARD_SIZE)
			addClue(i, question[i]);
}

// 候補削除
void Solver::deleteCandidate(const int& index, const int& n)
{
	deleteCandidate(index2x(index), index2y(index), n);
}
void Solver::deleteCandidate(const int& x, const int& y, const int& n)
{
	if (errorFlag == true) return;
	if (candidates[x][y][n] == false) return;
	candidates[x][y][n] = false;

	int box = xy2box(x, y);
	int box_x = x2box_x(x);
	int box_y = y2box_y(y);

	--boxCandidateCount[box][n];
	--rowCandidateCount[y][n];
	--columnCandidateCount[x][n];
	--cellCandidateCount[x][y];
	--candidateCount;

	// error
	if (boxCandidateCount[box][n] == 0
		|| rowCandidateCount[y][n] == 0
		|| columnCandidateCount[x][n] == 0
		|| cellCandidateCount[x][y] == 0){
		errorFlag = true;
		return;
	}

	// hidden single box
	if (boxCandidateCount[box][n] == 1){
		for (int i = 0; i<BOARD_SIZE; ++i){
			int tx = box_x + i % BOX_SIZE;
			int ty = box_y + i / BOX_SIZE;
			if (candidates[tx][ty][n] == true){
				addClue(tx, ty, n);
				break;
			}
		}
	}

	// hidden single line
	if (rowCandidateCount[y][n] == 1){
		for (int i = 0; i<BOARD_SIZE; ++i){
			if (candidates[i][y][n] == true){
				addClue(i, y, n);
				break;
			}
		}
	}
	if (columnCandidateCount[x][n] == 1){
		for (int i = 0; i<BOARD_SIZE; ++i){
			if (candidates[x][i][n] == true){
				addClue(x, i, n);
				break;
			}
		}
	}

	// naked single
	if (cellCandidateCount[x][y] == 1){
		for (int i = 1; i <= BOARD_SIZE; ++i){
			if (candidates[x][y][i] == true){
				addClue(x, y, i);
				break;
			}
		}
	}
}

// ヒント追加
void Solver::addClue(const int& index, const int& n)
{
	addClue(index2x(index), index2y(index), n);
}
void Solver::addClue(const int& x, const int& y, const int& n)
{
	if (errorFlag == true) return;
	if (candidates[x][y][n] == false){
		errorFlag = true;
		return;
	}

	int index = xy2index(x, y);
	if (answer[index] != 0){
		if (answer[index] != n)
			errorFlag = true;
		return;
	}

	int box = xy2box(x, y);
	int box_x = x2box_x(x);
	int box_y = y2box_y(y);

	answer[index] = n;
	--emptyCellCount;

	//候補削除
	int i;
	for (i = 0; i<BOARD_SIZE; ++i){
		int tx = box_x + i % BOX_SIZE;
		int ty = box_y + i / BOX_SIZE;
		if (tx != x || ty != y) deleteCandidate(tx, ty, n);
		if (i != x) deleteCandidate(i, y, n);
		if (i != y) deleteCandidate(x, i, n);
		if (i + 1 != n) deleteCandidate(x, y, i + 1);
	}
}

// 候補
bool Solver::getCandidate(const int& index, const int& n)const
{
	return getCandidate(index2x(index), index2y(index), n);
}
bool Solver::getCandidate(const int& x, const int& y, const int& n)const
{
	return candidates[x][y][n];
}

// 空きマス数
int Solver::getEmptyCellCount()const
{
	return emptyCellCount;
}

// 候補数の総和を調べる
int Solver::getCandidateCount()const
{
	return candidateCount;
}

// 候補数の平方和を計算する
int Solver::getCandidateSquareSum()const
{
	int squareSum = 0;
	for (int j = 0; j<BOARD_SIZE; ++j){
		for (int i = 0; i<BOARD_SIZE; ++i){
			int count = candidates[i][j].count();
			squareSum += count * count;
		}
	}
	return squareSum;
}

// 解答
int Solver::getAnswer(const int& index)const
{
	return answer[index];
}
int Solver::getAnswer(const int& x, const int& y)const
{
	return getAnswer(xy2index(x, y));
}
vector<int> Solver::getAnswer()const
{
	return answer;
}

// エラー
bool Solver::getError()const
{
	return errorFlag;
}

// 解く
bool Solver::solve()
{
	if (errorFlag == true)
		return false;
	if (emptyCellCount == 0)
		return true;
	return false;
}
