current_state = "GoAround"
debug = false
busy = false
attack = false
DestX = -1
DestY = -1

States = {}

-- 1st state: GoAround --------------------
States["GoAround"] = {}
States["GoAround"]["Enter"] = function()
	printDebug("[Lua]: GoAround:Enter")
	busy = false
end

States["GoAround"]["Execute"] = function()
	printDebug("[Lua]: GoAround:Execute") 
	if (busy == true) then
		cx,cy = inter_PiratesGetPosition()
		printDebug("[Lua]: PiratesPos "..cx.." : "..cy)
		printDebug("[Lua]: RaPos "..DestX.." : "..DestY)
		if (DestX == cx) and (DestY == cy) then
			busy = false
		end
	end
	if (busy == false) then
		DestX,DestY = inter_PiratesGetRandomPosition()
		printDebug("[Lua] PiratesGetRandomPosition")
		inter_PiratesSetDestination(DestX,DestY)
		busy = true
	end
end 

States["GoAround"]["Exit"] = function()
	printDebug("[Lua]: GoAround:Exit")
end

-- 2nd state: Attack -------------------
States["Attack"] = {}
States["Attack"]["Enter"] = function()
	printDebug("[Lua]: Attack:Enter")
	busy = false
	attack = true
end

-- TODO
States["Attack"]["Execute"] = function()
	printDebug("[Lua]: Attack:Execute")
end

States["Attack"]["Exit"] = function()
	printDebug("[Lua]: GoAround:Exit")
	busy = false
	attack = false
	changeState("GoAround")
end

-- interface and intermediate functions ---

LuaUpdateFSM = function()
	States[current_state]["Execute"]()
end

LuaGetAction = function()
end

ChangeState = function(newState)
	States[current_state]["Exit"]()
	current_state = newState
	States[current_state]["Enter"]()
end

printDebug = function(line)
	if debug then print(line) end
end
