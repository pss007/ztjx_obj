#include <stdio.h>
int main()
{
    int i,t ;
    i=2 ;
    t=1 ;
    while (i<=5)
    {
        t=i*t ;
        i=i+1 ;
    }
    printf ("%d\n",t) ;
    return 0 ;
}
