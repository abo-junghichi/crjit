# Cumulative Register JIT
累積レジスタマシンを仮想ISAとする、命令単位のJITコンパイラ。

## 構築済み
ライブラリ呼出規約を考慮した累積レジスタ割付の概念実証コード。

	$ gcc allocgreg-sc.c -o a.out
	$ ./a.out
	usage: allocgreg-sc.c [saved register count] [clobbered register count]
	# cat graph.data
	4	# ノード数
	ccss	# ライブラリ関数呼出による保存の要・不要
	0:1	# ノード間の干渉関係
	1:2
	2:3
	0:0	# 両端が同一ノードの干渉を入力されると終了
	$ ./a.out 0 0 < graph.data
