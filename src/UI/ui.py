import pygame
import sys
from settings import *
from elements import *

def main():
    running = True
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:  
                running = False
        pygame.display.update()

    pygame.quit()
    sys.exit()


# REMAINDER FOR STUPIDOS
# python -m venv venv
# venv/scripts/activate
# pip install -r requirements.txt // pip freeze > requirements.txt

if __name__ == "__main__":
    main()