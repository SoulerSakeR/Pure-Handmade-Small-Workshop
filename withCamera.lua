
awake = function()
    local gameObj = this.findTag("Player")
    Debug.logInfo(tostring(gameObj[1]:getID()))
end