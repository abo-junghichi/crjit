#include <stdio.h>
#include <stdlib.h>
typedef struct {
    unsigned int vertex_num;
    char *saved /* saved[vertex_num] */ ;
    char *edge /* edge[vertex_num][vertex_num] */ ;
} graph;
static int input_graph(graph * rtnp)
{
    unsigned int i, n, vn;
    char *tmp, *saved, *edge;
    if (1 != scanf("%u", &vn))
	goto err;
    saved = malloc(sizeof(char) * vn);
    for (i = 0; i < vn; i++) {
	char sc[2];
	if (1 != scanf("%1s", sc))
	    goto err;
	switch (sc[0]) {
	case 's':
	    saved[i] = 1;
	    break;
	case 'c':
	    saved[i] = 0;
	    break;
	default:
	    goto err;
	}
    }
    edge = malloc(sizeof(char) * vn * vn);
    tmp = edge;
    for (n = 0; n < vn; n++)
	for (i = 0; i < vn; i++)
	    *(tmp++) = 0;
    while (1) {
	if (2 != scanf("%u:%u", &i, &n))
	    goto err;
	if (n == i)
	    break;
	edge[i * vn + n] = edge[n * vn + i] = 1;
    }
    for (i = 0; i < vn; i++)
	putchar(saved[i] ? 's' : 'c');
    putchar('\n');
    for (n = 0; n < vn; n++) {
	for (i = 0; i < vn; i++)
	    putchar(edge[n * vn + i] ? 'H' : '.');
	putchar('\n');
    }
    rtnp->vertex_num = vn;
    rtnp->saved = saved;
    rtnp->edge = edge;
    return 0;
  err:
    rtnp->vertex_num = 0;
    rtnp->saved = NULL;
    rtnp->edge = NULL;
    return 1;
}
typedef struct {
    char saved;
    unsigned int priority, counter_prio;
} color;
static color *graph_coloring(graph g)
{
    unsigned int i, n, vn = g.vertex_num, *used_prio =
	malloc(sizeof(int) * vn);
    color *rtn = malloc(sizeof(color) * vn);
    for (i = 0; i < vn; i++)
	used_prio[i] = vn;
    for (n = 0; n < vn; n++) {
	color cur;
	char *edge = g.edge + n * vn;
	cur.saved = g.saved[n];
	cur.counter_prio = 0;
	for (i = 0; i < n; i++)
	    if (edge[i]) {
		color *counter = rtn + i;
		unsigned int prio = counter->priority;
		if (cur.saved != counter->saved) {
		    if (cur.counter_prio <= prio)
			cur.counter_prio = prio + 1;
		} else
		    used_prio[prio] = n;
	    }
	for (i = 0; n == used_prio[i]; i++);
	cur.priority = i;
	rtn[n] = cur;
    }
    for (i = 0; i < vn; i++) {
	color cur = rtn[i];
	printf("%c,%u,%u\n", cur.saved ? 's' : 'c', cur.priority,
	       cur.counter_prio);
    }
    return rtn;
}
typedef struct {
    char mr /* mem(0) or reg(1) */ ;
    int disp;
} physreg;
static physreg alloc_physreg_core(unsigned int saved_reg,
				  unsigned int clobbered_reg,
				  color cumureg)
{
    physreg rtn;
    if (cumureg.saved) {
	rtn.disp = saved_reg - cumureg.priority - 1;
	rtn.mr = 0 <= rtn.disp
	    && cumureg.counter_prio <= rtn.disp + clobbered_reg;
	if (!rtn.mr)
	    rtn.disp = -cumureg.priority - 1;
    } else {
	rtn.disp = cumureg.priority - clobbered_reg;
	rtn.mr = 0 > rtn.disp
	    || saved_reg > rtn.disp + cumureg.counter_prio;
	if (!rtn.mr)
	    rtn.disp = cumureg.priority;
    }
    return rtn;
}
static physreg *alloc_physreg(unsigned int saved_reg,
			      unsigned int clobbered_reg,
			      unsigned int vertex_num,
			      color * cumureg_array)
{
    unsigned int i;
    physreg *rtn = malloc(sizeof(physreg) * vertex_num);
    for (i = 0; i < vertex_num; i++)
	rtn[i] =
	    alloc_physreg_core(saved_reg, clobbered_reg, cumureg_array[i]);
    for (i = 0; i < vertex_num; i++) {
	physreg cur = rtn[i];
	if (cur.mr) {
	    int saved = 0 <= cur.disp;
	    printf("%c%i\n", saved ? 's' : 'c',
		   saved ? cur.disp : -1 - cur.disp);
	} else
	    printf("m[%i]\n", cur.disp);
    }
    return rtn;
}
int main(int argc, char **argv)
{
    int rtn;
    graph g;
    color *cumu;
    physreg *phys;
    unsigned int saved_reg, clobbered_reg;
    if (3 != argc) {
	fprintf(stderr,
		"usage: %s [saved register count] [clobbered register count]\n",
		argv[0]);
	return 1;
    }
    saved_reg = atoi(argv[1]);
    clobbered_reg = atoi(argv[2]);
    if ((rtn = input_graph(&g)))
	goto err;
    cumu = graph_coloring(g);
    phys = alloc_physreg(saved_reg, clobbered_reg, g.vertex_num, cumu);
    free(phys);
    free(cumu);
  err:
    free(g.saved);
    free(g.edge);
    return rtn;
}
