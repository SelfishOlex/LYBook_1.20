#pragma once
#include <ScriptCanvas/Core/Node.h>
#include <ScriptCanvas/CodeGen/CodeGen.h>
#include <Source/ScriptCanvas/MyScriptCanvasNode.generated.h>

namespace MyProject
{
    class MyScriptCanvasNode : public ScriptCanvas::Node
    {
        ScriptCanvas_Node(MyScriptCanvasNode,
            ScriptCanvas_Node::Name("My ScriptCanvas Node")
            ScriptCanvas_Node::Uuid(
                "{0AED9EC4-E05B-4FAE-B9B5-27933F633F0D}")
            ScriptCanvas_Node::Description("My first node")
        );

    protected:
        // Inputs
        ScriptCanvas_In(ScriptCanvas_In::Name("In", "desc"));
        // Outputs
        ScriptCanvas_Out(ScriptCanvas_Out::Name("Out", "desc"));

        void OnInputSignal(const ScriptCanvas::SlotId&) override;
    };
}