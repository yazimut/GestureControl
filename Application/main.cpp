#include <GestureControl/TrayIcon.hpp>
#include <GestureControl/Recognizer.hpp>

using namespace System;
using namespace System::Drawing;
using namespace System::Threading;
using namespace System::Windows::Forms;

using namespace Application;

[STAThreadAttribute]
int main(array<String ^> ^argv) {
    setlocale(LC_ALL, "");
    
    Windows::Forms::Application::EnableVisualStyles();
    Windows::Forms::Application::SetCompatibleTextRenderingDefault(false);

    TrayIcon ^trayIcon = gcnew TrayIcon();
    Recognizer ^recognizer;
    try {
        recognizer = gcnew Recognizer(trayIcon);
    } catch (System::Exception ^excp) {
        MessageBox::Show(
            L"Gestures model not found.",
            L"Error!",
            MessageBoxButtons::OK,
            MessageBoxIcon::Stop
        );
        Console::WriteLine(excp->Message);
        return -1;
    }
    
    Thread ^th = gcnew Thread(gcnew ThreadStart(recognizer, &Recognizer::Recognize));
    th->Start();

    Windows::Forms::Application::Run();
    th->Abort();
}
