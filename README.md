NPSfG (Number Place Solver for Generators)  
とん, 2016年2月


このプログラムは、ナンプレを自動生成するプログラムから使うことを想定して書かれたナンプレソルバーです。

このソルバーは、入力された問題を手筋を使って解き、「唯一解」「解無し」「不明(唯一解or複数解or解無し)」を判定することができます。

また、数字が入らなかったマスの数や、それらのマスに入る可能性がある数字を調べる関数が用意されています。  
これらの関数を用いて問題を評価することで、ナンプレを自動生成するプログラムを簡単に作ることができます。

main.cppは、問題が書かれているファイル(1行1問)を入力すると、入力された問題数,解けた問題数,解無しの問題数,解くことができなかった問題数を出力します。  
例えば、ヒント数17のナンプレ49,157問のリスト[1]を入力すると、以下のように出力されます。

----- 実行例 ここから -----  
Puzzle Count: 49157  
Solved Count: 21905  
No Solution Count: 0  
Unknown Count: 27252  
----- 実行例 ここまで -----

[1]  
ヒント数17のナンプレ49,157問のリストは以下のurlに載っています。  
Sudoku Puzzle Collections  
 Puzzles with 17 givens  
 (Gordon Royle's list of 49157 puzzles as of December 1st 2013)  
 https://sites.google.com/site/dobrichev/sudoku-puzzle-collections


このプログラムは、GPLの下で公開します。
