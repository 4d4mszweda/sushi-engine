import pygame
from settings import WHITE, BLACK 

class ChessPiece:
    def __init__(self, image_path, position):
        self.image = pygame.image.load(image_path)
        self.position = position

    def draw(self, surface):
        surface.blit(self.image, self.position)

class ChessBoard:
    def __init__(self, board_size, square_size):
        self.board_size = board_size
        self.square_size = square_size
        self.board = self.create_board()

    def create_board(self):
        board = []
        colors = [WHITE, BLACK]
        for row in range(self.board_size):
            board_row = []
            for col in range(self.board_size):
                color = colors[(row + col) % 2]
                rect = pygame.Rect(col * self.square_size, row * self.square_size, self.square_size, self.square_size)
                board_row.append((rect, color))
            board.append(board_row)
        return board

    def draw(self, surface):
        for row in self.board:
            for rect, color in row:
                pygame.draw.rect(surface, color, rect)
