require("IScriptBehaviour")

function moveGameObject(gameObject)
    if(InputDetection.getKey(0x57))then
        gameObject.transform:translate1(0,10)
    end
    if(InputDetection.getKey(0x41))then
        gameObject.transform:translate1(-10,0)
    end
    if(InputDetection.getKey(0x53))then
        gameObject.transform:translate1(0,-10)
    end
    if(InputDetection.getKey(0x44))then
        gameObject.transform:translate1(10,0)
    end
    if(InputDetection.getKey(0x20))then
        ability_dash(gameObject)
    end
end

function useAbility(self)
    if(InputDetection.getKey(0x45))then
        self.animator.setAnimation(self.animator,"buff_3",false)
    end
end

function ability_dash(gameObject)
   gameObject.transform:translate1(-30,0)
end

---@class input : IScriptBehaviour
---@field public gameObject GameObject
input = class(IScriptBehaviour)
input.animator = {}
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
    moveGameObject(self.gameObject)
    useAbility(self)
end