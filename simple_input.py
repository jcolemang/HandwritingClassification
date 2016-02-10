from PIL import Image
import numpy
import sys
import pygame
pygame.init()
from classifier import Classifier
import dbscan

import time

# defining some global connstants
black = 0, 0, 0
white = 255, 255, 255
size = 500, 100
image_size = 28, 28
mouse_was_pressed = False
eraser_mode = False
r = 10
prev = pygame.mouse.get_pos()
current = pygame.mouse.get_pos()
save_path = '/home/coleman/Pictures/saved.bmp'



def check_input( display, classifier):
    global prev, current, mouse_was_pressed, eraser_mode, save_path

    for event in pygame.event.get():
        # keyboard input
        if event.type == pygame.KEYDOWN:
            # which key?    

            if event.key == pygame.K_RETURN:
                print 'Enter pressed'
                vectors = dbscan.get_square_cluster_image_vectors( display, (28, 28) )

                title = ''
                for v in vectors:
                    title += str(classifier.predict(v))
                pygame.display.set_caption(title)

                surf = dbscan.color_clusters( display )
                display.blit( surf, (0, 0) )
                pygame.display.update()
                time.sleep(1)

            if event.key == pygame.K_e:
                print "'e' pressed"
                display.fill( white )

            if event.key == pygame.K_s:
                print "'s' pressed"
                pygame.image.save(display, save_path)


        # the 'X' arrow
        if event.type == pygame.QUIT:
            exit_application()
       
        if pygame.mouse.get_pressed()[1]:
            eraser_mode = True
        else:
            eraser_mode = False

        if pygame.mouse.get_pressed()[0]:
            prev = current
            current = pygame.mouse.get_pos()
            if eraser_mode:
                c = white
            else:
                c = black
            draw_on_mouse( display, r, c, prev, current )
 
        else:
            mouse_pressed = False
            prev = pygame.mouse.get_pos()
            current = pygame.mouse.get_pos()

def draw_on_mouse( display, radius, color, prev, curr ):
    pygame.draw.line( display, color, prev, curr, radius )    



def exit_application():
    sys.exit()




def main():
    
    background_color = white
    display = pygame.display.set_mode(size)
    display.fill(background_color)
    clock = pygame.time.Clock()
    classifier = Classifier()

    while True:
        clock.tick(60)
        check_input( display, classifier )            
        pygame.display.update()





if __name__ == "__main__":
    main()
