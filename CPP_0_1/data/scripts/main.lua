--
-- Main lua script file
-- This is the entry point for all other files you want to load
--
-- @author Stefan Geiger (mailto:stegei@real-world.ch)
-- @date  14/07/06

-- init function, this is called when the application starts
function init()
    -- do some init stuff
    print("Lua init");
end;

-- global key input function
function keyEvent(key, isDown)
    if isDown then
        print("key pressed: ", key);
    else
        print("key released: ", key);
    end;
end;

-- update function, dt is in seconds
function update(dt)
end;
