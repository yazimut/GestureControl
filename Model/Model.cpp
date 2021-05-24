#include <Model/ModelAPI.h>
#include <Model/Model.hpp>

#include <fdeep/fdeep.hpp>
#include <opencv2/opencv.hpp>

#include <string>
#include <iostream>
#include <windows.h>

using std::cout;
using std::endl;
using std::string;
using namespace cv;
using namespace fdeep;

model *Model = nullptr;

void LoadModel(const string &path) {
    Model = new model(load_model(path.c_str()));
}

MODEL_API string GesturesToString(const Gestures &val) {
    string res = "";
    switch (val) {
        case Gestures::NoGesture:
            res = "No gesture";
            break;
        case Gestures::Fist:
            res = "Fist";
            break;
        case Gestures::Palm:
            res = "Palm";
            break;
        case Gestures::Pointer:
            res = "Pointer";
            break;
        case Gestures::Spok:
            res = "Spok";
            break;
        case Gestures::ThumbDown:
            res = "Thumb down";
            break;
        case Gestures::ThumbUp:
            res = "Thumb up";
            break;
        default:
            break;
    }
    return res;
}

MODEL_API void Preprocess(Mat &img) {
    cvtColor(img, img, COLOR_BGR2RGB);
    resize(img, img, Size(224, 224), 0.0, 0.0, INTER_LINEAR);
}

Gestures Predict(Mat &img) {
    Preprocess(img);

    tensor input_tensor = tensor_from_bytes(
        img.ptr(),
        static_cast<size_t>(img.rows),
        static_cast<size_t>(img.cols),
        static_cast<size_t>(img.channels()),
        -1.0f, 1.0f
    );

    tensors result = Model->predict({ input_tensor });
    auto vec = result[0].as_vector();
    Gestures res = Gestures::NoGesture;
    for (int i = 0; i < vec->size(); ++i) {
        if ((*vec)[i] >= MIN_ACCURACY) {
            res = static_cast<Gestures>(i);
            #if defined(GC_DEBUG)
                cout << GesturesToString(res) << " - " << (*vec)[i] * 100.0f << '%' << endl;
            #endif
            break;
        }
    }

    return res;
}
