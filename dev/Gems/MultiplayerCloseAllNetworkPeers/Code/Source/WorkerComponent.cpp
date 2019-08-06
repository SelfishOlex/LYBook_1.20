#include "MultiplayerCloseAllNetworkPeers_precompiled.h"
#include "WorkerComponent.h"
#include <AzCore/Serialization/EditContext.h>
#include <GridMate/Replica/ReplicaFunctions.h>
#include <MultiplayerCloseAllNetworkPeers/ShutdownApplication.h>

using namespace AZ;
using namespace MultiplayerCloseAllNetworkPeers;
using namespace GridMate;

class WorkerComponent::Chunk : public GridMate::ReplicaChunk
{
public:
    GM_CLASS_ALLOCATOR(Chunk);

    Chunk() : m_closeAllRpc("CloseAllRpc") {}

    bool IsReplicaMigratable() override { return true; }

    static const char* GetChunkName()
    {
        return "WorkerComponent::Chunk";
    }

    GridMate::Rpc<>::BindInterface<WorkerComponent, &WorkerComponent::OnCloseAll> m_closeAllRpc;
};

void WorkerComponent::Reflect(AZ::ReflectContext* context)
{
    auto& descTable = ReplicaChunkDescriptorTable::Get();
    if (!descTable.FindReplicaChunkDescriptor(
        ReplicaChunkClassId(Chunk::GetChunkName())))
    {
        descTable.RegisterChunkType<Chunk>();
    }

    if (auto sc = azrtti_cast<AZ::SerializeContext*>(context))
    {
        sc->Class<WorkerComponent, AZ::Component>()->Version(1);
    }
}

void WorkerComponent::Activate()
{
    m_isShuttinDown = false;

    CloseAllRequestBus::Handler::BusConnect();
    TickBus::Handler::BusConnect();
}

void WorkerComponent::Deactivate()
{
    CloseAllRequestBus::Handler::BusDisconnect();
    TickBus::Handler::BusDisconnect();

    m_isShuttinDown = false;
}

CloseResult WorkerComponent::CloseAll()
{
    if (Chunk* chunk = static_cast<Chunk*>(m_chunk.get()))
    {
        AZ_TracePrintf("CloseAll", "invoking RPC");
        chunk->m_closeAllRpc();
        return CloseResult::ClosingViaWorkerComponent;
    }

    ShutdownApplication();
    return CloseResult::CloseImmediately;
}

bool WorkerComponent::OnCloseAll(const GridMate::RpcContext&)
{
    m_shutdownCountdown = m_ticksBeforeShutdown;
    m_isShuttinDown = true;
    return true; // routes the call back to all clients
}

void WorkerComponent::OnTick(float, ScriptTimePoint)
{
    if (m_isShuttinDown && m_shutdownCountdown-- == 0)
    {
        ShutdownApplication();
    }
}

GridMate::ReplicaChunkPtr WorkerComponent::GetNetworkBinding()
{
    m_chunk = GridMate::CreateReplicaChunk<Chunk>();
    m_chunk->SetHandler(this);
    return m_chunk;
}

void WorkerComponent::SetNetworkBinding(ReplicaChunkPtr chunk)
{
    m_chunk = chunk;
    m_chunk->SetHandler(this);
}

void WorkerComponent::UnbindFromNetwork()
{
    m_chunk->SetHandler(nullptr);
    m_chunk = nullptr;
}
