
"""

Please don't judge me on the code. 
I am writing this to be as stupid simple
as possible and am paying literally no mind
to efficiency. Really I'm almost 
intentionally inefficient in a couple
places just to make it that much easier
to write. I just want to see if this nonsense
will work at all.

TODO: Comment all this nonsense.

"""
from math import sqrt
#import pygame
import numpy
import pdb
import dbscan as cdbscan # this should be the c module
from PIL import Image
from sys import exit
import time

Noise = 0
Border = 1
Core = 2 

colors = [ (255, 0, 0), (0, 255, 0), (0, 0, 255), (255, 255, 0), (255, 0, 255), (0, 255, 255), (100, 100, 100), (150, 150, 150), (255, 100, 0), (255, 0, 100) ]
for i in range(100):
    colors.append((0,0,0))


def scale_to_max_val(vec, max_val=None):
    if max_val == None:
        max_val = max(vec)
    vec_copy = vec.copy()
    for i in range(len(vec)):
        vec_copy[i] = vec[i] / max_val
    return vec_copy

def replace_negatives(vec):
    return_vec = vec.copy()
    for i in range(len(vec)):
        return_vec[i] = abs(vec[i])
    return return_vec


def all_or_nothing(vec, max_val=1):
    vec_copy = vec.copy()
    for i in range(len(vec)):
        if vec[i] != 0:
            vec_copy[i] = max_val
        else:
            vec_copy[i] = 0
    return vec_copy


def clusters_to_surface( groups, size ):
    surf = pygame.Surface( size )
    surf.fill( (255, 255, 255) )
    for i in range(len(groups)):
        for point in groups[i]:
            color = colors[i]
            if type( point ) == tuple:
                surf.set_at( (point[0], point[1]), color )
            else:
                surf.set_at( point.get_position(), color )
    return surf


def split_matrix_into_point_tuples( matrix ):
    points = []
    for i in range(len(matrix)):
        for j in range(len(matrix[0])):
            val = matrix[i][j]
            if val == 255 or val == True:
                points.append( (i, j) )
    return points


def matrix_to_point_list( matrix ):
    points = []
    for i in range(len(matrix)):
        for j in range(len(matrix[0])):
            val = matrix[i][j]
            if val == 0:
                points.append( DBSCANClusterPoint( i, j ) )
    return points


class ClusterPoint(object):

    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.grouped = False
        self._distance_to = {}

    def get_position(self):
        return self.x, self.y


    def distance_to( self, other_point ):
        x_diff = ( other_point.x - self.x )
        y_diff = ( other_point.y - self.y )
        dist = sqrt(x_diff * x_diff + y_diff * y_diff)
        return dist


    def get_points_within_dist( self, points, dist ):
        pts = []
        for point in points:
            if self.distance_to( point ) > dist:
                pts.append( point )
        return pts


    def get_num_points_within_dist( self, points, dist ):
        return len( self.get_num_points_within_dist(points, dist) )


    def set_is_grouped(self, val=True):
        self.grouped = val 


    def is_grouped(self):
        return self.grouped


class DBSCANClusterPoint( ClusterPoint ):


    def __init__( self, x, y ):
        super( DBSCANClusterPoint, self).__init__(x, y)
        self.classification = Noise
        self.connected_to = []        
        self.visited = False


    def upgrade(self):
        if self.classification != Core:
            self.classification = Border

    def is_noise(self):
        return self.classification == Noise

    def is_core(self):
        return self.classification == Core

    def is_border(self):
        return self.classification == Border

    def connect_to( self, other_pt ):
        self.connected_to.append( other_pt )

    def is_connected_to( self, other_pt ):
        return other_pt in self.connected_to

    def has_been_visited(self):
        return self.visited


def classify_points( points, eps, core_threshhold_num ): 
    for point in points:
        pts_in_radius = point.get_points_within_dist( points, eps )
        if len(pts_in_radius) >= core_threshhold_num:
            point.classification = Core
            for pt in pts_in_radius:
                pt.upgrade()
                
                
def eliminate_noise_points( points ):
    for pt in points:
        if pt.is_noise():
            points.remove(pt)


def get_border_points( points ):
    border_points = [] 
    for pt in points:
        if pt.is_border():
            border_points.append(pt)
    return border_points

# this should be done during classification
def get_core_points( points ):
    core_points = [] 
    for pt in points:
        if pt.is_core():
            core_points.append(pt)
    return core_points

 
def connect_core_points( core_points, eps ):
    for i in range( len(core_points) ):
        for j in range( i+1, len(core_points) ):
            pt1 = core_points[i]
            pt2 = core_points[j]
            if pt1.distance_to( pt2 ) < eps:
                pt1.connect_to( pt2 )
                pt2.connect_to( pt1 )


def group_core_points( core_points ):
    groups = [] 
    for point in core_points:
        i = 0
        while i < len(groups):
            group = groups[i]
            for group_point in group:
                if group_point.is_connected_to(point):
                    group.append(point)
                    point.set_is_grouped()
                    break
            if point.is_grouped():
                break
            i += 1
        if not point.is_grouped():
            groups.append([point])
    return groups


def add_points_to_group(point, group):
    print len(group)
    if not point.has_been_visited():
        group.append(point)
        point.visited = True
        for sub_pt in point.connected_to:
            if sub_pt.has_been_visited():
                continue
            if point.is_core() and not sub_pt == point:
                add_points_to_group(sub_pt, group)



def group_border_points( border_points, core_groups, eps ):
    for pt in border_points:
        in_a_group = False
        for group in core_groups:
            for core_pt in group:
                if pt.distance_to( core_pt ) < eps:
                    in_a_group = True
                    group.append( pt )
                    break
        if in_a_group:
            break


# this is surely extremely slow. Find a builtin to do this for me
def PixelArray_to_numpy_array( pa, grayscale=True ):
    pa_size = len(pa), len(pa[0])
    arr = numpy.empty( pa_size, dtype=int )
    for i in range( pa_size[0] ):
        for j in range( pa_size[1] ):
            pixel = pa[i][j]
            new_val = ( (0xff & pixel) + ((0xff00 & pixel) >> 8) + ((0xff0000 & pixel) >> 16) ) // 3        
            arr[i][j] = new_val
    return arr


def get_average( iterable ):
    total = 0
    num = 0
    for i in iterable:
        num += 1
        total += i
    return total / num


# TODO: add support for the resizing option.
def get_display_matrix( display ):
    surface = pygame.display.get_surface()
    arr = PixelArray_to_numpy_array( pygame.PixelArray(surface) )
    return arr

"""
def get_display_vector( display, resize=True ):
    surf = pygame.display.get_surface()
    arr = PixelArray_to_numpy_array( pygame.PixelArray(surf) )
    im = Image.fromarray( numpy.uint8(arr), 'L')
    im_small = im.resize( image_size, Image.ANTIALIAS )
    vec = numpy.array( im_small )
    vec = vec.ravel()
    all_or_nothing(vec)
    return vec
"""


def combine_groups(groups):
    i = 0
    while i < len(groups):
        j = i + 1
        while j < len(groups):
            groups_combined = False
            for pt1 in groups[i]:
                for pt2 in groups[j]:
                    if pt1.is_connected_to(pt2):
                        groups_combined = True
                        for pt in groups[j]:
                            groups[i].append(pt)
                        groups.remove(groups[j])
                        break
                if not j < len(groups):
                    break
            if not groups_combined:
                j += 1
        i += 1



def cluster_to_square_image(cluster):

    padding = 5


    # the c version.
    if type(cluster[0]) == tuple:
        left_most_point = cluster[0]
        right_most_point = cluster[0]
        top_most_point = cluster[0]
        bottom_most_point = cluster[0]

        for point in cluster:
            if point[0] < left_most_point[0]:
                left_most_point = point
            if point[0] > right_most_point[0]:
                right_most_point = point
            if point[1] > bottom_most_point[1]:
                bottom_most_point = point
            if point[1] < top_most_point[1]:
                top_most_point = point

    # the python version.
    else:
        left_most_point = cluster[0].get_position()
        right_most_point = cluster[0].get_position() 
        top_most_point = cluster[0].get_position()
        bottom_most_point = cluster[0].get_position()

        for point in cluster:
            pos = point.get_position()
            if pos[0] < left_most_point[0]:
                left_most_point = point.get_position()
            if pos[0] > right_most_point[0]:
                right_most_point = point.get_position()
            if pos[1] > bottom_most_point[1]:
                bottom_most_point = point.get_position()
            if pos[1] < top_most_point[1]:
                top_most_point = point.get_position()

    top_left = left_most_point[0], top_most_point[1]
    bottom_right = right_most_point[0], bottom_most_point[1]
    
    # using top left as 0, -1
    num_rows = bottom_most_point[1] - top_most_point[1]
    num_cols = right_most_point[0] - left_most_point[0]

    row_shift = -top_most_point[1]
    col_shift = -left_most_point[0]
    
    if num_rows > num_cols:
        square_size = num_rows
        row_shift += padding / 2
        col_shift += (padding / 2) + (num_rows - num_cols) / 2
    else:
        square_size = num_cols
        row_shift += (padding / 2) + (num_cols - num_rows) / 2
        col_shift += (padding / 2) 
    
    im_array = numpy.zeros( (square_size+padding, square_size+padding) )

    if type(cluster[0]) == tuple:
        for point in cluster:
            x = point[0] + col_shift
            y = point[1] + row_shift
            im_array[y][x] = 255

    else:
        for point in cluster:
            pos = point.get_position()
            x = pos[0] + col_shift
            y = pos[1] + row_shift
            im_array[y][x] = 255

    image = Image.fromarray(im_array)
    
    return image    

def dbscan( display, threshold_num, eps, print_stuff=False ):
    t = time.time()
    pixel_array = get_display_matrix( display )

    if print_stuff: print 'Pixel array extracted. Size: {0}, {1}' \
                        .format( len(pixel_array), len(pixel_array[0]) )

    t = time.time()
    dbscan_points = matrix_to_point_list( pixel_array ) 

    if print_stuff: print 'Points extracted from array. Size: {0}'.format( len(dbscan_points) )

    t = time.time()
    classify_points( dbscan_points, eps, threshold_num )

    if print_stuff: print 'Points classified'
    s = len(dbscan_points)

    t = time.time()
    eliminate_noise_points( dbscan_points )

    if print_stuff: print 'Noise eliminated. \n\tSize before: {0}\n\tSize after: {1}'\
                        .format(s, len(dbscan_points))
    core_points = get_core_points( dbscan_points ) 
    border_points = get_border_points( dbscan_points )

    t = time.time()
    connect_core_points( core_points, eps )

    if print_stuff: print 'Core points connected\nNum core points: {0}'.format(len(core_points))
    t = time.time()
    groups = group_core_points( core_points )

    if print_stuff: print 'Core points grouped' 
    group_border_points( border_points, groups, eps  ) 
    if print_stuff:
        print 'Border points grouped\nNum border points: {0}'.format(len(border_points)) 
        print 'Number of clusters: {0}'.format( len(groups) )  
        print 'Combining clusters'
    combine_groups(groups)
    if print_stuff: print 'DBSCAN complete'
    return groups


def dbscanV2( display, eps, threshold_num ):
    pixarr = get_display_matrix(display)
    arr = numpy.array(pixarr)
    pt_locations = numpy.where( arr == 0)
    points = []
    for i in range(len(pt_locations[0])):
        points.append( (pt_locations[0][i], pt_locations[1][i]))
    clusters = cdbscan.dbscan( points, threshold_num, eps )
    #    for cluster in clusters:
    #        print cluster
    return clusters


def get_vectors(image, eps, threshold):
    pil_image = image
    image_array = numpy.array(pil_image)
    pt_locations = numpy.where( image_array != 0 ) 
    points = []
    for i in range(len(pt_locations)):
        points.append( (pt_locations[0][i], pt_locations[1][i]) )
    clusters = cdbscan.dbscan( points, threshold, eps )
    images = []
    image_vectors = []
    for cluster in clusters:
        images.append( cluster_to_square_image(cluster) )
    for image in images:
        resized = image.resize( (28,28), Image.ANTIALIAS )
        resized.show()
        vec = numpy.array(resized).ravel()
        vec = replace_negatives(vec)
        vec = scale_to_max_val( vec, max_val=255 )
        image_vectors.append(vec)
    return image_vectors
	
    

def color_clusters( display, threshold_num, eps ):
    clusters = dbscanV2( display, threshold_num, eps )
    return clusters_to_surface( clusters, display.get_size() )


def c_get_square_cluster_image_vectors( display, image_size, threshold_num, eps):
    groups = dbscanV2( display, eps, threshold_num )
    images = []
    surfaces = []
    image_vectors = []
    for group in groups:
        images.append( cluster_to_square_image( group ) )
    for image in images:
        resized = image.resize( image_size, Image.ANTIALIAS )
        vec = numpy.array(resized).ravel()
        vec = replace_negatives(vec)
        vec = scale_to_max_val(vec, max_val=255)
        image_vectors.append( vec )
    return image_vectors


def py_get_square_cluster_image_vectors( display, image_size, threshold_num, eps ):
    groups = dbscan( display, threshold_num, eps ) # the difference is here if you were wondering
    images = []
    surfaces = []
    image_vectors = []
    for group in groups:
        images.append( cluster_to_square_image( group ) )
    for image in images:
        resized = image.resize( image_size, Image.ANTIALIAS )
        vec = numpy.array(resized).ravel()
        vec = replace_negatives(vec)
        vec = scale_to_max_val(vec, max_val=255)
        image_vectors.append( vec )
    return image_vectors







def py_cluster_points(points, num_points, points_dist):
    dbscan_points = []
    for i in range(len(points)):
        dbscan_points.append( DBSCANClusterPoint( points[i][0], points[i][1] ))
    classify_points( dbscan_points, points_dist, num_points )
    eliminate_noise_points( dbscan_points )
    core_points = get_core_points( dbscan_points ) 
    border_points = get_border_points( dbscan_points )
    connect_core_points( core_points, points_dist )
    groups = group_core_points( core_points )
    group_border_points( border_points, groups, points_dist  ) 
    combine_groups(groups)
    return groups



def c_cluster_points(points, num_points, points_dist):
    return cdbscan.dbscan(points, num_points, points_dist) 

















