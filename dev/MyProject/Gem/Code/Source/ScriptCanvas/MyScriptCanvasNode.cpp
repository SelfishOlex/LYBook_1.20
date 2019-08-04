#include "MyScriptCanvasNode.h"

using namespace ScriptCanvas;

namespace MyProject
{
    void MyScriptCanvasNode::OnInputSignal(const SlotId&)
    {
        SignalOutput(
            MyScriptCanvasNodeProperty::GetOutSlotId(this));
    }
}

#include <Source/ScriptCanvas/MyScriptCanvasNode.generated.cpp>