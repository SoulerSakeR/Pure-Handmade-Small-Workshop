#pragma once

class Collider
{
public:   
    virtual bool is_trigger() const;
    virtual void set_trigger(bool value);
protected:
    bool is_trigger_;
};

