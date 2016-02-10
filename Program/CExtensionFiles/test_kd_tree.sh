
rm test_tree
gcc -g -std=c11 -o test_tree kdtree.c kd_tree_test.c
#gcc -std=c11 -g -o test_tree kdtree.c kd_tree_test.c
./test_tree

