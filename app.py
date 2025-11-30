import os
import threading
from flask import Flask, render_template, request, jsonify
import chess
import chess.engine

app = Flask(__name__)

ENGINE_PATH = os.path.abspath("./main")
_engine = None
_engine_lock = threading.Lock()


def get_engine():
    global _engine

    if _engine is None:
        with _engine_lock:
            if _engine is None:
                if not os.path.exists(ENGINE_PATH):
                    raise RuntimeError(f"Engine binary not found at {ENGINE_PATH}")

                _engine = chess.engine.SimpleEngine.popen_uci(ENGINE_PATH)
    return _engine


@app.route("/")
def root():
    return render_template("crabstar.html")


@app.route("/make_move", methods=["POST"])
def make_move():
    try:
        engine = get_engine()

        fen = request.form.get("fen")
        weight = float(request.form.get("weight", 0.1))

        board = chess.Board(fen)
        result = engine.play(board, chess.engine.Limit(depth=6, time=weight))

        board.push(result.move)

        return jsonify({
            "fen": board.fen(),
            "best_move": str(result.move)
        })

    except chess.engine.EngineTerminatedError:
        return jsonify({"error": "Engine process crashed."}), 500

    except Exception as e:
        return jsonify({"error": str(e)}), 500


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=int(os.environ.get("PORT", 5000)))
