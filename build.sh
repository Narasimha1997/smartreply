#!/bin/bash

bazel build libs/cc:smartreply.cpython-36m-x86_64-linux-gnu.so
mkdir runtime/lib 
cp bazel-bin/libs/cc/smartreply.cpython-36m-x86_64-linux-gnu.so runtime/lib/
bazel clean
rmdir bazel-bin/ bazel-out/ bazel-smartreply/ bazel-testlogs