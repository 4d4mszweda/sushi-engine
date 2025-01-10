import subprocess
import os


class ChessEngine:
    def __init__(self):
        script_dir = os.path.dirname(os.path.abspath(__file__))
        engine_path = os.path.join(script_dir, "../chess.exe")
        self.process = subprocess.Popen(
            [engine_path],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            bufsize=1,
        )

    def send_command(self, command):
        self.process.stdin.write(command + "\n")
        self.process.stdin.flush()

    def read_output(self):
        output = []
        while True:
            line = self.process.stdout.readline()
            if line.strip() == "readyok" or line == "":
                break
            output.append(line.strip())
        return "\n".join(output)

    def start_new_game(self):
        self.send_command("ucinewgame")
        self.send_command("isready")
        self.read_output()

    def set_position(self, fen):
        self.send_command(f"position fen {fen}")
        self.send_command("isready")
        self.read_output()

    def go(self):
        self.send_command("go")
        self.send_command("isready")
        score, target, dest, prom = self.read_output().split()
        if prom.isalpha():
            return [int(score), target + dest + prom]
        else:
            return [int(score), target + dest]

    def quit(self):
        self.send_command("quit")
        self.process.stdin.close()
        self.process.stdout.close()
        self.process.terminate()


if __name__ == "__main__":

    engine = ChessEngine()

    try:
        engine.start_new_game()
        fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
        engine.set_position(fen)
        print(engine.go())
    finally:
        engine.quit()
