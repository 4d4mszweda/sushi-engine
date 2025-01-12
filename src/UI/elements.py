import pygame
from settings import *
import chess
import chess.svg

class System:
    def __init__(self, fen="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", pgn=""):
        self.winner = ""
        self.captured_pieces_white, self.captured_pieces_black = self.get_missing_pieces(fen, pgn)
        self.turn = True
        self.pgn = pgn
        self.fen = fen
        self.message = "TO JEST TESTOWA WIADOMOŚĆ"
        self.images = {}
        self.auto = False
        self.board = chess.Board(fen)
        print(self.board.board_fen())
        print(self.board.turn)
        print(self.board.castling_rights)
        print(self.board.ep_square)
        print(self.board.halfmove_clock)
        print(self.board.fullmove_number)
        self.load_images()
        return
    
    def update_fen():
        return
    
    def update_pgn():
        return

    def get_missing_pieces(self, fen, pgn):
        # if pgn == "" and fen == "":
        #     return [], []
        return ["pawn","knight","bishop"], ["pawn","rook","pawn","queen"]

    def load_images(self):
        pieces = ['pawn', 'rook', 'knight', 'bishop', 'queen', 'king']
        colors = ['w', 'b']
        for color in colors:
            for piece in pieces:
                self.images[f'{color}_{piece}'] = pygame.image.load(f'resources/pieces/{color}_{piece}.png')
    
    def draw_score(self, score=0):
        score_width = SCREEN_WIDTH // 8
        score_rect = pygame.Rect(0, 0, score_width, SCREEN_HEIGHT)
        pygame.draw.rect(DISPLAYSURF, (200, 200, 200), score_rect)

        thermometer_width = score_width // 2
        thermometer_height = SCREEN_HEIGHT - SCREEN_HEIGHT // 10
        thermometer_x = (score_width - thermometer_width) // 2
        thermometer_y = (SCREEN_HEIGHT - thermometer_height) // 2
        half_height = thermometer_height // 2

        if score > 0:
            white_height = half_height + (score / 100) * half_height # wymień ten wskaźnik na inny w zależności jak wygląda score
            black_height = thermometer_height - white_height
        elif score < 0:
            black_height = half_height + (abs(score) / 100) * half_height # wymień ten wskaźnik na inny w zależności jak wygląda score
            white_height = thermometer_height - black_height
        else:
            white_height = half_height
            black_height = half_height

        black_rect = pygame.Rect(thermometer_x, thermometer_y, thermometer_width, black_height)
        pygame.draw.rect(DISPLAYSURF, (0, 0, 0), black_rect)

        white_rect = pygame.Rect(thermometer_x, thermometer_y + black_height, thermometer_width, white_height)
        pygame.draw.rect(DISPLAYSURF, (255, 255, 255), white_rect)

        score_text = FONT.render(f'Score: {score}', True, (0, 0, 0))
        DISPLAYSURF.blit(score_text, (thermometer_x - score_width // 7, thermometer_y + thermometer_height + 10))

    def draw_board(self):
        left_boundry_width = SCREEN_WIDTH // 8
        right_boundry_width = SCREEN_WIDTH // 4
        board_width = SCREEN_WIDTH - left_boundry_width - right_boundry_width
        board_rect = pygame.Rect(left_boundry_width, 0, board_width, SCREEN_HEIGHT)
        pygame.draw.rect(DISPLAYSURF, (200, 200, 200), board_rect)

        pieces_text = FONT.render("Pieces:", True, (0, 0, 0))
        DISPLAYSURF.blit(pieces_text, (board_rect.left + 5, board_rect.top + 10))
        DISPLAYSURF.blit(pieces_text, (board_rect.left + 5, board_rect.bottom - pieces_text.get_height() - 10)) 

        # Wyświetlanie podobizn figur z captured_pieces_black
        x_offset = board_rect.left + pieces_text.get_width() + board_width // 50
        y_offset = board_rect.top + 8
        for piece in self.captured_pieces_black:
            image_key = f'w_{piece}'
            if image_key in self.images:
                DISPLAYSURF.blit(pygame.transform.scale(self.images[image_key], (25,25)), (x_offset, y_offset))
                x_offset += 10 + board_width // 50

        # Wyświetlanie podobizn figur z captured_pieces_white
        x_offset = board_rect.left + pieces_text.get_width() + board_width // 50
        y_offset = board_rect.bottom - pieces_text.get_height() - 12
        for piece in self.captured_pieces_white:
            image_key = f'b_{piece}'
            if image_key in self.images:
                DISPLAYSURF.blit(pygame.transform.scale(self.images[image_key], (25,25)), (x_offset, y_offset))
                x_offset += 10 + board_width // 50

        square_size = board_width // 8
        for row in range(8):
            for col in range(8):
                color = (209, 74, 65) if (row + col) % 2 == 0 else (48, 48, 48)
                square_rect = pygame.Rect(left_boundry_width + col * square_size, SCREEN_HEIGHT // 19 + row * square_size, square_size, square_size)
                pygame.draw.rect(DISPLAYSURF, color, square_rect)
                pygame.draw.rect(DISPLAYSURF, (250, 223, 22), square_rect, 1)
                                
                if row == 7:
                    letter_text = FONT.render(chr(ord('a') + col), True, (179, 179, 179))
                    DISPLAYSURF.blit(letter_text, (left_boundry_width + col * square_size + square_size - letter_text.get_width() - 4, row * square_size + square_size + 14))

                if col == 0:
                    number_text = FONT.render(str(8 - row), True, (179, 179, 179))
                    DISPLAYSURF.blit(number_text, (left_boundry_width + 4 + col * square_size, row * square_size + square_size // 2 ))

    def draw_text_wrapped(self, text, rect, font, color, margin=5):
        words = text.split(' ')
        lines = []
        current_line = ""
        for word in words:
            test_line = current_line + word + " "
            if font.size(test_line)[0] <= rect.width - 2 * margin:
                current_line = test_line
            else:
                lines.append(current_line)
                current_line = word + " "
        lines.append(current_line)

        y_offset = rect.top + margin
        for line in lines:
            line_surface = font.render(line, True, color)
            DISPLAYSURF.blit(line_surface, (rect.left + margin, y_offset))
            y_offset += font.get_height()

    def draw_history(self):
        hist_width = SCREEN_WIDTH // 4
        hist_rect = pygame.Rect(SCREEN_WIDTH - hist_width, 0, hist_width, SCREEN_HEIGHT)
        pygame.draw.rect(DISPLAYSURF, (200, 200, 200), hist_rect)

        button_height = SCREEN_HEIGHT // 20
        button_width = hist_width - hist_width // 3
        button_margin = 10

        self.revert_btn = pygame.Rect(hist_rect.centerx - button_width // 2, hist_rect.top + button_margin, button_width, button_height)
        revert_btn_hover = self.revert_btn.collidepoint(pygame.mouse.get_pos())
        self.draw_button(self.revert_btn, "Revert move", self.revert_btn.collidepoint(pygame.mouse.get_pos()))

        self.fen_btn = pygame.Rect(hist_rect.centerx - button_width  // 2 - button_margin // 2, hist_rect.bottom - button_height - button_margin, button_width  // 2, button_height)
        self.png_btn = pygame.Rect(hist_rect.centerx + button_margin // 2, hist_rect.bottom - button_height - button_margin, button_width // 2, button_height)
        fen_btn_hover = self.fen_btn.collidepoint(pygame.mouse.get_pos())
        png_btn_hover = self.png_btn.collidepoint(pygame.mouse.get_pos())
        self.draw_button(self.fen_btn, "FEN", self.fen_btn.collidepoint(pygame.mouse.get_pos()))
        self.draw_button(self.png_btn, "PNG", self.png_btn.collidepoint(pygame.mouse.get_pos()))

        self.auto_switch = pygame.Rect(hist_rect.centerx - button_width // 2, self.revert_btn.bottom + button_margin, button_width, button_height)
        auto_switch_hover = self.auto_switch.collidepoint(pygame.mouse.get_pos())
        self.draw_switch(self.auto_switch, self.auto)

        message_rect = pygame.Rect(hist_rect.left + button_margin, self.auto_switch.bottom + button_margin, hist_width - 2 * button_margin, button_height * 2)
        pygame.draw.rect(DISPLAYSURF, (0, 0, 0), message_rect)
        pygame.draw.rect(DISPLAYSURF, (250, 223, 22), message_rect, 2)
        self.draw_text_wrapped(self.message, message_rect, FONT, (255, 255, 0), margin=10)

        history_label_rect = pygame.Rect(hist_rect.left + button_margin, message_rect.bottom + button_margin, hist_width - 2 * button_margin, button_height)
        history_label_text = FONT.render("History:", True, (0, 0, 0))
        DISPLAYSURF.blit(history_label_text, (history_label_rect.left + 5, history_label_rect.top + 5))

        if revert_btn_hover or fen_btn_hover or png_btn_hover or auto_switch_hover:
            pygame.mouse.set_cursor(pygame.SYSTEM_CURSOR_HAND)
        else:
            pygame.mouse.set_cursor(pygame.SYSTEM_CURSOR_ARROW)

    def draw_button(self, rect, text, hover):
        color = (209, 74, 65) if hover else (48, 48, 48)
        pygame.draw.rect(DISPLAYSURF, color, rect)
        button_text = FONT.render(text, True, (255, 255, 255))
        DISPLAYSURF.blit(button_text, (rect.centerx - button_text.get_width() // 2, rect.centery - button_text.get_height() // 2))
        pygame.draw.rect(DISPLAYSURF, (250, 223, 22), rect, 2)

    def draw_switch(self, rect, state):
        switch_color = (33, 166, 0) if state else (181, 28, 11)
        pygame.draw.rect(DISPLAYSURF, switch_color, rect)
        switch_text = FONT.render("ON" if state else "OFF", True, (255, 255, 255))
        DISPLAYSURF.blit(switch_text, (rect.centerx - switch_text.get_width() // 2, rect.centery - switch_text.get_height() // 2))

    def handle_button_click(self, mouse_pos):
        if self.revert_btn.collidepoint(mouse_pos):
            self.handle_revert_move()
        elif self.fen_btn.collidepoint(mouse_pos):
            self.handle_fen()
        elif self.png_btn.collidepoint(mouse_pos):
            self.handle_png()
        elif self.auto_switch.collidepoint(mouse_pos):
            self.handle_switch()

    def handle_revert_move(self):
        print("Revert move button clicked!")

    def handle_fen(self):
        print("FEN button clicked!")

    def handle_png(self):
        print("PNG button clicked!")

    def handle_switch(self):
        self.auto = not self.auto
        print(f"Switch state: {'ON' if self.auto else 'OFF'}")

    def eat_sushi(self):
        return
