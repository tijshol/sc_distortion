
#include <stdio.h>

main()
{
int k;

	while ( scanf ("%d", &k) != EOF) {
		//printf("k = %d\n",k);
		write (1, &k, 1);
		}
}
