#include <GestureControl/Recognizer.hpp>
#include <GestureControl/KeyboardEmulator.hpp>

#include <Model/Model.hpp>

#include <string>
#include <iostream>

using std::cout;
using std::endl;
using std::string;

using namespace cv;

using namespace System;
using namespace System::IO;
using namespace System::Threading;
using namespace System::Windows::Forms;

using namespace Runtime::InteropServices;

using namespace Application;

Recognizer::Recognizer(TrayIcon ^trayIcon): 
        ContinuousRepeats(CONTINUOUS_REPEATS_DEFAULT), 
        ContinuousStep(1), 
        trayIcon(trayIcon), 
        webcam(nullptr), frame(nullptr), 
        WebcamMtx(gcnew Mutex) {

    // Load model
    System::String ^ModelPathRef = Windows::Forms::Application::ExecutablePath;
    ModelPathRef = ModelPathRef->Replace(L"GestureControl.exe", L"") + L"model.json";
    if (!File::Exists(ModelPathRef)) {
        throw gcnew System::Exception(L"Model file not found");
    }
    string ModelPath = (const char *)(Marshal::StringToHGlobalAnsi(ModelPathRef)).ToPointer();
    LoadModel(ModelPath);

    // Initialize image capture thread
    this->WebcamTh = gcnew Thread(gcnew ThreadStart(this, &Recognizer::CaptureFrame));
    this->WebcamTh->Start();
}

Recognizer::~Recognizer() {
    if (this->webcam) {
        this->webcam->release();
        delete this->webcam;
        destroyAllWindows();
    }

    if (this->frame) {
        delete this->frame;
    }
}

void Recognizer::CaptureFrame() {
    try {
        while (true) {
            if (!this->trayIcon->isPaused()) {
                this->WebcamMtx->WaitOne();
                
                if (!this->webcam) {
                    this->webcam = new VideoCapture(0);
                    if (!this->webcam->isOpened()) {
                        delete this->webcam;
                        this->webcam = nullptr;
                        this->trayIcon->Pause();
                        MessageBox::Show(
                            L"Camera is busy. Try again later.",
                            L"Error!",
                            MessageBoxButtons::OK,
                            MessageBoxIcon::Information
                        );
                        this->WebcamMtx->ReleaseMutex();
                        continue;
                    }
                    this->frame = new Mat;
                }

                *this->webcam >> *this->frame;
                #if defined(GC_DEBUG)
                    imshow("GestureControl", *this->frame);
                    waitKey(1);
                #endif
                
                this->WebcamMtx->ReleaseMutex();
                Thread::Sleep(1000/30);
            } else {
                if (this->webcam) {
                    this->webcam->release();
                    delete this->webcam;
                    this->webcam = nullptr;

                    delete this->frame;
                    this->frame = nullptr;
                }

                Thread::Sleep(Recognizer::SleepInPause);
                continue;
            }
        }
    } catch (ThreadAbortException ^) {
        return;
    }
}

void Recognizer::Recognize() {
    try {
        Mat frame;
        while (true) {
            // Actions are allowed?
            if (this->trayIcon->isPaused()) {
                Thread::Sleep(Recognizer::SleepInPause);
                continue;
            }

            // Capture frame from a web-camera
            this->WebcamMtx->WaitOne();
            if (!this->webcam) {
                this->WebcamMtx->ReleaseMutex();
                continue;
            }
            
            Mat img = *this->frame;
            this->WebcamMtx->ReleaseMutex();
            
            // Predict a gesture
            Gestures gesture = Predict(img);

            // Actions are allowed?
            if (this->trayIcon->isPaused()) {
                Thread::Sleep(Recognizer::SleepInPause);
                continue;
            }

            // Do actions
            switch (gesture) {
                case Gestures::NoGesture:
                    this->ContinuousRepeats = CONTINUOUS_REPEATS_DEFAULT;
                    this->ContinuousStep = 1;
                    Thread::Sleep(Recognizer::SleepBetweenGestures);
                    break;
                case Gestures::Fist:
                    this->ContinuousRepeats = CONTINUOUS_REPEATS_DEFAULT;
                    this->ContinuousStep = 1;
                    PressKey(Keys::Stop);
                    Thread::Sleep(Recognizer::SleepAfterGesture);
                    break;
                case Gestures::Palm:
                    this->ContinuousRepeats = CONTINUOUS_REPEATS_DEFAULT;
                    this->ContinuousStep = 1;
                    PressKey(Keys::PlayPause);
                    Thread::Sleep(Recognizer::SleepAfterGesture);
                    break;
                case Gestures::Pointer:
                    this->ContinuousRepeats = CONTINUOUS_REPEATS_DEFAULT;
                    this->ContinuousStep = 1;
                    Thread::Sleep(Recognizer::SleepAfterGesture);
                    break;
                case Gestures::Spok:
                    this->ContinuousRepeats = CONTINUOUS_REPEATS_DEFAULT;
                    this->ContinuousStep = 1;
                    this->trayIcon->Pause();
                    Thread::Sleep(Recognizer::SleepAfterGesture);
                    break;
                case Gestures::ThumbDown:
                    --this->ContinuousRepeats;
                    if (!this->ContinuousRepeats) {
                        this->ContinuousRepeats += CONTINUOUS_REPEATS_DEFAULT;
                        ++this->ContinuousStep;
                    }
                    for (int i = 0; i < this->ContinuousStep; ++i) {
                        PressKey(Keys::VolumeDown);
                    }
                    Thread::Sleep(Recognizer::SleepDuringContinuousGesture);
                    break;
                case Gestures::ThumbUp:
                    --this->ContinuousRepeats;
                    if (!this->ContinuousRepeats) {
                        this->ContinuousRepeats += CONTINUOUS_REPEATS_DEFAULT;
                        ++this->ContinuousStep;
                    }
                    for (int i = 0; i < this->ContinuousStep; ++i) {
                        PressKey(Keys::VolumeUp);
                    }
                    Thread::Sleep(Recognizer::SleepDuringContinuousGesture);
                    break;
            }
        }
    } catch (ThreadAbortException ^) {
        this->WebcamTh->Abort();
        return;
    }
}
