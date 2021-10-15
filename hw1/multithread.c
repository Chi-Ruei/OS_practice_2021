#include <pthread.h>     // 引用 pthread 函式庫
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>   
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#define VAL 99999999

u_int64_t num = 0;
u_int8_t buffer[VAL];
u_int8_t integer = 13;

// 加入 Mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// 子執行緒函數
void* child(void *arg) {
  u_int64_t *input = (u_int64_t *) arg; // 取得資料
  u_int64_t local_num = 0;
  for(u_int64_t j=input[0];j<input[1];j++){
    if (buffer[j]==integer) local_num++;
  }

  pthread_mutex_lock( &mutex ); // 上鎖
  num += local_num;
  pthread_mutex_unlock( &mutex ); // 解鎖
  pthread_exit(NULL);
}

// int main()
// {
//     int arr[999];
//     int i,t;
//     unsigned seed;
//     seed = (unsigned)time(NULL); // 取得時間序列
//     srand(seed); // 以時間序列當亂數種子
//     for(i=0; i<999; ++i)
//     {
//         arr[i]=(rand()%11+10);
//     }
//     return 0;
// }

// 主程式
int main(int argc, char *argv[]) {
  FILE *F ;
  F = fopen("multithread.csv","a");
  if(F == NULL) printf("gg\n");

  //prepare data
  u_int64_t datasize = atoi(argv[1]);
  u_int8_t process_num = atoi(argv[2]);
  printf("test setting: datasize=%lu, thread_num=%d \n",datasize,process_num);
  int i,t;
  unsigned seed;
  seed = (unsigned)time(NULL); // 取得時間序列
  srand(seed); // 以時間序列當亂數種子
  for(u_int64_t i=0;i<datasize;i++)
  {
    buffer[i]=(rand()%11+10);
  }

  u_int64_t input[process_num][2];
  for(u_int8_t i=0;i<process_num;i++){
    input[i][0] = (datasize/process_num)*i; //index_start
    if (!(i == process_num-1)) input[i][1] = (datasize/process_num)*(i+1); //index_start
    else input[i][1] = datasize;
  }

  long long int time_sum = 0;
  for(int test_time=0;test_time<10;test_time++){
    num = 0;

    // calculate time cost
    struct  timeval start;
    struct  timeval end;
    long long int diff;
    gettimeofday(&start,NULL); //start time



    pthread_t t[process_num];
    for(u_int8_t i=0;i<process_num;i++){
      pthread_create(&t[i], NULL, child, (void*) input[i]);
    }


    for(u_int8_t i=0;i<process_num;i++) pthread_join(t[0], NULL);

    // calculate time cost
    gettimeofday(&end,NULL);
    diff = 1e6*(end.tv_sec-start.tv_sec) + (end.tv_usec-start.tv_usec);
    printf("time cost is %lld ms  ,",diff);

    time_sum += diff;
    printf("Integer 13 occurs %ld times in the array\n", num );
  }


  long long int time_avg = time_sum/10;
  printf("average time cost is %lld ms\n",time_avg);
  fprintf(F, "%lu, %d, %lld \n",datasize, process_num, time_avg);
  fclose(F);

  return 0;
}