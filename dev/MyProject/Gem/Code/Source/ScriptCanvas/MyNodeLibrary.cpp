#include "MyNodeLibrary.h"
#include <AzCore/Serialization/EditContext.h>
#include <ScriptCanvas/Libraries/Libraries.h>
#include "MyScriptCanvasNode.h"
#include "MyIncrementNode.h"

using namespace MyProject;
using namespace ScriptCanvas;

void MyNodeLibrary::Reflect(AZ::ReflectContext* rc)
{
    if (auto* sc = azrtti_cast<AZ::SerializeContext*>(rc))
    {
        sc->Class<MyNodeLibrary, LibraryDefinition>()
            ->Version(1);

        if (auto* editContext = sc->GetEditContext())
        {
            editContext->Class<MyNodeLibrary>("My Nodes", "");
        }
    }
}

void MyNodeLibrary::InitNodeRegistry(NodeRegistry& nr)
{
    using namespace ScriptCanvas::Library;
    AddNodeToRegistry<MyNodeLibrary, MyScriptCanvasNode>(nr);
    AddNodeToRegistry<MyNodeLibrary, MyIncrementNode>(nr);
}

AZStd::vector<AZ::ComponentDescriptor*>
    MyNodeLibrary::GetComponentDescriptors()
{
    return {
        MyScriptCanvasNode::CreateDescriptor(),
        MyIncrementNode::CreateDescriptor(),
    };
}