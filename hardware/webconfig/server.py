from flask import Flask, json, render_template

app = Flask(__name__, template_folder=".")

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/save", methods=["POST"])
def saveSetting():
    return json.jsonify(result="success")

@app.route("/restart")
def restart():
    return json.jsonify(result="success")

app.run(debug=True)