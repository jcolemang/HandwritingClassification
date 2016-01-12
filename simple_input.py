import sys
import pygame
pygame.init()

# defining some global connstants
black = 0, 0, 0
white = 255, 255, 255
size = 500, 500
mouse_was_pressed = False
prev = pygame.mouse.get_pos()
current = pygame.mouse.get_pos()

def check_input( display ):
    global prev, current, mouse_was_pressed

    
    
    for event in pygame.event.get():


        # keyboard input
        if event.type == pygame.KEYDOWN:
            # which key?    
            if event.key == pygame.K_p:
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
