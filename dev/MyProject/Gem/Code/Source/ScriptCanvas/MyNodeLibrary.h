#pragma once
#include <AzCore/std/containers/vector.h>
#include <AzCore/std/containers/unordered_map.h>
#include <ScriptCanvas/Libraries/Libraries.h>

namespace MyProject
{
    struct MyNodeLibrary :
        ScriptCanvas::Library::LibraryDefinition
    {
        AZ_RTTI(MyNodeLibrary,
            "{55c3e827-0048-4f65-a461-692177c25699}",
            ScriptCanvas::Library::LibraryDefinition);

        static void Reflect(AZ::ReflectContext*);
        static void InitNodeRegistry(
            ScriptCanvas::NodeRegistry& nr);
        static AZStd::vector<AZ::ComponentDescriptor*>
            GetComponentDescriptors();
    };
}