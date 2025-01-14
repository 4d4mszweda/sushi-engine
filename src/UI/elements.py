import pygame
from settings import *
import chess
import chess.svg

class System:
    def __init__(self, fen="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", pgn=""):
        self.pgn = pgn
        self.board = chess.Board(fen)
        self.winner = ""
        self.captured_pieces_white, self.captured_pieces_black = self.get_missing_pieces()
        self.turn = True
        self.message = "TO JEST TESTOWA WIADOMOŚĆ"
        self.score = 0
        self.auto = False
        self.images = {
            'r': pygame.image.load('src/UI/resources/pieces/b_rook.png'),
            'n': pygame.image.load('src/UI/resources/pieces/b_knight.png'),
            'b': pygame.image.load('src/UI/resources/pieces/b_bishop.png'),
            'q': pygame.image.load('src/UI/resources/pieces/b_queen.png'),
            'k': pygame.image.load('src/UI/resources/pieces/b_king.png'),
            'p': pygame.image.load('src/UI/resources/pieces/b_pawn.png'),
            'R': pygame.image.load('src/UI/resources/pieces/w_rook.png'),
            'N': pygame.image.load('src/UI/resources/pieces/w_knight.png'),
            'B': pygame.image.load('src/UI/resources/pieces/w_bishop.png'),
            'Q': pygame.image.load('src/UI/resources/pieces/w_queen.png'),
            'K': pygame.image.load('src/UI/resources/pieces/w_king.png'),
            'P': pygame.image.load('src/UI/resources/pieces/w_pawn.png')
        }
    
    def update(self):
        self.draw_score()
        self.draw_board()
        self.draw_history()
        self.draw_pieces()

    def event_listener(self):
        for event in pygame.event.get():
            if event.type == pygame.QUIT:  
                return False
            elif event.type == pygame.MOUSEBUTTONDOWN:
                mouse_pos = event.pos
                self.handle_button_click(mouse_pos)
        return True

    def update_fen():
        return
    
    def update_pgn():
        return

    def get_missing_pieces(self):
        # if fen == "":
        #     return [], []
        return ["p","n","b"], ["P","R","P","Q"]

    def draw_score(self):
        score_width = SCREEN_WIDTH // 8
        score_rect = pygame.Rect(0, 0, score_width, SCREEN_HEIGHT)
        pygame.draw.rect(DISPLAYSURF, (200, 200, 200), score_rect)

        thermometer_width = score_width // 2
        thermometer_height = SCREEN_HEIGHT - SCREEN_HEIGHT // 10
        thermometer_x = (score_width - thermometer_width) // 2
        thermometer_y = (SCREEN_HEIGHT - thermometer_height) // 2
        half_height = thermometer_height // 2

        if self.score > 0:
            white_height = half_height + (self.score / 100) * half_height # wymień ten wskaźnik na inny w zależności jak wygląda score
            black_height = thermometer_height - white_height
        elif self.score < 0:
            black_height = half_height + (abs(self.score) / 100) * half_height # wymień ten wskaźnik na inny w zależności jak wygląda score
            white_height = thermometer_height - black_height
        else:
            white_height = half_height
            black_height = half_height

        black_rect = pygame.Rect(thermometer_x, thermometer_y, thermometer_width, black_height)
        pygame.draw.rect(DISPLAYSURF, (0, 0, 0), black_rect)

        white_rect = pygame.Rect(thermometer_x, thermometer_y + black_height, thermometer_width, white_height)
        pygame.draw.rect(DISPLAYSURF, (255, 255, 255), white_rect)

        score_text = FONT.render(f'Score: {self.score}', True, (0, 0, 0))
        DISPLAYSURF.blit(score_text, (thermometer_x - score_width // 7, thermometer_y + thermometer_height + 10))

    def draw_board(self):
        main_rect = pygame.Rect(SCREEN_WIDTH // 8, 0, SCREEN_WIDTH - SCREEN_WIDTH // 8 - SCREEN_WIDTH // 4, SCREEN_HEIGHT)
        pygame.draw.rect(DISPLAYSURF, (200, 200, 200), main_rect)

        self.board_rect = pygame.Rect(main_rect.left, main_rect.top + main_rect.height // 20, main_rect.width, int(main_rect.height * 0.9))

        pieces_text = FONT.render("Pieces:", True, (0, 0, 0))
        DISPLAYSURF.blit(pieces_text, (main_rect.left + 5, main_rect.top + 10))
        DISPLAYSURF.blit(pieces_text, (main_rect.left + 5, main_rect.bottom - pieces_text.get_height() - 10)) 

        # Wyświetlanie podobizn figur z captured_pieces_black
        x_offset = main_rect.left + pieces_text.get_width() + main_rect.width // 50
        y_offset = main_rect.top + 6
        for piece in self.captured_pieces_black:
            if piece in self.images:
                DISPLAYSURF.blit(pygame.transform.scale(self.images[piece], (25,25)), (x_offset, y_offset))
                x_offset += 10 + main_rect.width // 50

        # Wyświetlanie podobizn figur z captured_pieces_white
        x_offset = main_rect.left + pieces_text.get_width() + main_rect.width // 50
        y_offset = main_rect.bottom - pieces_text.get_height() - 12
        for piece in self.captured_pieces_white:
            if piece in self.images:
                DISPLAYSURF.blit(pygame.transform.scale(self.images[piece], (25,25)), (x_offset, y_offset))
                x_offset += 10 + main_rect.width // 50

        square_size = self.board_rect.width // 8
        for row in range(8):
            for col in range(8):
                color = MAIN_BOARD if (row + col) % 2 == 0 else SECOND_BOARD
                square_rect = pygame.Rect(self.board_rect.left + col * square_size, SCREEN_HEIGHT // 19 + row * square_size, square_size, square_size)
                pygame.draw.rect(DISPLAYSURF, color, square_rect)
                # pygame.draw.rect(DISPLAYSURF, BLACK, square_rect, 1)
                                
                if row == 7:
                    letter_text = FONT.render(chr(ord('a') + col), True, WHITE)
                    DISPLAYSURF.blit(letter_text, (self.board_rect.left + col * square_size + square_size - letter_text.get_width() - 4, row * square_size + square_size + 14))

                if col == 0:
                    number_text = FONT.render(str(8 - row), True, WHITE)
                    DISPLAYSURF.blit(number_text, (self.board_rect.left + 4 + col * square_size, row * square_size + square_size // 2 ))

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

    def draw_pieces(self):
        square_size = self.board_rect.width // 8
        for row in range(8):
            for col in range(8):
                piece = self.board.piece_at(chess.square(col, 7 - row))
                if piece:
                    x = self.board_rect.left + col * square_size
                    y = self.board_rect.top + row * square_size
                    DISPLAYSURF.blit(pygame.transform.scale(self.images[str(piece)], (square_size, square_size)), (x, y))
        
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
