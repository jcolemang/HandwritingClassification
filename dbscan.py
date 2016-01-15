"""

Please don't judge me on the code. 
I am writing this to be as stupid simple
as possible and am paying literally no mind
to efficiency. Really I'm almost 
intentionally inefficient in a couple
places just to make it that much easier
to write. I just want to see if this nonsense
will work at all.

"""
from math import sqrt


Noise = 0
Border = 1
Core = 2


def cluster( matrix ):
    clusters = set()
    points = split_matrix_into_points( matrix )
    



def split_matrix_into_points( matrix ):
    points = []
    for i in range(len(matrix)):
        for j in range(len(matrix[0])):
            val = matrix[i][j]
            if val == 255 or val == True:
                points.append( (i, j) )
    return points


class ClusterPoint(object):

    def __init__(self, x, y):
        self.x = x
        self.y = y
    

    def distance_to( self, other_point ):
        x_diff = other_point.x - self.x
        y_diff = other_point.y - self.y
        return sqrt(x_diff * x_diff + y_diff * y_diff)


    def get_points_within_dist( self, points, dist ):
        pts = []
        for point in points:
            if self.distance_to( point ) > dist:
                pts.append( point )
        return pts


    def get_num_points_within_dist( self, points, dist ):
        return len( self.get_num_points_within_dist(points dist) )


class DBSCANClusterPoint( ClusterPoint ):


    def __init__(self, x, y):
        super( DBSCANClusterPoint, self).__init__(x, y)
        self.classification = Noise
        self.connected_to = set()        

    def upgrade(self):
        if self.classification != Core:
            self.classification = Border

    def is_noise(self):
        return self.classification == Noise

    def is_core(self):
        return self.classification == Core

    def is_border(self):
        return self.classification == Border

    def connect_to( other_pt ):
        self.connected_to.add( other_pt )

    def is_connected_to( other_pt ):
        return other_pt in self.connected_to

def classify_points( points, eps, core_threshhold_num ): 
    for point in points:
        pts_in_radius = point.get_points_within_dist( eps )
        if len(pts_in_radius) > core_threshhold_num:
            point.classification = Core
            for pt in pts_in_radius:
                pt.upgrade()
                
                
def eliminate_noise_points( points ):
    for pt in points:
        if pt.is_noise():
            points.remove(pt)


# this should be done during classification
def get_core_points( points ):
    core_points = set()
    for pt in points:
        if pt.is_core():
            core_points.add(pt)
    return core_points

 
def connect_group_points( core_points, eps ):
    for i in range( len(core_points) ):
        for j in range( i+1, len(core_points) ):
            pt1 = core_points[i]
            pt2 = core_points[j]
            if pt1.distance_to( pt2 ) < eps:
                pt1.connect_to( pt2 )
                pt2.connect_to( pt1 )

def group_core_points( core_points ):
    groups = set()
    for pt in core_points:
        in_a_group = False
        for group in groups:
            for gp_pt in group:
                if pt.is_connected_to( gp_pt ):
                    in_a_group = True
                    group.add(pt)
                    break
        if not in_a_group:
            groups.add( {pt} )
    return groups

def group_border_points( border_points, core_groups, eps ):
    for pt in border_points:
        in_a_group = False
        for group in core_groups:
            for core_pt in group:
                if pt.distance_to( core_pt ) < eps:
                    in_a_group = True
                    group.add( pt )
                    break
        if in_a_group:
            break

def dbscan( eps, min_pts ):
    pass    







