import pygame
import sys
from settings import *
from elements import *

running = True
system = System()

x = 0 # TMP

while running:
    if x < 100: # TMP
        x += 1 # TMP
        
    for event in pygame.event.get():
        if event.type == pygame.QUIT:  
            running = False
        elif event.type == pygame.MOUSEBUTTONDOWN:
            mouse_pos = event.pos
            system.handle_button_click(mouse_pos)


    DISPLAYSURF.fill(WHITE)
    system.draw_score(x)
    system.draw_board()
    system.draw_history()

    pygame.display.update()
    delta = FPS_TIMER.tick(FPS) / 1000

pygame.quit()
sys.exit()


# REMAINDER FOR STUPIDOS
# python -m venv venv
# venv/scripts/activate
# pip install -r requirements.txt // pip freeze > requirements.txt