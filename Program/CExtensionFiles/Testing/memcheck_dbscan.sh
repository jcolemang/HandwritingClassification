rm test_mem_dbscan
gcc -std=c11 -g dynamic_array.c kd_tree.c dbscan.c test_dbscan.c -o test_mem_dbscan
valgrind --tool=memcheck --leak-check=yes ./test_mem_dbscan
echo 'done with valgrind. running program alone.'
./test_mem_dbscan
