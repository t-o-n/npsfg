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

#ifndef __solver_h__
#define __solver_h__

#include <bitset>
#include <climits>
#include <string>
#include <vector>

class Solver
{
	// ブロック 3x3, マス 9x9
	static const int BOX_SIZE = 3;
	static const int BOARD_SIZE = BOX_SIZE * BOX_SIZE;

	static int index2x(const int& index){ return index % BOARD_SIZE; }
	static int index2y(const int& index){ return index / BOARD_SIZE; }
	static int xy2index(const int& x, const int& y){ return x + y * BOARD_SIZE; }
	
	// x,yからブロック(box)の番号を計算する
	//   0,1,2,
	//   3,4,5,
	//   6,7,8,
	static int xy2box(const int& x, const int& y){ return x / BOX_SIZE + y / BOX_SIZE * BOX_SIZE; }

	// x,yが属するブロックの左上座標を計算する
	static int x2box_x(const int& x){ return x / BOX_SIZE * BOX_SIZE; }
	static int y2box_y(const int& y){ return y / BOX_SIZE * BOX_SIZE; }

	// 空きマス数
	//   初期値 : 81
	int emptyCellCount;

	// 候補数
	//   初期値 : 729 (=81*9)
	int candidateCount;

	// 解答
	//   初期値 : 0,0,0,… (長さ81)
	std::vector<int> answer;

	// 候補
	//   位置 x,y に 数字 n が入れられるかどうかのフラグ
	//   初期値 : 全て true (どの数字も入る可能性がある)
	std::vector< std::vector< std::bitset<BOARD_SIZE + 1> > > candidates; // true:候補有 -> false:候補無

	// 各行,各列,各ブロックの各数字の候補数
	//   初期値 : 全て 9
	std::vector< std::vector<int> > rowCandidateCount;		// y,n
	std::vector< std::vector<int> > columnCandidateCount;	// x,n
	std::vector< std::vector<int> > boxCandidateCount;		// box,n

	// 各マスの候補数
	//   初期値 : 全て 9
	std::vector< std::vector<int> > cellCandidateCount;	// x,y

	// 解無しを判定するフラグ
	//   初期値 : false
	//            true (解無し)
	bool errorFlag;

	// 初期化
	void init();

	// 候補を削除する
	//   index : 削除する位置(0-80)
	//   n : 削除する数字(1-9)
	void deleteCandidate(const int& index, const int& n);
	//   x : 削除する位置(0-8)
	//   y : 削除する位置(0-8)
	//   n : 削除する数字(1-9)
	void deleteCandidate(const int& x, const int& y, const int& n);

public:
	Solver();
	Solver(const std::string& question);
	Solver(const std::vector<int>& question);

	// 問題代入
	//   question : 長さ81のstringかvector<int>
	//              1-9以外の文字や値は0とする
	//     例 : 4......1..2...6..8....186341.......7....63...5.....2...8......1...5.2.....4...95.
	//     例 : 083000000500000027090000000000230050908010000000500480200040800000000061000700500
	void setQuestion(const std::string& question);
	void setQuestion(const std::vector<int>& question);

	// ヒント追加
	//   index : 数字を追加する位置(0-80)
	//   n : 追加する数字(1-9)
	void addClue(const int& index, const int& n);
	//   x : 数字を追加する位置(0-8)
	//   y : 数字を追加する位置(0-8)
	//   n : 追加する数字(1-9)
	void addClue(const int& x, const int& y, const int& n);

	// 候補の有無を調べる
	//   index : 調べる位置(0-80)
	//   n : 調べる数字(1-9)
	//   return : 候補の有無(true:有, false:無)
	bool getCandidate(const int& index, const int& n)const;
	//   x : 調べる位置(0-8)
	//   y : 調べる位置(0-8)
	//   n : 調べる数字(1-9)
	//   return : 候補の有無(true:有, false:無)
	bool getCandidate(const int& x, const int& y, const int& n)const;

	// 空きマスの数を調べる
	//   return : 空きマス数(0-81)
	int getEmptyCellCount()const;

	// 候補数の総和を調べる
	int getCandidateCount()const;

	// 候補数の平方和を計算する
	//   return : 候補数の平方和(81-6561)
	int getCandidateSquareSum()const;
	
	// 問題の評価値を計算する
	//   return : 評価値
	//     errorFlagがtrueならINT_MAXを返す。
	int getScore()const{
		if (errorFlag)
			return INT_MAX;
		//return getEmptyCellCount(); // 空きマスの数を評価値として使う
		//return getCandidateCount(); // 候補数の総和を評価値として使う
		return getCandidateSquareSum(); // 候補数の平方和を評価値として使う
	}

	// 解答を調べる
	//   index : 調べる位置(0-80)
	//   return : 解答(0-9の配列)
	int getAnswer(const int& index)const;
	//   x : 調べる位置(0-8)
	//   y : 調べる位置(0-8)
	//   return : その位置の解答(0-9)
	int getAnswer(const int& x, const int& y)const;
	//   return : 全ての位置の解答を調べる
	std::vector<int> getAnswer()const;

	
	// "解無し"かを調べる
	//   return : 解無しか(true:解無し, false:不明)
	//
	//   getError()==true -> 解無し
	//   getError()==false && getEmptyCellCount()==0 -> 唯一解
	//   getError()==false && getEmptyCellCount()>0 -> 不明
	bool getError()const;
	
	
	// 解く
	//   return : 解けたか(true:唯一解, false:不明)
	//     実装されている手筋では解けない場合、不明を返す。
	//
	//   solve()==true -> 唯一解
	//   solve()==false && getError()==true -> 解無し
	//   solve()==false && getError()==false -> 不明
	bool solve();

};
#endif
