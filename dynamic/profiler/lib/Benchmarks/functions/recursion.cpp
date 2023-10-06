/* MDH WCET BENCHMARK SUITE. File version $Id: recursion.c 7800 2018-01-19 12:43:12Z lkg02 $ */

/* Generate an example of recursive code, to see  *
 * how it can be modeled in the scope graph.      */

/* self-recursion  */
int fib(int i)
{
  if(i==0)
    return 1;
  if(i==1)
    return 1;
  return fib(i-1) + fib(i-2);
}

/* mutual recursion */
int anka(int);

int kalle(int j)
{
  if(j<=0)
    return 0;
  else
    return anka(j-1);
}

int anka(int k)
{
  if(k<=0)
    return 1;
  else
    return kalle(k-1);
}

extern volatile int In;

int recursion(void)
{
  kalle(10);
  In = fib(10);
  return 0;
}