import pygame
import sys
from settings import *
from elements import *

def main():
    # board = ChessBoard(8, 50)
    # pieces = [
    #     ChessPiece('path_to_white_rook.png', (0, 0)),
    #     ChessPiece('path_to_black_rook.png', (350, 350))
    # ]

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