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

#include <fstream>
#include <iostream>
#include "solver.h"

using namespace std;

int main(int argc, char *argv[])
{
	// ナンプレの問題ファイル読み込み
	// 1行1問
	if (argc != 2){
		cout << "Usage: " << argv[0] << " filename" << endl;
		return -1;
	}
	ifstream ifs(argv[1]);
	if (ifs.fail()){
		cout << "No such file: " << argv[1] << endl;
		return -1;
	}

	string str;
	int puzzleCount = 0;	// 問題数
	int solvedCount = 0;	// 解けた問題数
	int noSolutionCount = 0;	// 解無しの問題数
	int unknownCount = 0;		// 実装されている手筋では解くことができなかった問題数
	while (getline(ifs, str)){
		Solver solver(str);
		if (solver.solve() == true){
			// 唯一解
			++solvedCount;
		}
		else{
			if (solver.getError()){
				// 解無し
				++noSolutionCount;
			}
			else{
				// 不明 (複数解 or 唯一解 or 解無し)
				// (実装されている手筋では解けなかった)
				++unknownCount;
			}
		}
		++puzzleCount;
	}

	cout << "Puzzle Count: " << puzzleCount << endl;
	cout << "Solved Count: " << solvedCount << endl;
	cout << "No Solution Count: " << noSolutionCount << endl;
	cout << "Unknown Count: " << unknownCount << endl;

	return 0;
}
