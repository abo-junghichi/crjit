# Cumulative Register JIT
���ѥ쥸�����ޥ������ISA�Ȥ��롢̿��ñ�̤�JIT����ѥ��顣

## ���ۺѤ�
�饤�֥��ƽе�����θ�������ѥ쥸�������դγ�ǰ�¾ڥ����ɡ�

	$ gcc allocgreg-sc.c -o a.out
	$ ./a.out
	usage: allocgreg-sc.c [saved register count] [clobbered register count]
	# cat graph.data
	4	# �Ρ��ɿ�
	ccss	# �饤�֥��ؿ��ƽФˤ����¸���ס�����
	0:1	# �Ρ��ɴ֤δ��Ĵط�
	1:2
	2:3
	0:0	# ξü��Ʊ��Ρ��ɤδ��Ĥ����Ϥ����Ƚ�λ
	$ ./a.out 0 0 < graph.data
