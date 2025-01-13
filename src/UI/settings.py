import pygame

pygame.init()

# Framerate
FPS_TIMER = pygame.time.Clock()
FPS = 60
delta = 0


# Default Colors
BLUE  = (0, 0, 255)
RED   = (255, 0, 0)
GREEN = (0, 255, 0)
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
SOFT_BLACK = (48, 48, 48)
SOFT_RED = (209, 74, 65)
GOLD = (250, 223, 22)
MAIN_BOARD = (92, 32, 32)
SECOND_BOARD = (251,245,222)
GREY_TEXT = (148, 148, 148)

# Resolution
SCREEN_WIDTH = 1000
SCREEN_HEIGHT = 700

# Fonts
FONT = pygame.font.Font('freesansbold.ttf', 20)
MEDIUM_FONT = pygame.font.Font('freesansbold.ttf', 40)
BIG_FONT = pygame.font.Font('freesansbold.ttf', 50)

# Icon
ICON = pygame.image.load('resources/icon.png')
pygame.display.set_icon(ICON)

# Screen
DISPLAYSURF = pygame.display.set_mode((SCREEN_WIDTH,SCREEN_HEIGHT))
DISPLAYSURF.fill(WHITE)
pygame.display.set_caption("sushi-engine")
