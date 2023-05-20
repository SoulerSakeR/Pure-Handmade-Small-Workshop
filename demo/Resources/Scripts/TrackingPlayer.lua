require("IScriptBehaviour")

function Vector2DToString(v)
    return v.x .. ", " .. v.y 
end

function getDistance(v1, v2)
    local dx = v2.x - v1.x
    local dy = v2.y - v1.y
    return math.sqrt(dx * dx + dy * dy)
end

function normalize(v)
    local len = math.sqrt(v.x * v.x + v.y * v.y)
    if len == 0 then
        return {x = 0.0, y = 0.0}
    else
        return {x = v.x / len, y = v.y / len}
    end
end

---@class TrackingPlayer : IScriptBehaviour
---@field public gameObject GameObject
TrackingPlayer = class(IScriptBehaviour)

TrackingPlayer.velocity = 2

TrackingPlayer.awake = function(self)
    local cameraPosAfterInitialized = self.gameObject.findTag("Player")[1].transform:getWorldPosition()

    local cameraPosBeforeInitialized = self.gameObject.transform:getWorldPosition()
    self.gameObject.transform:translate1(cameraPosAfterInitialized.x-cameraPosBeforeInitialized.x,cameraPosAfterInitialized.y-cameraPosBeforeInitialized.y)
    print("TrackingPlayer AWAKE")

end

---@param self TrackingPlayer
TrackingPlayer.update  = function(self)
    moveGameObject(self.gameObject)
end