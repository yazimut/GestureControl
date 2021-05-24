#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdexcept>

namespace Application {
    enum class Keys {
        VolumeUp = VK_VOLUME_UP,
        VolumeDown = VK_VOLUME_DOWN,
        VolumeMute = VK_VOLUME_MUTE,

        Stop = VK_MEDIA_STOP,
        PlayPause = VK_MEDIA_PLAY_PAUSE,
        PrevTrack = VK_MEDIA_PREV_TRACK,
        NextTrack = VK_MEDIA_NEXT_TRACK
    };

    enum class KeyEventDelay {
        VolumeUp    = 0,
        VolumeDown  = 0,
        VolumeMute  = 100,

        Stop        = 100,
        PlayPause   = 100,
        PrevTrack   = 100,
        NextTrack   = 100
    };

    // Emulates pressing a key on a keyboard.
    // Throws std::runtime_error, if error occured.
    void PressKey(Keys key);
}
