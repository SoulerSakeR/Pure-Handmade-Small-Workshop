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



---@class Tracking : IScriptBehaviour
---@field public gameObject GameObject
Tracking = class(IScriptBehaviour) 

Tracking.velocity = 2

---@param self Tracking
Tracking.update  = function(self)
    local selfgameObjPos = self.gameObject.transform:getWorldPosition()
--- print(Vector2DToString(gameObjPos))

    local playerPos = self.gameObject.findTag("Player")[1].transform:getWorldPosition()
    getDistance(playerPos,selfgameObjPos)
--- print(tostring(getDistance(playerPos,selfgameObjPos)))
    
    local moveVector = playerPos - selfgameObjPos
--- print(Vector2DToString(moveVector))

    local moveVectorNormal = normalize(moveVector)
    self.gameObject.transform:translate1(self.velocity * moveVectorNormal.x , self.velocity * moveVectorNormal.y)
end