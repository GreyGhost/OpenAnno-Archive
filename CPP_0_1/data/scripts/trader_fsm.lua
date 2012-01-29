current_state = "GoAround"
debug = false
busy = false
DestX = -1
DestY = -1
BuyerID = -1
SellerID = -1
PlayerID = -1

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
		cx,cy = inter_GetPosition()
		printDebug("[Lua]: MyPos "..cx.." : "..cy)
		printDebug("[Lua]: RaPos "..DestX.." : "..DestY)
		if (DestX == cx) and (DestY == cy) then
			busy = false
		end
	end
	if (busy == false) then
		PlayerID = -1;
		BuyerID = inter_GetBuyer()
		SellerID = inter_GetSeller()
		if ( BuyerID > 0 ) then
			PlayerID = BuyerID
		end
		if ( SellerID > 0 ) then
			PlayerID = SellerID
		end
		if ( PlayerID > 0 ) then
			DestX,DestY = inter_GetHarbourPositionOfPlayer( PlayerID )
			printDebug("[Lua] GetHarbourPositionOfPlayer "..PlayerID )
			if ( DestX >= 0 ) then
				printDebug("[Lua] Found harbour of player "..PlayerID.." at "..DestX..":"..DestY )
				changeState("GoToIsland")
			end
		end
		if ( PlayerID <= 0 ) then
			DestX,DestY = inter_GetRandomPosition()
			printDebug("[Lua]: RandPos "..DestX.." : "..DestY )
		end
		inter_SetDestination(DestX,DestY)
		busy = true
	end
end 

States["GoAround"]["Exit"] = function()
	printDebug("[Lua]: GoAround:Exit")
end

-- 2nd state: GoToIsland ------------------
States["GoToIsland"] = {}
States["GoToIsland"]["Enter"] = function()
	printDebug("[Lua]: GoToIsland:Enter")
end

States["GoToIsland"]["Execute"] = function()
	printDebug("[Lua]: GoToIsland:Execute")
	cx,cy = inter_GetPosition()
	if (DestX == cx) and (DestY == cy) then
		changeState("BuyAndSell")
	end
	inter_SetDestination(DestX,DestY)
end

States["GoToIsland"]["Exit"] = function()
	printDebug("[Lua]: GoToIsland:Exit")
	changeState("GoAround")
end

-- 2nd state: GoToIsland ------------------
States["BuyAndSell"] = {}
States["BuyAndSell"]["Enter"] = function()
	printDebug("[Lua]: BuyAndSell:Enter")
end

States["BuyAndSell"]["Execute"] = function()
	printDebug("[Lua]: BuyAndSell:Execute")
-- TODO: Implement buy and sell -----------
end

States["BuyAndSell"]["Exit"] = function()
	printDebug("[Lua]: BuyAndSell:Exit")
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
