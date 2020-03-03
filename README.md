# smartreply
Unofficial port of Google's smart reply runtime (powers gmail and assistant) model to python, allowing developers to leverage intelligent smart reply as an API in Web and embedded systems that supports Linux, a loader (ld.so), a fully POSIX C++ Runtime and Python interpreter.

### How it works ?
Recently Google released a smart-reply on-device TFLite model, this model has smaller memory footprint and runs on Android Platform. This model powers on-device
smart-reply applications. I made an attempt to run the same model on Linux machine, however it failed because the model required a runtime and a set of plugins that does
pre and post-processing tasks, the framework that supported the runtime was built with C++ and was compiled into an android archive, the nature of the framework made it
impossible to run on desktop environments, and it was almost impossible to use it with python by traditional methods. So I thought of making a wrapper library
on top of Google's smart-reply framework using **PyBind**, later the wrapper was compiled with existing TFlite smart-reply framework to create a 
stand-alone CPython shared library, This shared library only requires a fully functional Linux Operating system. The dependencies of this library is listed below :
```
	linux-vdso.so.1 (0x00007ffc83fa9000)
	libstdc++.so.6 => /usr/lib/x86_64-linux-gnu/libstdc++.so.6 (0x00007f49062cd000)
	libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007f4905f2f000)
	libpthread.so.0 => /lib/x86_64-linux-gnu/libpthread.so.0 (0x00007f4905d10000)
	libdl.so.2 => /lib/x86_64-linux-gnu/libdl.so.2 (0x00007f4905b0c000)
	librt.so.1 => /lib/x86_64-linux-gnu/librt.so.1 (0x00007f4905904000)
	libgcc_s.so.1 => /lib/x86_64-linux-gnu/libgcc_s.so.1 (0x00007f49056ec000)
	libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f49052fb000)
	/lib64/ld-linux-x86-64.so.2 (0x00007f49069f6000)
```
Any platform that provides these libraries can run the pre-built smart-reply python runtime. See `libs/cc` for Google's Smart-reply framework code
and the wrapper `inference.cc` I wrote to wrap the framework, the codebase has `PyBind11` as its dependency and uses `bazel` as build system.

### Direct run on x86_64 architecture :
If you are deploying on a x86_64 machine, you can directly utilize the pre-built shared library provided in the repo, by importing `smartreply` python module.
To use smart-reply model : 
```python
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
```

To run it inside a web-server, run `server.py`, this creates a web server instance at port `5000`, Run inference by making a JSON 
POST Request :
```
curl -d '{"input" : "Hello"}' -H "Content-Type:application/json" http://localhost:5000/api/inference
```
Response :
```
{"result":{"Hi, how are you doing?":1.0899782180786133,"How are you sir?":1.4489225149154663,"I do not understand":1.1177111864089966,"No pictures":0.4019201695919037,"Sending now":0.4459223747253418,"So how did it go?":1.0521267652511597},"success":true}
```
To infer multiple strings :
```
curl -d '{"input" : ["Hello", "hi", "I am happy"]}' -H "Content-Type:application/json" http://localhost:5000/api/inference
```
A dockerfile is provided for those who want to make it production ready, stateless inference container, build the docker in a normal way and run it as :
```
sudo docker run --rm -ti --net=host  smartreply_rt
```
You can disable host-networking mode by providing a port mapping using `-p` option and attach the flask server on `0.0.0.0` by modifying `server.py`.

### Building on your own 
If you want to build on your own, you can start by setting up the build environment, Install bazel, and pybind11 and follow the steps :

1. ##### Get pybind11 header and python header paths : 
```
python3 -m pybind11 --includes
```
It outputs the include paths where `Python.h` and `pybind11` headers are located. On my machine :
```
-I/usr/include/python3.6m -I/usr/local/include/python3.6 -I/home/narasimha/.local/include/python3.6m
```
2. ##### Create Python header symbolic-links :
Bazel build systems can smoothly include headers that are present inside its workspace, including external dependencies can be tricky.
So I created a symbolic links of these headers inside bazel workspace. See `libs/cc/ln.sh` to create symbolic links.
```
mkdir pydep

ln -s /usr/include/python3.6m pydep/python3.6m
ln -s /usr/local/include/python3.6 pydep/python3.6
ln -s $HOME/.local/include/python3.6m pydep/python3.6m_i
```
Run these commands insde `libs/cc` directory. This should create a directory called `pydep` and place all `python3` and `pybind11` dependencies there.

3. ##### Build with bazel :
You can simply run `build.sh`
This should build the `smartreply.cpython-36m-x86_64-linux-gnu.so` shared library and place it in `runtime/lib` directory. This library is loaded 
by Python runtime since it is a CPython extension. The contents of build.sh are as follows :
```shell
bazel build libs/cc:smartreply.cpython-36m-x86_64-linux-gnu.so
mkdir runtime/lib 
cp bazel-bin/libs/cc/smartreply.cpython-36m-x86_64-linux-gnu.so runtime/lib/
bazel clean
rmdir bazel-bin/ bazel-out/ bazel-smartreply/ bazel-testlogs
```
