from runtime import SmartReplyRuntime

#create SmartReplyRuntime() object, you can specify your own model path as an argument, or default provided 
#model will be used, ex : SmartReplyRuntime('mymodel.tflite')
rt = SmartReplyRuntime()

#Prediction on a single string
single = rt.predict("hi")
print(single)

#Prediction on multiple strings, runs in a loop
multi = rt.predictMultiple(["hello", "i am happy", "great"])
print(multi)

#Prediction on multiple strings, exploits batching capability of tflite
multiBatch = rt.predictMultiple(["hello", "i am happy", "see me tomorrow"], batch = True)
print(multiBatch)
