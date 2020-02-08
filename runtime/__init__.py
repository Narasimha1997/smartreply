from runtime.lib import smartreply
import os

DEFAULT_MODEL_PATH = os.path.join(os.path.dirname(__file__), "model/smartreply.tflite")

class SmartReplyRuntime :

    def __init__(self, model_file = DEFAULT_MODEL_PATH):
        smartreply.load(model_file)
    
    def predict(self, text):
        if not type(text) == str :
            print('Attribute text must be a string')
            return {} 
        
        return smartreply.predict(text)
    
    def predictMultiple(self, texts, batch = False) :
        if not type(texts) == list :
            print('Attribute texts must be a list of strings')
            return []
        
        if not batch : 
            return smartreply.multi_predict(texts)

        return smartreply.batch_predict(texts)
