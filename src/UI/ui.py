import pygame
import sys
from settings import *
from elements import *

running = True
system = System()

while running:        
    running = system.event_listener()
    system.update()
    pygame.display.update()
    delta = FPS_TIMER.tick(FPS) / 1000

system.sushi_stop()
pygame.quit()
sys.exit()


# REMAINDER FOR STUPIDOS
# python -m venv venv
# venv/scripts/activate
# pip install -r requirements.txt // pip freeze > requirements.txt