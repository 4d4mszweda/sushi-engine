import pygame

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

DISPLAYSURF = pygame.display.set_mode((SCREEN_HEIGHT,SCREEN_WIDTH))
DISPLAYSURF.fill(WHITE)
pygame.display.set_caption("sushi-engine")
