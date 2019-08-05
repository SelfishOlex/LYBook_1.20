local SimpleAI =
{
    Properties =
    {
        -- This is the target toward which you'll move
        MoveToEntity = {
            default=EntityId(),
            description="Entity to move to." },
    }
}

function SimpleAI:OnActivate()
	self.count = 0
    -- Activation Code
    self.tickHandler = TickBus.Connect(self)
end

function SimpleAI:OnDeactivate()
    -- Deactivation Code
    self.tickHandler:Disconnect()
end

function SimpleAI:OnTick()
	self.count = self.count + 1
	if (self.count % 50 == 0) then
	    -- Move!
	    self.requestId = NavigationComponentRequestBus.
	        Event.FindPathToEntity(
	            self.entityId,
	            self.Properties.MoveToEntity)
    end
end

return SimpleAI