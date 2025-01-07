import pygame
from pygame.locals import *
import sys

pygame.init()

# Framerate
FPS = pygame.time.Clock()
FPS.tick(60)
# FPS = 60
# FramePerSec = pygame.time.Clock()

# Default Colors
BLUE  = (0, 0, 255)
RED   = (255, 0, 0)
GREEN = (0, 255, 0)
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)

# Resolution
SCREEN_WIDTH = 400
SCREEN_HEIGHT = 600

DISPLAYSURF = pygame.display.set_mode((SCREEN_HEIGHT,SCREEN_WIDTH ))
DISPLAYSURF.fill(WHITE)
pygame.display.set_caption("sushi-engine")


while True:
    for event in pygame.event.get():
        if event.type == QUIT:
            pygame.quit()
            sys.exit()
    pygame.display.update()


# REMAINDER FOR STUPIDOS
# python -m venv venv
# venv/scripts/activate
# pip install -r requirements.txt // pip freeze > requirements.txt