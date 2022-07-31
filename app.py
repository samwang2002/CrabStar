from flask import Flask
from flask import render_template

app = Flask(__name__)

@app.route('/')
def root():
    return render_template('crabstar.html')

if __name__ == '__main__':
    app.run(debug=True, threaded=True, host='localhost', port=4000)