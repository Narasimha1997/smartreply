from runtime import SmartReplyRuntime

rt = SmartReplyRuntime()

single = rt.predict("hi")
print(single)

multi = rt.predictMultiple(["hello", "i am happy", "great"])
print(multi)

multiBatch = rt.predictMultiple(["hello", "i am happy", "see me tomorrow"], batch = True)
print(multiBatch)