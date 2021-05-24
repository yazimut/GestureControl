#include <GestureControl/TrayIcon.hpp>

using namespace System;
using namespace System::Drawing;
using namespace System::Threading;
using namespace System::Windows::Forms;

using namespace Microsoft::Win32;

using namespace Application;

TrayIcon::TrayIcon(): Autorun(false), Paused(false), Mtx(gcnew Mutex()) {
    // Tray.Autorun
    this->menuItem_Autorun = gcnew MenuItem();
    this->menuItem_Autorun->Click += gcnew EventHandler(this, &TrayIcon::OnClick_Autorun);

    RegistryKey ^autorunRegKey = Registry::CurrentUser->OpenSubKey(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", true);
    Object ^regVal = autorunRegKey->GetValue(L"GestureControl");
    if (regVal && regVal->ToString() == Windows::Forms::Application::ExecutablePath->ToString()) {
        this->Autorun = false;
    } else {
        this->Autorun = true;
    }
    this->OnClick_Autorun(nullptr, nullptr);

    // Tray.PauseResume
    this->menuItem_PauseResume = gcnew MenuItem();
    this->menuItem_PauseResume->Click += gcnew EventHandler(this, &TrayIcon::OnClick_PauseResume);
    this->menuItem_PauseResume->Text = L"Pause recognition";

    // Tray.Quit
    this->menuItem_Quit = gcnew MenuItem();
    this->menuItem_Quit->Click += gcnew EventHandler(this, &TrayIcon::OnClick_Quit);
    this->menuItem_Quit->Text = L"Quit";

    // Tray context menu
    this->trayContextMenu = gcnew Windows::Forms::ContextMenu();
    this->trayContextMenu->MenuItems->Add(this->menuItem_Autorun);
    this->trayContextMenu->MenuItems->Add(this->menuItem_PauseResume);
    this->trayContextMenu->MenuItems->Add(this->menuItem_Quit);

    // Tray
    this->trayIcon = gcnew NotifyIcon();
    this->trayIcon->Icon = gcnew Drawing::Icon(SystemIcons::WinLogo, 64, 64);
    this->trayIcon->Text = L"Gesture control";
    this->trayIcon->Visible = true;
    this->trayIcon->ContextMenu = this->trayContextMenu;
}

TrayIcon::~TrayIcon() {}

void TrayIcon::OnClick_Autorun(Object ^sender, EventArgs ^args) {
    RegistryKey ^autorunRegKey = Registry::CurrentUser->OpenSubKey(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", true);
    this->Autorun = !this->Autorun;

    if (this->Autorun) {
        autorunRegKey->SetValue(L"GestureControl", Windows::Forms::Application::ExecutablePath->ToString());
        this->menuItem_Autorun->Text = L"Disable autorun with Windows";
    } else {
        try {
            autorunRegKey->DeleteValue(L"GestureControl");
        } catch (ArgumentException ^) {
        } catch (Exception ^e) {
            Console::WriteLine(e->Message);
        }
        this->menuItem_Autorun->Text = L"Autorun with Windows";
    }
}

void TrayIcon::OnClick_PauseResume(Object ^sender, EventArgs ^args) {
    this->Mtx->WaitOne();
    this->Paused = !this->Paused;
    if (this->Paused) {
        this->menuItem_PauseResume->Text = L"Resume recognition";
    } else {
        this->menuItem_PauseResume->Text = L"Pause recognition";
    }
    this->Mtx->ReleaseMutex();
}

void TrayIcon::OnClick_Quit(Object ^sender, EventArgs ^args) {
    Windows::Forms::Application::Exit();
}

bool TrayIcon::isPaused() {
    bool result = false;
    this->Mtx->WaitOne();
    result = this->Paused;
    this->Mtx->ReleaseMutex();
    return result;
}

void TrayIcon::Pause() {
    this->OnClick_PauseResume(nullptr, nullptr);
}
