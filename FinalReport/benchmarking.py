
import time
import random

import dbscan as cdbscan
from py_dbscan import c_cluster_points, py_cluster_points

def main():


    points_range = 100000000
    num_points = 1 
    eps = 1
    threshold = 1
    times_to_run = 50
    iteration_repetitions = 5

    c_time = 0.0
    py_time = 0.0
    prev_c_time = 1
    num_iterations = 0
    points = []
    benchmark_c = True
    benchmark_python = True
    double_points = False

    header = 'iterations,number'

    filename = str(points_range)
    if benchmark_c:
        filename += 'c'
        header += ',c_time,c_clusters'
    if benchmark_python:
        filename += 'python'
        header += ',py_time,py_clusters'


    f = open('BenchmarkingData/' + filename, 'w')
    f.write(header + '\n')

    while num_iterations < times_to_run:

        print 'Creating list.'

        while len(points) < num_points:
            candidate = (random.randint(0, points_range), random.randint(0, points_range))
            repeat = False
            for x in range(len(points)):
                if points[x][0] == candidate[0] and points[x][1] == candidate[1]:
                    repeat = True
                    break
            if not repeat:
                points.append( candidate )

        print 'Timing...'
        if benchmark_c:
            for i in range(iteration_repetitions):
                c_start = time.time() 
                c_clusters = c_cluster_points( points, threshold, eps)
                c_time = time.time() - c_start

        if benchmark_python:
            for i in range(iteration_repetitions):
                py_start = time.time()
                py_clusters = py_cluster_points( points, threshold, eps)
                py_time = time.time() - py_start

        
        f.write(',' + str(num_iterations) + ',' + str(num_points)) 

        if benchmark_c:
            f.write(',' + str(c_time) + ',' + str(len(c_clusters)))
        if benchmark_python:
            f.write(',' + str(py_time) + ',' + str(len(py_clusters)))

        f.write('\n')

        num_iterations += 1
        if double_points:
            num_points *= 2
        else:
            num_points += 100


    f.close()



if __name__ == '__main__':
    main()
