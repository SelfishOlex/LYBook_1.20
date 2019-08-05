-- server_timed_spawner.lua --
local ServerTimedSpawner = {}

function ServerTimedSpawner:OnActivate()
    if MyHelper.IsDedicatedServer() then
        Debug.Log("Spawner enabled on dedicated server")
        RandomTimedSpawnerRequestBus.Event.Enable(self.entityId)
    end
end

return ServerTimedSpawner