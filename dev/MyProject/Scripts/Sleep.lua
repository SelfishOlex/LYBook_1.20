-- Sleep.lua --
local Sleep = {}

function Sleep:OnActivate()
    Debug.Log("DONT USE! Just a hack to show the loading screen.")
    local sec = tonumber(os.clock() + 2); -- 2 seconds
    while (os.clock() < sec) do
    end
end

return Sleep