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

Tracking = class(IScriptBehaviour)
Tracking.animator = {}

---@param self Tracking
Tracking.awake = function(self)
self.animator = self.gameObject:getAnimator()
local allAnimation = self.animator:getAllAnimations()
local allSkins = self.animator:getAllSkins()
local skin_index = self.animator:get_skin_index()
print(skin_index)
self.animator:set_skin_index(4)
end

Tracking.start = function(self)
    self.animator:set_skin_index(5)
end

Tracking.velocity = 2
Tracking.hp = 100
Tracking.skillCD = 100
Tracking.isInvincible = false
Tracking.walkAnimationTime = 160

---@param self Tracking
Tracking.update  = function(self)
    local selfgameObjPos = self.gameObject.transform:getWorldPosition()
--- print(Vector2DToString(gameObjPos))

    local player = self.gameObject.findTag("Player")[1]
    local playerPos = self.gameObject.findTag("Player")[1].transform:getWorldPosition()
    getDistance(playerPos,selfgameObjPos)
--- print(tostring(getDistance(playerPos,selfgameObjPos)))
    
    local moveVector = playerPos - selfgameObjPos
--- print(Vector2DToString(moveVector))

    local moveVectorNormal = normalize(moveVector)
    self.gameObject.transform:translate1(self.velocity * moveVectorNormal.x , self.velocity * moveVectorNormal.y)

    if getDistance(playerPos,selfgameObjPos) < 200 and 
    self.skillCD == 0 then
        self.animator.setAnimation(self.animator,"skill",false)
        self.skillCD = 1000
        self.isInvincible = false
    end

    if self.skillCD <= 840 and
    self.skillCD >= 160 and
    self.walkAnimationTime == 0 then
        self.animator.setAnimation(self.animator,"walking",true)
        self.walkAnimationTime = 160
    end


    if getDistance(playerPos,selfgameObjPos) < 200  and 
    InputDetection.getKey(0x45) and 
    self.isInvincible == false 
    then
        self.animator.setAnimation(self.animator,"landing",false)
        self.hp = self.hp-1
        self.isInvincible = true
    end

    if self.hp <= 0 then
--- UI shows GAME OVER
        print("OVER")
    end


    if self.skillCD > 0 then
        self.skillCD = self.skillCD - 1
    end

    if self.walkAnimationTime > 0 then
        self.walkAnimationTime =  self.walkAnimationTime - 1
    end

    print(self.walkAnimationTime)

  ---  print(self.skillCD)
end