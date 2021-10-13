#include <stdio.h>
#include <stdlib.h>
#include <time.h>
 
int main()
{
    int arr[999];
    int i,t;
    unsigned seed;
    seed = (unsigned)time(NULL); // 取得時間序列
    srand(seed); // 以時間序列當亂數種子
    for(i=0; i<999; ++i)
    {
        arr[i]=(rand()%11+10);
    }
    return 0;
}