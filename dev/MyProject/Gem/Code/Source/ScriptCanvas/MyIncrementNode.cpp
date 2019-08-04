#include "MyIncrementNode.h"

using namespace ScriptCanvas;

namespace MyProject
{
    void MyIncrementNode::OnInputSignal(const SlotId&)
    {
        float value = MyIncrementNodeProperty::GetValue(this);

        value++;

        const SlotId resultSlotId =
            MyIncrementNodeProperty::GetResultSlotId(this);
        const Slot* slot = GetSlot(resultSlotId);
        PushOutput(Datum(value), *slot);

        SignalOutput(
            MyIncrementNodeProperty::GetOutSlotId(this));
    }
}

#include <Source/ScriptCanvas/MyIncrementNode.generated.cpp>