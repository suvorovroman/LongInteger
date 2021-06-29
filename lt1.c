#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lint.h"

/* 61500 65600 */

#define N 300
#define L 1000

int main()
{
  int i;
  unsigned int s;

  s = time(NULL);

  for(i = 1; i <= N; i++)
    {
      LINTEGER x, y, r, q, z;
      LADDRESS xe, ye, re, qe, ze;
      unsigned int n1, n2;

      
      n1 = rand_r(&s)%L;
      n2 = rand_r(&s)%L;
      
      printf("%5d. %d/%d mem:%u\n", i, n1, n2, lleak());

      xe = lintrand(&x, rand_r(&s)%L, &s);
      ye = lintrand(&y, rand_r(&s)%L, &s);

      re = lintcopy(&r, x);
      lintdiv(&r, &re, y, &q, &qe);
      lintmul(y, q, &z, &ze);
      lintadd(&z, &ze, r);
      if(lintcmp(x, z))
	{
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
