
awake = function()
    Debug.logInfo(this.name)
end


update = function()
    if(InputDetection.getKey(0x57))then
        this.transform.translate1(this.transform,0,1)
        print("button w down")
    end
    if(InputDetection.getKeyUp(0x57))then
        print("button w up")
    end
    if(InputDetection.getKey(0x41))then
        this.transform:translate1(-1,0)
        print("button a down")
    end
    if(InputDetection.getKeyUp(0x41))then

        print("button a up")
    end
    if(InputDetection.getKey(0x53))then
        this.transform:translate1(0,-1)
        print("button s down")
    end
    if(InputDetection.getKeyUp(0x53))then
        print("button s up")
    end
    if(InputDetection.getKey(0x44))then
        this.transform:translate1(1,0)
        print("button d down")
    end
    if(InputDetection.getKeyUp(0x44))then
        print("button d up")
    end
end