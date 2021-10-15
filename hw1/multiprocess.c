// compile: $ gcc multiprocess.c
// run: $ ./a.out datasize process_num
// (datasize<99999999, process_num<256)
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define VAL 99999999

u_int64_t num = 0;
u_int8_t buffer[VAL];
u_int8_t integer = 13;

int main(int argc, char *argv[]) {
  FILE *F ;
  F = fopen("multiprocess.csv","a");
  if(F == NULL) printf("gg\n");

  //prepare data
  u_int64_t datasize = atoi(argv[1]);
  u_int8_t process_num = atoi(argv[2]);
  printf("test setting: datasize=%lu, process_num=%d \n",datasize,process_num);
  int i,t;
  //unsigned seed;
//   seed = (unsigned)time(NULL); // 取得時間序列
//   srand(seed); // 以時間序列當亂數種子
  for(u_int64_t i=0;i<datasize;i++)
  {
    buffer[i]=(rand()%11+10);
  }
  printf("?");

  long long int time_sum = 0;
  for(int test_time=0;test_time<10;test_time++){
    num = 0;
    int status;
    pid_t pid[process_num];


    // calculate time cost
    struct  timeval start;
    struct  timeval end;
    long long int diff;
    gettimeofday(&start,NULL); //start time

    // child process
    for(u_int8_t i=0;i<process_num;i++){
      if ((pid[i] = fork()) == -1) {
        exit(EXIT_FAILURE);
      }
      else if (pid[i] == 0) {//start of child process
        u_int8_t count = 0;
        u_int64_t index_start = (datasize/process_num)*i;
        u_int64_t index_end;
        if (!(i == process_num-1)) index_end = (datasize/process_num)*(i+1);
        else index_end = datasize;

        for(u_int64_t j=index_start;j<index_end;j++){
          if (buffer[j]==integer) count++;
          // printf("%d %d %d\n", index_start,index_end,j);
        }
        exit(count);
      }
    }

    for(u_int8_t i=0;i<process_num;i++){//start of parent process
      if ((pid[i] = wait(&status)) == -1) ; //Wait for child process.
        // perror("wait error");
      else if ((pid[i] = wait(&status)) != 0) { //Check status.
        if (WIFSIGNALED(status) != 0) ;
          // printf("Child process ended because of signal %d \n",WTERMSIG(status));

        else if (WIFEXITED(status) != 0){
          num += WEXITSTATUS(status); //WEXITSTATUS 是一個巨集 (macro) , 他可以用來提取出指定的子程序結束狀態值
          // printf("Child process ended normally; status = %d \n",WEXITSTATUS(status));
        }
        // else printf("Child process did not end normally \n");
      }
    }


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