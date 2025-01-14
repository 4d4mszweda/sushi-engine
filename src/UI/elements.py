import pygame
from settings import *
import chess
import chess.svg
import pyperclip

class System:
    def __init__(self, fen="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"):
        self.board = chess.Board(fen)
        self.winner = ""
        self.captured_pieces_white, self.captured_pieces_black = self.get_missing_pieces()
        self.turn = True
        self.message = "TO JEST TESTOWA WIADOMOŚĆ"
        self.score = 0
        self.scroll_offset = 0
        self.scroll_speed = 20
        self.auto = False 
        self.selected_square = None
        self.possible_moves = []
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
        self.draw_possible_moves()

    def event_listener(self):
        for event in pygame.event.get():
            if event.type == pygame.QUIT:  
                return False
            elif event.type == pygame.MOUSEBUTTONDOWN:
                mouse_pos = event.pos
                self.handle_button_click(mouse_pos)
                self.handle_board_click(mouse_pos)
            if event.type == pygame.MOUSEWHEEL:
                self.scroll_offset += event.y * self.scroll_speed
                max_scroll_offset = max(0, (len(self.board.move_stack) // 2) * (FONT.get_height() + 5) - (SCREEN_HEIGHT - self.history_label_rect.bottom - 100))
                self.scroll_offset = max(min(self.scroll_offset, 0), -max_scroll_offset)
        return True

    def update_fen():
        return
    
    def update_pgn():
        return

    def get_missing_pieces(self):
        initial_pieces = {
            'P': 8, 'R': 2, 'N': 2, 'B': 2, 'Q': 1, 'K': 1,
            'p': 8, 'r': 2, 'n': 2, 'b': 2, 'q': 1, 'k': 1
        }
        current_pieces = {
            'P': 0, 'R': 0, 'N': 0, 'B': 0, 'Q': 0, 'K': 0,
            'p': 0, 'r': 0, 'n': 0, 'b': 0, 'q': 0, 'k': 0
        }

        for square in chess.SQUARES:
            piece = self.board.piece_at(square)
            if piece:
                current_pieces[piece.symbol()] += 1

        captured_pieces_white = []
        captured_pieces_black = []

        for piece, count in initial_pieces.items():
            if piece.isupper():
                missing_count = count - current_pieces[piece]
                captured_pieces_white.extend([piece] * missing_count)
            else:
                missing_count = count - current_pieces[piece]
                captured_pieces_black.extend([piece] * missing_count)

        return captured_pieces_white, captured_pieces_black

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

        self.fen_btn = pygame.Rect(hist_rect.centerx - button_width  // 2 - button_margin // 2, hist_rect.bottom - button_height - button_margin, button_width, button_height)
        fen_btn_hover = self.fen_btn.collidepoint(pygame.mouse.get_pos())
        self.draw_button(self.fen_btn, "FEN", self.fen_btn.collidepoint(pygame.mouse.get_pos()))

        self.auto_switch = pygame.Rect(hist_rect.centerx - button_width // 2, self.revert_btn.bottom + button_margin, button_width, button_height)
        auto_switch_hover = self.auto_switch.collidepoint(pygame.mouse.get_pos())
        self.draw_switch(self.auto_switch, self.auto)

        message_rect = pygame.Rect(hist_rect.left + button_margin, self.auto_switch.bottom + button_margin, hist_width - 2 * button_margin, button_height * 2)
        pygame.draw.rect(DISPLAYSURF, (0, 0, 0), message_rect)
        pygame.draw.rect(DISPLAYSURF, (250, 223, 22), message_rect, 2)
        self.draw_text_wrapped(self.message, message_rect, FONT, (255, 255, 0), margin=10)

        turn_text = "White's turn" if self.turn else "Black's turn"
        turn_rect = pygame.Rect(hist_rect.left + button_margin, message_rect.bottom + button_margin, hist_width - 2 * button_margin, button_height)
        turn_text_surface = FONT.render(turn_text, True, (0, 0, 0))
        DISPLAYSURF.blit(turn_text_surface, (turn_rect.left + 5, turn_rect.top + 5))

        self.history_label_rect = pygame.Rect(hist_rect.left + button_margin, turn_rect.bottom + button_margin, hist_width - 2 * button_margin, button_height)
        history_label_text = FONT.render("History:", True, (0, 0, 0))
        DISPLAYSURF.blit(history_label_text, (self.history_label_rect.left + 5, self.history_label_rect.top + 5))

        # Adjust the height of the history_rect to be smaller from the bottom
        history_rect_height = SCREEN_HEIGHT - self.history_label_rect.bottom - button_margin - 50  # Adjust the -50 to the desired height
        history_rect = pygame.Rect(hist_rect.left + button_margin, self.history_label_rect.bottom, hist_width - 2 * button_margin, history_rect_height)
        pygame.draw.rect(DISPLAYSURF, (255, 255, 255), history_rect)
        pygame.draw.rect(DISPLAYSURF, (0, 0, 0), history_rect, 2)

        # Create a surface for the history content
        history_surface = pygame.Surface((history_rect.width, history_rect.height))
        history_surface.fill((200, 200, 200))

        y_offset = 5 + self.scroll_offset
        move_index = 1
        move_text = ""
        for i, move in enumerate(self.board.move_stack):
            from_square = chess.square_name(move.from_square)
            to_square = chess.square_name(move.to_square)

            if i % 2 == 0:
                move_text = f"{move_index}. {from_square}{to_square}"
            else:
                move_text += f" {from_square}{to_square}"
                move_surface = FONT.render(move_text, True, (0, 0, 0))
                history_surface.blit(move_surface, (5, y_offset))
                y_offset += move_surface.get_height() + 5
                move_index += 1

        # If there is an odd number of moves, render the last move
        if len(self.board.move_stack) % 2 != 0:
            move_surface = FONT.render(move_text, True, (0, 0, 0))
            history_surface.blit(move_surface, (5, y_offset))

        # Blit the history surface onto the main display surface
        DISPLAYSURF.blit(history_surface, (history_rect.left, history_rect.top), (0, 0, history_rect.width, history_rect.height))
        if revert_btn_hover or fen_btn_hover or auto_switch_hover:
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
        elif self.auto_switch.collidepoint(mouse_pos):
            self.handle_switch()

    def handle_board_click(self, mouse_pos):
        if self.board_rect.collidepoint(mouse_pos):
            col = (mouse_pos[0] - self.board_rect.left) // (self.board_rect.width // 8)
            row = 7 - (mouse_pos[1] - self.board_rect.top) // (self.board_rect.width // 8)
            square = chess.square(col, row)
            piece = self.board.piece_at(square)
            
            if piece and ((self.turn and piece.color == chess.WHITE) or (not self.turn and piece.color == chess.BLACK)):
                if self.selected_square is not None and piece.color != self.board.piece_at(self.selected_square).color:
                    self.move_piece(self.selected_square, square)
                    self.selected_square = None
                    self.possible_moves = []
                else:
                    self.selected_square = square
                    self.possible_moves = [move for move in self.board.legal_moves if move.from_square == square]
            else:
                if self.selected_square is not None:
                    self.move_piece(self.selected_square, square)
                self.selected_square = None
                self.possible_moves = []

            print(f"Clicked on square: {chess.square_name(square)}")

    def draw_possible_moves(self):
        if self.selected_square is not None:
            square_size = self.board_rect.width // 8
            for move in self.possible_moves:
                to_square = move.to_square
                col = chess.square_file(to_square)
                row = 7 - chess.square_rank(to_square)
                x = self.board_rect.left + col * square_size
                y = self.board_rect.top + row * square_size
                pygame.draw.circle(DISPLAYSURF, LIGHT_GREEN, (x + square_size // 2, y + square_size // 2), square_size // 6)

    def draw_pieces(self):
        square_size = self.board_rect.width // 8
        for row in range(8):
            for col in range(8):
                piece = self.board.piece_at(chess.square(col, 7 - row))
                if piece:
                    x = self.board_rect.left + col * square_size
                    y = self.board_rect.top + row * square_size
                    DISPLAYSURF.blit(pygame.transform.scale(self.images[str(piece)], (square_size, square_size)), (x, y))
        
    def move_piece(self, from_square, to_square):
        move = chess.Move(from_square, to_square)
        if move in self.board.legal_moves:
            captured_piece = self.board.piece_at(to_square)
            if captured_piece:
                if captured_piece.color == chess.WHITE:
                    self.captured_pieces_black.append(captured_piece.symbol())
                else:
                    self.captured_pieces_white.append(captured_piece.symbol())
            self.board.push(move)
            self.turn = not self.turn
            self.selected_square = None
            self.possible_moves = []
            print(f"Moved piece from {chess.square_name(from_square)} to {chess.square_name(to_square)}")
        else:
            print(f"Illegal move from {chess.square_name(from_square)} to {chess.square_name(to_square)}")

    def handle_revert_move(self):
        if self.board.move_stack:
            last_move = self.board.pop()
            self.turn = not self.turn
            self.selected_square = None
            self.possible_moves = []
            self.message = f"Reverted move: {last_move}"
        else:
            self.message = "No moves to revert!"

    def handle_fen(self):
        try:
            fen = pyperclip.paste()
            self.board.set_fen(fen)
            
            self.turn = self.board.turn
            self.selected_square = None
            self.possible_moves = list(self.board.legal_moves)
            self.captured_pieces_white, self.captured_pieces_black = self.get_missing_pieces()
            
            self.message = f"FEN updated to: {fen}"
        except Exception as e:
            self.message = f"Failed to update FEN: {str(e)}"

    def handle_switch(self):
        self.auto = not self.auto
        if self.auto: 
            self.message = "Enemy Engine is on"
        else:
            self.message = "Enemy Engine is off"

    def eat_sushi(self):
        return
