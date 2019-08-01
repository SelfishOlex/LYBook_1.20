#pragma once
struct ISystem;
struct IConsoleCmdArgs;

namespace WindowPosition
{
    class WindowPositionCommands
    {
    public:
        void Register(ISystem& system);
        void Unregister(ISystem& system);
        static void WindowXY(IConsoleCmdArgs* args);
    };
}