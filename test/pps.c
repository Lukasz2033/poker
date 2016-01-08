#include <stdio.h>

typedef struct node{
	int val;
	struct node *next;
}node;

int main(){
/**
	node v, *pv, **ppv;
	v.val = 3;
	v.next = 0;

	pv = &v;
	ppv = &pv;
	printf("%d \n", (*pv).val);
	printf("%d \n", pv->val);
	printf("%d \n", (**ppv).val);
**/

int a, b, *pa1, *pa2, **ppa;

a = 1;
b = 2;

pa1 = &a;
pa2 = &b;

ppa = &pa1;
//*ppa = pa2;

printf("%d\n", *pa1);		

return 0;
}
