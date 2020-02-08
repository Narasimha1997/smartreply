package(
    default_visibility = ["//visibility:public"],
)

licenses(["notice"])  # Apache 2.0

load("@build_bazel_rules_android//android:rules.bzl", "android_library")
load("@org_tensorflow//tensorflow:tensorflow.bzl", "tf_cc_test")
load(
    "@org_tensorflow//tensorflow/lite:build_def.bzl",
    "gen_selected_ops",
    "tflite_copts",
    "tflite_jni_binary",
)
load("@org_tensorflow//tensorflow/lite/java:aar_with_jni.bzl", "aar_with_jni")

gen_selected_ops(
    name = "smartreply_ops",
    model = "//libs/cc/testdata:smartreply.tflite",
)

cc_library(
    name = "custom_ops",
    srcs = [
        "ops/extract_feature.cc",
        "ops/normalize.cc",
        "ops/predict.cc",
        ":smartreply_ops",
    ],
    copts = tflite_copts(),
    deps = [
        "@org_tensorflow//tensorflow/lite:framework",
        "@org_tensorflow//tensorflow/lite:string_util",
        "@org_tensorflow//tensorflow/lite/kernels:builtin_ops",
        "@org_tensorflow//tensorflow/lite/kernels:kernel_util",
        "@com_google_absl//absl/strings",
        "@com_googlesource_code_re2//:re2",
        "@farmhash_archive//:farmhash",
    ],
    alwayslink = 1,
)

cc_library(
    name = "predictor_lib",
    srcs = ["predictor.cc"],
    hdrs = ["predictor.h"],
    copts = tflite_copts(),
    deps = [
        ":custom_ops",
        "@org_tensorflow//tensorflow/lite:framework",
        "@org_tensorflow//tensorflow/lite:string_util",
        "@org_tensorflow//tensorflow/lite/kernels:builtin_ops",
        "@com_google_absl//absl/strings",
        "@com_googlesource_code_re2//:re2",
    ],
)


cc_test(
    name = "extract_feature_op_test",
    size = "small",
    srcs = ["ops/extract_feature_test.cc"],
    tags = ["no_oss"],
    deps = [
        ":custom_ops",
        "@org_tensorflow//tensorflow/lite:framework",
        "@org_tensorflow//tensorflow/lite/kernels:builtin_ops",
        "@org_tensorflow//tensorflow/lite/kernels:test_util",
        "@com_google_googletest//:gtest",
        "@farmhash_archive//:farmhash",
    ],
)

cc_test(
    name = "normalize_op_test",
    size = "small",
    srcs = ["ops/normalize_test.cc"],
    tags = ["no_oss"],
    deps = [
        ":custom_ops",
        "@org_tensorflow//tensorflow/lite:framework",
        "@org_tensorflow//tensorflow/lite:string_util",
        "@org_tensorflow//tensorflow/lite/kernels:builtin_ops",
        "@org_tensorflow//tensorflow/lite/kernels:test_util",
        "@com_google_googletest//:gtest",
    ],
)

cc_test(
    name = "predict_op_test",
    size = "small",
    srcs = ["ops/predict_test.cc"],
    tags = ["no_oss"],
    deps = [
        ":custom_ops",
        "@org_tensorflow//tensorflow/lite:framework",
        "@org_tensorflow//tensorflow/lite:string_util",
        "@org_tensorflow//tensorflow/lite/kernels:builtin_ops",
        "@org_tensorflow//tensorflow/lite/kernels:test_util",
        "@com_google_googletest//:gtest",
    ],
)

cc_binary(
    name = "smartreply.cpython-36m-x86_64-linux-gnu.so",
    srcs = glob(["inference.cc", "pydep/python3.6m/*.h", "pydep/python3.6m_i/**/*.h"]),
    copts = tflite_copts() + ['-std=c++11', '-Ilibs/cc/', '-Wl,--whole-archive', '-shared', '-fPIC'] + ['-Ilibs/cc/pydep/python3.6m', '-Ilibs/cc/pydep/python3.6m_i', '-Ilibs/cc/pydep/python3.6'],
    deps = [
        ":predictor_lib",
        "@org_tensorflow//tensorflow/lite:framework",
        "@org_tensorflow//tensorflow/lite:string_util",
        "@org_tensorflow//tensorflow/lite/kernels:builtin_ops",
        "@com_google_absl//absl/strings",
        "@com_googlesource_code_re2//:re2",
    ],
    linkshared=True
)



