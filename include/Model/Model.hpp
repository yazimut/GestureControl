#pragma once
#include "ModelAPI.h"

#include <opencv2/opencv.hpp>

#define MIN_ACCURACY 0.9995f

enum class Gestures {
    NoGesture = -1,
    Fist,
    Palm,
    Pointer,
    Spok,
    ThumbDown,
    ThumbUp
};

MODEL_API void LoadModel(const std::string &path);

MODEL_API Gestures Predict(cv::Mat &img);
