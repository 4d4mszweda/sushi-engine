import pygame
from enum import Enum
from settings import WHITE, BLACK 

class PieceType(Enum):
    KING = 'king'
    QUEEN = 'queen'
    ROOK = 'rook'
    BISHOP = 'bishop'
    KNIGHT = 'knight'
    PAWN = 'pawn'

class ChessPiece:
    def __init__(self, piece_type, side, position):
        self.piece_type = piece_type
        # self.color = color
        # TODO switch po PieceType
        self.position = position
        self.image = self.load_image()

    def load_image(self):
        image_path = f'images/{self.color}_{self.piece_type.value}.png'
        return pygame.image.load(image_path)

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