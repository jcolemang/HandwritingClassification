
import time
import random

import dbscan as cdbscan
from py_dbscan import c_cluster_points, py_cluster_points

def main():

    f = open('Benchmarking/benchmark_c_only', 'w')
    f.write('number,c,python\n')

    points_range = 1000
    num_points = 1 
    eps = 100
    threshold = 1
    times_to_run = 1000

    c_time = 0.0
    py_time = 0.0
    num_iterations = 0
    points = []

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
        c_start = time.time() 
        c_clusters = c_cluster_points( points, threshold, eps)
        c_time = time.time() - c_start


        py_start = time.time()
        py_clusters = py_cluster_points( points, threshold, eps)
        py_time = time.time() - py_start

        
        print 'Number of points clustered: {0}'.format( num_points )
        print 'Number of python clusters: {0}'.format( len(py_clusters) )
        print 'Number of c clusters: {0}'.format( len(c_clusters) )
        print 'C average: {0:.6f}'.format(c_time)
        print 'Python average: {0:.6f}'.format(py_time) 
        print 'Ratio: {0:.6f}\n'.format( py_time / c_time )

        f.write(str(num_points))
        f.write(',')
        f.write(str(c_time))
        f.write(',')
        f.write(str(py_time))
        f.write('\n')

        num_iterations += 1
        num_points += 100


    f.close()



if __name__ == '__main__':
    main()
