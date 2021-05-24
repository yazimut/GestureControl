#include <GestureControl/KeyboardEmulator.hpp>

using namespace std;
using namespace Application;

void Application::PressKey(Keys key) {
    INPUT ip;
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0;
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;
    ip.ki.wVk = (WORD)key;

    ip.ki.dwFlags = 0;
    if (!SendInput(1, &ip, sizeof(ip))) {
        throw runtime_error("Error occured while inserting KEY_DOWN event in a system stream");
    }

    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    if (!SendInput(1, &ip, sizeof(ip))) {
        throw runtime_error("Error occured while inserting KEY_UP event in a system stream");
    }

    switch (key) {
        case Keys::VolumeUp:
            Sleep((DWORD)KeyEventDelay::VolumeUp);
            break;
        case Keys::VolumeDown:
            Sleep((DWORD)KeyEventDelay::VolumeDown);
            break;
        case Keys::VolumeMute:
            Sleep((DWORD)KeyEventDelay::VolumeMute);
            break;
        case Keys::Stop:
            Sleep((DWORD)KeyEventDelay::Stop);
            break;
        case Keys::PlayPause:
            Sleep((DWORD)KeyEventDelay::PlayPause);
            break;
        case Keys::PrevTrack:
            Sleep((DWORD)KeyEventDelay::PrevTrack);
        case Keys::NextTrack:
            Sleep((DWORD)KeyEventDelay::NextTrack);
            break;
        default:
            throw runtime_error("Error! Invalid key.");
            break;
    }
}
