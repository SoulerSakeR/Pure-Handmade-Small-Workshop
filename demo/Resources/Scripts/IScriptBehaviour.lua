-- 声明一个 lua class
-- super 为父类
---@return class
function class(super)
    -- 构建类
    local clazz = {super = super ,static = {}}
    clazz.__index = clazz
    setmetatable(clazz, {__index = super})
    -- new 方法创建类对象
    clazz.new = function(...)
        -- 构造一个对象
        local instance = {}
        -- 设置对象的元表为当前类，这样，对象就可以调用当前类生命的方法了
        setmetatable(instance, clazz)
        if clazz.ctor then
            clazz.ctor(instance, ...)
        end
        return instance
    end
    return clazz
end

---@class IScriptBehaviour
---@field public new function
---@field public ctor function
---@field public static table
IScriptBehaviour = class()

---@interface IScriptBehaviour called when all GameObject was created
function IScriptBehaviour:awake() end

---@interface IScriptBehaviour called when all GameObject was awaked
function IScriptBehaviour:start() end

---@interface IScriptBehaviour called
function IScriptBehaviour:onCollide() end

---@interface IScriptBehaviour called every frame before update
function IScriptBehaviour:beforeUpdate() end

---@interface IScriptBehaviour called every frame
function IScriptBehaviour:update() end

---@interface IScriptBehaviour called every frame after update
function IScriptBehaviour:afterUpdate() end
