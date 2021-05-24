#pragma once

namespace Application {
    public ref class TrayIcon sealed {
        bool Autorun;
        bool Paused;

        System::Threading::Mutex ^Mtx;

        System::Windows::Forms::NotifyIcon      ^trayIcon;
        System::Windows::Forms::ContextMenu     ^trayContextMenu;
        System::Windows::Forms::MenuItem        ^menuItem_Autorun;
        System::Windows::Forms::MenuItem        ^menuItem_PauseResume;
        System::Windows::Forms::MenuItem        ^menuItem_Quit;

        void OnClick_Autorun(System::Object ^sender, System::EventArgs ^args);
        void OnClick_PauseResume(System::Object ^sender, System::EventArgs ^args);
        void OnClick_Quit(System::Object ^sender, System::EventArgs ^args);

    public:
        TrayIcon();
        virtual ~TrayIcon();

        bool isPaused();
        void Pause();
    };
}
