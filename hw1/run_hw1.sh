rm *.csv

gcc multithread.c -lpthread -o multithread.out
gcc multithread_string.c -lpthread -o multithread_string.out
gcc multithread_struct.c -lpthread -o multithread_struct.out
gcc multiprocess.c -o multiprocess.out
gcc multiprocess_string.c -o multiprocess_string.out
gcc multiprocess_struct.c -o multiprocess_struct.out

echo "generate csv"
for ((i=1;i<100000000;i*=2))
do
  for ((j=1;j<17;j++))
  do
   ./multithread.out $i $j  > /dev/null
   ./multithread_string.out $i $j > /dev/null
   ./multithread_struct.out $i $j > /dev/null
    ./multiprocess.out $i $j > /dev/null
   ./multiprocess_string.out $i $j > /dev/null
   ./multiprocess_struct.out $i $j > /dev/null



    if (( $j == 1 ))
    then
      echo "$i"
    fi

  done
done

echo "plot"
python3 plot.py
