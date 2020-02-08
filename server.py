from flask import Flask, request, jsonify
from runtime import SmartReplyRuntime

model = SmartReplyRuntime()
app = Flask("__main__")

@app.route("/api/inference", methods = ['POST'])
def inference():
    data = request.get_json()
    if not 'input' in data :
        return jsonify({
            "success" : False,
            "message" : "Invalid JSON, key input not found"
        })
    
    if type(data['input']) == list :
        result =  model.predictMultiple(data['input'])
        return jsonify({
            "success" : True,
            "result" : result
        })
    
    if type(data['input']) == str :
        result = model.predict(data['input'])
        return jsonify({
            "success" : True,
            "result" : result
        })
    
    return jsonify({
        "success" : False,
        "result" : "Invalid result"
    })


app.run()