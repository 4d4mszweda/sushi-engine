import pygame

pygame.init()

# Framerate
FPS_TIMER = pygame.time.Clock()
FPS = 60


# Default Colors
BLUE  = (0, 0, 255)
RED   = (255, 0, 0)
GREEN = (0, 255, 0)
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)

# Resolution
SCREEN_WIDTH = 1000
SCREEN_HEIGHT = 900

# Fonts
FONT = pygame.font.Font('freesansbold.ttf', 20)
MEDIUM_FONT = pygame.font.Font('freesansbold.ttf', 40)
BIG_FONT = pygame.font.Font('freesansbold.ttf', 50)

# Screen
DISPLAYSURF = pygame.display.set_mode((SCREEN_HEIGHT,SCREEN_WIDTH))
DISPLAYSURF.fill(WHITE)
pygame.display.set_caption("sushi-engine")
