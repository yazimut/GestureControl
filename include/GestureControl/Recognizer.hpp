#pragma once
#include <opencv2/opencv.hpp>
#include <GestureControl/TrayIcon.hpp>

#define CONTINUOUS_REPEATS_DEFAULT 5

namespace Application {

    public ref class Recognizer sealed {
        static const int SleepInPause = 1000;
        static const int SleepAfterGesture = 2500;
        static const int SleepBetweenGestures = 500;
        static const int SleepDuringContinuousGesture = 25;

        int ContinuousRepeats;
        int ContinuousStep;
        
        TrayIcon ^trayIcon;

        System::Threading::Thread ^WebcamTh;
        System::Threading::Mutex ^WebcamMtx;
        cv::VideoCapture *webcam;
        cv::Mat *frame;

        void CaptureFrame();

    public:
        Recognizer(TrayIcon ^trayIcon);
        virtual ~Recognizer();

        void Recognize();
    };
}
