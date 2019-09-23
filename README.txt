InfoChip8 v0.1Jリリースノート
Copyright (c) 2005 Jay's Factory

■はじめに

InfoChip8 v0.1J for Win32は、Win32上で動作するCHIP-8エミュレータです。
高速で、移植性が高いことが特徴です。

■インストール方法

(1) アーカイブInfoChip801.zipを展開する
(2) 実行ファイル./InfoChip801/bin/InfoChip8.exeを、適当な場所
　　（例：c:\cygwin\usr\local\bin）にコピーする
(3) 環境変数PATHに、コピー先（例：c:\cygwin\usr\local\bin）設定する
　　[コントロールパネル] - [システム] - [詳細設定] - [環境変数]

■起動方法

(1) コマンドラインから、以下をタイプする

$ InfoChip8 [ROMファイル名（例：Pong）]

(2) もしくは、ROMファイルを、実行ファイルInfoChip8.exeに、ドラッグアン
    ドドロップする

(3) 終了方法は、[×]をクリックする

■操作方法

キーは、以下のようにマッピングされている。

オリジナル　|1|2|3|C|　　マッピング　|4|5|6|7|
　　　　　　|4|5|6|D|　　　　　　　　|R|T|Y|U|
　　　　　　|7|8|9|E|　　　　　　　　|F|G|H|J|
　　　　　　|A|0|B|F|　　　　　　　　|V|B|N|M|

■仕様

□メモリ
　- ROMファイル（200H - F10H）
　- 16進数フォント（F10H - F60H）

□レジスタ
　- データレジスタ（V0 .. VF）
　- アドレスレジスタ（I）
　- タイマ（ディレイ、サウンド）
　- スタック（16ワード長、スタックポインタ）

□グラフィックス
　- スプライト（CHIP-8モード：8 x 1 .. 15）
　- 衝突フラグ
　- 16進数フォント

□命令セット
　- CHIP-8命令（代入、算術、条件分岐、サブルーチン、スプライト、等）

□キーボード
　- 16進数キーボード

■ビルド方法

ソースより、バイナリを生成するためには、以下を実行する。

□準備

(1) Cygwinをインストール時に、以下をチェックする

All + Devel + gcc: C, C++, Fortran compilers
All + Devel + gcc-mingw: Mingw32 support headers and libraries for GCC

(2) 以下のファイルをダウンロードし、展開する

DirectX for MinGW
http://www.libsdl.org/extras/win32/common/directx-devel.tar.gz

(3) インクルードファイルとライブラリファイルをコピーする

$ cp ./include/*.h /usr/i686-pc-mingw32/include
$ cp ./lib/*.a /usr/i686-pc-mingw32/lib

□リビルド

(4) アーカイブInfoChip801.zipを展開する
(5) ディレクトリ./InfoChip801/src/win32に移動する

$ cd ./InfoChip801/src/win32

(6) リビルドする

$ make clean
$ make

[EOF]
