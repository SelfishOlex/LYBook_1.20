#pragma once
#include <ScriptCanvas/Core/Node.h>
#include <ScriptCanvas/CodeGen/CodeGen.h>
#include <Source/ScriptCanvas/MyIncrementNode.generated.h>

namespace MyProject
{
    class MyIncrementNode : public ScriptCanvas::Node
    {
        ScriptCanvas_Node(MyIncrementNode,
            ScriptCanvas_Node::Name("My Increment Node")
            ScriptCanvas_Node::Uuid(
                "{4C8D4A9A-D19B-496F-8E4F-2DA6D7DA194A}")
            ScriptCanvas_Node::Description("Increments by one"));

    protected:
        // Inputs
        ScriptCanvas_In(ScriptCanvas_In::Name("In", "desc"));
        // Outputs
        ScriptCanvas_Out(ScriptCanvas_Out::Name("Out", "desc"));

        ScriptCanvas_Property(float,
            ScriptCanvas_Property::Name("Value",
                "The input value.")
            ScriptCanvas_Property::Visibility(true)
            ScriptCanvas_Property::Input);

        ScriptCanvas_Property(float,
            ScriptCanvas_Property::Name("Result",
                "The input incremented by one.")
            ScriptCanvas_Property::Visibility(true)
            ScriptCanvas_Property::Output
            ScriptCanvas_Property::OutputStorageSpec);

        void OnInputSignal(const ScriptCanvas::SlotId&) override;
    };
}