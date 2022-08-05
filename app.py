from flask import Flask
from flask import render_template
from flask import request
import chess
import chess.engine

engine = chess.engine.SimpleEngine.popen_uci('./engine/crabstar-ubuntu')

board = chess.Board()

app = Flask(__name__)

# root route
@app.route('/')
def root():
    return render_template('crabstar.html')

# make move api
@app.route('/make_move', methods=['POST'])
def make_move():
    print("hello")
    fen = request.form.get('fen')
    print('fen:', fen)

    # find best move
    board = chess.Board(fen)
    result = engine.play(board, chess.engine.Limit(depth=6))
    board.push(result.move)
    best_move = str(result.move)

    return {'fen': board.fen(), 'best_move': best_move}

# if __name__ == '__main__':
#     app.run(debug=True, threaded=True)
