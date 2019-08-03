#include "PebbleSpawnerComponent.h"
#include <AzCore/Serialization/EditContext.h>
#include <GridMate/Replica/ReplicaFunctions.h>
#include <AzFramework/Network/NetBindingHandlerBus.h>
#include <LmbrCentral/Scripting/SpawnerComponentBus.h>
// for marshaling AZ::Transform
#include <GridMate/Serialize/MathMarshal.h>

using namespace AZ;
using namespace AzFramework;
using namespace GridMate;
using namespace MultiplayerCharacter;

class PebbleSpawnerComponent::Chunk
    : public GridMate::ReplicaChunk
{
public:
    GM_CLASS_ALLOCATOR(Chunk);
    Chunk() : m_spawnPebbleRpc("Spawn Pebble") {}
    static const char* GetChunkName()
    {
        return "PebbleSpawnerComponent::Chunk";
    }
    bool IsReplicaMigratable() override { return true; }
    GridMate::Rpc<RpcArg<AZ::Transform>>::BindInterface<
            PebbleSpawnerComponent,
            &PebbleSpawnerComponent::OnPebbleSpawn>
        m_spawnPebbleRpc;
};

void PebbleSpawnerComponent::Reflect(
    ReflectContext* context)
{
    // We also need to register the chunk type,
    // and this would be a good time to do so.
    auto& descTable = ReplicaChunkDescriptorTable::Get();
    if (!descTable.FindReplicaChunkDescriptor(
        ReplicaChunkClassId(Chunk::GetChunkName())))
    {
        descTable.RegisterChunkType<Chunk>();
    }

    auto sc = azrtti_cast<SerializeContext*>(context);
    if (sc)
    {
        sc->Class<PebbleSpawnerComponent, Component>()
          ->Version(1);
        if (EditContext* ec = sc->GetEditContext())
        {
            ec->Class<PebbleSpawnerComponent>(
                "Pebble Spawner",
                "Executes on server")
                ->ClassElement(Edit::ClassElements::EditorData,
                    "")
                ->Attribute(Edit::Attributes::Category,
                    "Multiplayer Character")
                ->Attribute(Edit::Attributes::
                    AppearsInAddComponentMenu,
                    AZ_CRC("Game"));
        }
    }
}

void PebbleSpawnerComponent::Activate()
{
    PebbleSpawnerComponentBus::Handler::BusConnect();
}

void PebbleSpawnerComponent::Deactivate()
{
    PebbleSpawnerComponentBus::Handler::BusDisconnect();
}

ReplicaChunkPtr PebbleSpawnerComponent::GetNetworkBinding()
{
    m_chunk = CreateReplicaChunk<Chunk>();
    m_chunk->SetHandler(this);
    return m_chunk;
}

void PebbleSpawnerComponent::SetNetworkBinding(
    GridMate::ReplicaChunkPtr chunk)
{
    m_chunk = chunk;
    m_chunk->SetHandler(this);
}

void PebbleSpawnerComponent::UnbindFromNetwork()
{
    if (m_chunk)
    {
        m_chunk->SetHandler(nullptr);
        m_chunk = nullptr;
    }
}

bool PebbleSpawnerComponent::OnPebbleSpawn(AZ::Transform tm,
    const GridMate::RpcContext&)
{
    using SpawnerBus = LmbrCentral::SpawnerComponentRequestBus;
    SpawnerBus::Event(GetEntityId(),
        &SpawnerBus::Events::SpawnAbsolute, tm);
    return false; // Don't send to all proxies
}

void PebbleSpawnerComponent::SpawnPebbleAt(
    const AZ::Transform& tm)
{
    if (Chunk* chunk = static_cast<Chunk*>(m_chunk.get()))
    {
        chunk->m_spawnPebbleRpc(tm);
    }
    else
    {
        OnPebbleSpawn(tm, {}); // enable Editor to shoot as well
    }
}