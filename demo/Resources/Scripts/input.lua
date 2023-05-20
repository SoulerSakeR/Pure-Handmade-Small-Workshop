require("IScriptBehaviour")

function negateX(v)
    v.x = -math.abs(v.x)
    return v
end

function absVector(v)
    v.x = math.abs(v.x)
    v.y = math.abs(v.y)
    return v
end



function moveGameObject(gameObject)
    if(InputDetection.getKey(0x57))then
        gameObject.transform:translate1(0,10)
    end
    if(InputDetection.getKey(0x41))then
        gameObject.transform:translate1(-10,0)
        local localScale = gameObject.transform:get_localScale()
        gameObject.transform:set_localScale(negateX(localScale))
    end
    if(InputDetection.getKey(0x53))then
        gameObject.transform:translate1(0,-10)
    end
    if(InputDetection.getKey(0x44))then
        gameObject.transform:translate1(10,0)
        local localScale = gameObject.transform:get_localScale()
        gameObject.transform:set_localScale(absVector(localScale))
    end
    if(InputDetection.getKey(0x20))then
        ability_dash(gameObject)
    end
end

function useAbility(self)
    if(InputDetection.getKey(0x45))then
        self.animator.setAnimation(self.animator,"buff_3",false)
        input.timer = 0
    end
    if(InputDetection.getKey(0x57)) and input.timer == 0 then
        self.animator.setAnimation(self.animator,"walk",true)
        input.timer = input.timer + 1
    end
    if(InputDetection.getKey(0x41)) and input.timer == 0 then
        self.animator.setAnimation(self.animator,"walk",true)
        input.timer = input.timer + 1

    end
    if(InputDetection.getKey(0x53)) and input.timer == 0 then
        self.animator.setAnimation(self.animator,"walk",true)
        input.timer = input.timer + 1
    end
    if(InputDetection.getKey(0x44)) and input.timer == 0 then
        self.animator.setAnimation(self.animator,"walk",true)
        input.timer = input.timer + 1
    end
end

function ability_dash(gameObject)
   gameObject.transform:translate1(-30,0)
end

---@class input : IScriptBehaviour
---@field public gameObject GameObject
input = class(IScriptBehaviour)
input.animator = {}
input.timer = 0
---@param self input
input.awake = function(self)
self.animator = self.gameObject:getAnimator()
local allAnimation = self.animator:getAllAnimations()

for i = 1, #allAnimation do
    print(allAnimation[i])
end

end

---@param self input
input.update  = function(self)
    local selfgameObjPos = self.gameObject.transform:getWorldPosition()
    local bossPos = self.gameObject.findTag("Boss")[1].transform:getWorldPosition()
    moveGameObject(self.gameObject)
    useAbility(self)
end