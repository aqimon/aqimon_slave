from flask import Flask, json, render_template
import time

app = Flask(__name__, template_folder=".")

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/2")
def index2():
    return render_template("index2.html")

@app.route("/3")
def index3():
    return render_template("index3.html")

@app.route("/restart")
def restart():
    return json.jsonify(result="success")

@app.route("/save", methods=["POST"])
def saveSetting():
    time.sleep(1)
    return json.jsonify(result="success")

@app.route("/currSetting")
def setting():
    return json.jsonify(ip="111.111.111.111", subnet="111.111.111.111", gateway="111.111.111.111",
                        ssid="111.111.111.111", password="xxxxxxxxxxx", host="e3.xxxx",
                        port=80, clientID="xxxxxxxxxxxxxxxxxxx", apiKey="xxxxxxxxxxxxx", static=1)

app.run(debug=True)