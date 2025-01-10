import pygame
from settings import *

captured_pieces_white = []
captured_pieces_black = []

winner = ""

class Checkmate:
    def __init__(self, display={}, notation={}):
        return
    
    def draw_board():
        for i in range(32):
            column = i % 4
            row = i // 4
            if row % 2 == 0:
                pygame.draw.rect(DISPLAYSURF, 'light gray', [600 - (column * 200), row * 100, 100, 100])
            else:
                pygame.draw.rect(DISPLAYSURF, 'light gray', [700 - (column * 200), row * 100, 100, 100])
            pygame.draw.rect(DISPLAYSURF, 'gray', [0, 800, SCREEN_WIDTH, 100])
            pygame.draw.rect(DISPLAYSURF, 'gold', [0, 800, SCREEN_WIDTH, 100], 5)
            pygame.draw.rect(DISPLAYSURF, 'gold', [800, 0, 200, SCREEN_WIDTH], 5)
            status_text = ['White: Select a Piece to Move!', 'White: Select a Destination!',
                        'Black: Select a Piece to Move!', 'Black: Select a Destination!']
            DISPLAYSURF.blit(BIG_FONT.render(status_text[turn_step], True, 'black'), (20, 820))
            for i in range(9):
                pygame.draw.line(DISPLAYSURF, 'black', (0, 100 * i), (800, 100 * i), 2)
                pygame.draw.line(DISPLAYSURF, 'black', (100 * i, 0), (100 * i, 800), 2)
            DISPLAYSURF.blit(MEDIUM_FONT.render('FORFEIT', True, 'black'), (810, 830))
            if white_promote or black_promote:
                pygame.draw.rect(DISPLAYSURF, 'gray', [0, 800, SCREEN_WIDTH - 200, 100])
                pygame.draw.rect(DISPLAYSURF, 'gold', [0, 800, SCREEN_WIDTH - 200, 100], 5)
                DISPLAYSURF.blit(BIG_FONT.render('Select Piece to Promote Pawn', True, 'black'), (20, 820))
