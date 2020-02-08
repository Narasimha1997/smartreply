#include "libs/cc/predictor.h"


#include<iostream>
#include<stdlib.h>

#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/model.h"
#include "tensorflow/lite/op_resolver.h"
#include "tensorflow/lite/string_util.h"

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

static std::unique_ptr<::tflite::FlatBufferModel> model;

namespace tflite {
namespace custom {
namespace smartreply {
namespace runtime {

    void LoadModel(const char * path) {
        if(!path) {
            std::cout<<"Model File not found!\n";
            exit(0);
        }
        model = tflite::FlatBufferModel::BuildFromFile(path);
        if(!model) { 
            std::cout<<"Failed to load Model\n";
            exit(0); 
        }
    }

    std::map<std::string, float> MakePredictions(std::vector<std::string> inputs) {
        std::vector<smartreply::PredictorResponse> response;
        GetSegmentPredictions(inputs, *model, {{}}, &response);
        std::map<std::string, float> c_11_response;
        for(smartreply::PredictorResponse resp : response) 
           c_11_response[resp.GetText().data()] = resp.GetScore();
        
        return c_11_response;
    }
}}}}

void ccapi_loadModel(std::string model_path) {
    tflite::custom::smartreply::runtime::LoadModel(model_path.c_str());
}

std::map<std::string, float> ccapi_makePredictions(std::string text) {
    std::vector<std::string> vect = {text};
    return tflite::custom::smartreply::runtime::MakePredictions(vect);
}

std::vector<std::map<std::string, float>> ccapi_multiplePredictions(std::vector<std::string> inputs) {
    std::vector<std::map<std::string, float>> predictions;
    for(std::string input : inputs) {
        std::map<std::string, float> prediction = ccapi_makePredictions(input);
        predictions.emplace(predictions.end(), prediction);
    }
    return predictions;
}

std::map<std::string, float> ccapi_batchPredictions(std::vector<std::string> inputs) {
    return tflite::custom::smartreply::runtime::MakePredictions(inputs);
}

PYBIND11_MODULE(smartreply, m) {
    m.doc() = "Smartreply python3 runtime, Author : Narasimha Prasanna H.N";
    m.def("load", &ccapi_loadModel, "Model loader");
    m.def("predict", &ccapi_makePredictions, "Make predictions");
    m.def("multi_predict", &ccapi_multiplePredictions, "Make multiple predictions");
    m.def("batch_predict", &ccapi_batchPredictions, "Make batch predictions");
}
