
#include "MainForm.h"
//using namespace std;
using namespace System;
using namespace System::Windows::Forms;

[STAThread]
void Main(array<System::String^> ^args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	ANPRproject::MainForm form;
	Application::Run(%form);
}