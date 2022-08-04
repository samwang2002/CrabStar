from flask import Flask
from flask import render_template
from flask import request
import chess
import chess.engine

engine = chess.engine.SimpleEngine.popen_uci('./engine/crabstar2')
# engine = chess.uci.popen_engine("./engine/crabstar")

board = chess.Board()
# while not board.is_game_over():
#     result = engine.play(board, chess.engine.Limit(time=0.1))
#     board.push(result.move)

# engine.quit()

app = Flask(__name__)

# root route
@app.route('/')
def root():
    return render_template('crabstar.html')

# make move api
@app.route('/make_move', methods=['POST'])
def make_move():
    fen = request.form.get('fen')
    print('fen:', fen)

    # find best move
    board = chess.Board(fen)
    result = engine.play(board, chess.engine.Limit(depth=6))
    print('\n', result.move, '\n')
    board.push(result.move)

    return {'fen': board.fen()}

if __name__ == '__main__':
    app.run(debug=True, threaded=True, host='localhost', port=4000)