-- Orbiting.lua -- Constant turning around Z axis
local Orbiting =
{
    Properties =
    {
        -- Property definitions
        speed = { default = 10,
            description = "speed of orbiting",
            suffix = "deg/sec" }
    }
}

function Orbiting:OnActivate()
     -- Activation Code
    Debug.Log("Orbiting activated")
    self.tickHandler = TickBus.Connect(self)
end

function Orbiting:OnDeactivate()
    -- Deactivation Code
    self.tickHandler:Disconnect()
end

function Orbiting:OnTick(deltaTime, timePoint)
    TransformBus.Event.RotateAroundLocalZ(
        self.entityId, self.Properties.speed * deltaTime)
end

return Orbiting
