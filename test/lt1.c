#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lint.h"

/* 61500 65600 */

#define N 100		/* Количество итераций */
#define L 100000	/* Максимальная длина числа */

int main()
{
  int i;

  srand(time(NULL));

  for(i = 1; i <= N; i++)
    {
      LINTEGER x, y, r, q, z;
      lcell *xe, *ye, *re, *qe, *ze;
      int n1, n2;

      printf("%5d. mem:%u\n", i, lleak());

      xe = lintrand(&x, -L);
      ye = lintrand(&y, -L);

      re = lintcopy(&r, x);

      lintdiv(&r, &re, y, &q, &qe);
      lintmul(y, q, &z, &ze);
      lintadd(&z, &ze, r);
      if(lintcmp(x, z))
	{
	  printf("Error\n");
	  printf("x:");
	  lintout(stdout, x);
	  printf("\ny:");
	  lintout(stdout, y);
	  printf("\nz:");
	  lintout(stdout, z);
	  printf("\nq:");
	  lintout(stdout, q);
	  printf("\nr:");
	  lintout(stdout, r);
	  printf("\n");
	  return 2021;
	}

      lintdelex(x, xe);
      lintdelex(y, ye);
      lintdelex(r, re);
      lintdelex(q, qe);
      lintdelex(z, ze);
    }
  
  return 0;
}
