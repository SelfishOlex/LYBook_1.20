#include "WindowPositionCommands.h"
#include <windows.h>
#include <Winuser.h>
#include <ISystem.h>
#include <IConsole.h>
#include "AzCore/std/string/conversions.h"

using namespace AZStd;
using namespace WindowPosition;

void WindowPositionCommands::Register(ISystem& system)
{
    IConsole* console = system.GetIConsole();
    if (console)
    {
        console->AddCommand("windowxy", WindowXY, 0,
            "set X and Y position of the window");
    }
}

void WindowPositionCommands::Unregister(ISystem& system)
{
    IConsole* console = system.GetIConsole();
    if (console)
    {
        console->RemoveCommand("windowxy");
    }
}

void WindowPositionCommands::WindowXY(IConsoleCmdArgs* args)
{
    if (args->GetArgCount() > 2)
    {
        const int x = stoi(AZStd::string(args->GetArg(1)));
        const int y = stoi(AZStd::string(args->GetArg(2)));

        HWND handle = GetActiveWindow();
        if (!handle)
        {
            // try console window
            handle = GetConsoleWindow();
        }

        if (handle)
            SetWindowPos(handle, nullptr,
                x, y,
                0, 0, SWP_NOOWNERZORDER | SWP_NOSIZE);
    }
}