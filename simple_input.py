from PIL import Image
import numpy
import sys
import pygame
pygame.init()

import time

# defining some global connstants
black = 0, 0, 0
white = 255, 255, 255
# ~~~~~ Note that resize quality will be important. The computer needs to be able to read our data.
size = 500, 500
image_size = 28, 28
mouse_was_pressed = False
prev = pygame.mouse.get_pos()
current = pygame.mouse.get_pos()
image_path = '/home/coleman/Pictures/im.png'


# this is surely extremely slow. Find a builtin to do this for me
def PixelArray_to_numpy_array( pa, grayscale=True ):
    pa_size = len(pa), len(pa[0])
    arr = numpy.empty( pa_size, dtype=int )
    for i in range( pa_size[0] ):
        for j in range( pa_size[1] ):
            pixel = pa[i][j]
            new_val = ( (0xff & pixel) + ((0xff00 & pixel) >> 8) + ((0xff0000 & pixel) >> 16) ) // 3        
            arr[j][i] = new_val
    return arr


def get_average( iterable ):
    total = 0
    num = 0
    for i in iterable:
        num += 1
        total += i
    return total / num


def all_or_nothing(vector, max_val=255, min_val=0, threshhold=225):
    for i in range(len(vector)):
        if vector[i] > threshhold:
            vector[i] = max_val
        else:
            vector[i] = min_val


def get_display_vector( display ):
    surf = pygame.display.get_surface()
    arr = PixelArray_to_numpy_array( pygame.PixelArray(surf) )
    im = Image.fromarray( numpy.uint8(arr), 'L')
    im_small = im.resize( image_size, Image.ANTIALIAS )
    vec = numpy.array( im_small )
    vec = vec.ravel()
    all_or_nothing(vec)
    return vec


def check_input( display ):
    global prev, current, mouse_was_pressed

    for event in pygame.event.get():

        # keyboard input
        if event.type == pygame.KEYDOWN:
            # which key?    
            if event.key == pygame.K_RETURN:
                vec = get_display_vector(display)
                im = Image.fromarray( numpy.reshape(vec, (28, 28)) )
                im.save( image_path )
                # This all will absolutely be redesigned but some classification could go here.
                display.fill( white )


        # the 'X' arrow
        if event.type == pygame.QUIT:
            exit_application()
        
        if pygame.mouse.get_pressed()[0]:
            prev = current
            current = pygame.mouse.get_pos()
            draw_on_mouse( display, 3, black, prev, current )
 
        else:
            mouse_pressed = False
            prev = pygame.mouse.get_pos()
            current = pygame.mouse.get_pos()

def draw_on_mouse( display, radius, color, prev, curr ):
    
    pygame.draw.line( display, color, prev, curr, 3 )    



def exit_application():
    sys.exit()




def main():
    
    background_color = white
    display = pygame.display.set_mode(size)
    display.fill(background_color)

    while True:

        check_input( display )
        
        pygame.display.update()





if __name__ == "__main__":
    main()
