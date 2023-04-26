update = function()
    if(InputDetection.getKeyDown(0x57))then
        this.transform:translate1(0,50)
        print("button w down")
    end
    if(InputDetection.getKeyUp(0x57))then
        print("button w up")
    end
    if(InputDetection.getKeyDown(0x41))then
        print("button a down")
    end
    if(InputDetection.getKeyUp(0x41))then

        print("button a up")
    end
    if(InputDetection.getKeyDown(0x53))then
        this.transform:translate1(0,-50)
        print("button s down")
    end
    if(InputDetection.getKeyUp(0x53))then
        print("button s up")
    end
    if(InputDetection.getKeyDown(0x44))then
        print("button d down")
    end
    if(InputDetection.getKeyUp(0x44))then
        print("button d up")
    end
end