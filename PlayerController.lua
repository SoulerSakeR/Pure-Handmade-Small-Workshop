require("IScriptBehaviour")

---@class PlayerController:IScriptBehaviour
---@field public hp number
---@field gameobject GameObject
PlayerController = class(IScriptBehaviour)
PlayerController.hp = 10

function PlayerController:awake()
    print(tostring(self.gameObject.name))
end
function PlayerController:update()
    if(InputDetection.getKey(0x57))then
        self.gameObject.transform:translate1(0,1)
        print("button w down")
    end
    if(InputDetection.getKeyUp(0x57))then
        print("button w up")
    end
    if(InputDetection.getKey(0x41))then
        self.gameObject.transform:translate1(-1,0)
        print("button a down")
    end
    if(InputDetection.getKeyUp(0x41))then

        print("button a up")
    end
    if(InputDetection.getKey(0x53))then
        self.gameObject.transform:translate1(0,-1)
        print("button s down")
    end
    if(InputDetection.getKeyUp(0x53))then
        print("button s up")
    end
    if(InputDetection.getKey(0x44))then
        self.gameObject.transform:translate1(1,0)
        print("button d down")
    end
    if(InputDetection.getKeyUp(0x44))then
        print("button d up")
    end
end