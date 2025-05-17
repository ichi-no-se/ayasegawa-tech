import subprocess
import time
from pathlib import Path
import statistics


def validate_output(input_text, output_text):
    def parse_board(text, allow_question=False):
        lines = text.strip().splitlines()
        if len(lines) != 9 or any(len(line.strip()) != 9 for line in lines):
            return None  # フォーマット不正

        board = []
        for line in lines:
            row = []
            for ch in line.strip():
                if ch in "123456789":
                    row.append(ch)
                elif ch == "?" and allow_question:
                    row.append("?")
                else:
                    return None  # 不正な文字
            board.append(row)
        return board

    def is_valid_solution(board):
        def is_valid_unit(unit):
            return sorted(unit) == list("123456789")

        # 行・列
        for i in range(9):
            row = [board[i][j] for j in range(9)]
            col = [board[j][i] for j in range(9)]
            if not is_valid_unit(row) or not is_valid_unit(col):
                return False

        # ブロック
        for bi in range(0, 9, 3):
            for bj in range(0, 9, 3):
                block = [board[bi + i][bj + j] for i in range(3) for j in range(3)]
                if not is_valid_unit(block):
                    return False

        return True

    def is_consistent(input_board, output_board):
        for i in range(9):
            for j in range(9):
                if input_board[i][j] in "123456789":
                    if input_board[i][j] != output_board[i][j]:
                        return False
        return True

    # パース
    input_board = parse_board(input_text, allow_question=True)
    output_board = parse_board(output_text, allow_question=False)

    if input_board is None or output_board is None:
        return False  # フォーマット不正

    if not is_consistent(input_board, output_board):
        return False  # 入力で指定されていた数字が保持されていない

    if not is_valid_solution(output_board):
        return False  # 数独としての正しさを満たしていない

    return True


input_dir = Path("./inputs")
input_files = sorted(input_dir.glob("*.txt"))

command = ["./solver"]

for input_file in input_files:
    with open(input_file, "r") as f:
        input_text = f.read()
    print(f"Running for: {input_file.name}")
    times = []
    for _ in range(10):
        start = time.perf_counter()
        proc = subprocess.run(
            command,
            input=input_text.encode("utf-8"),
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
        )
        end = time.perf_counter()
        elapsed = end - start
        times.append(elapsed)
    median=statistics.median(times)

    output_text = proc.stdout.decode("utf-8")
    valid = validate_output(input_text, output_text)

    print(f"  Time: {median:.5f} sec  |  Valid: {'✔' if valid else '✘'}")
