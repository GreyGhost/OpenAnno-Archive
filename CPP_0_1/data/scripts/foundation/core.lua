--
-- Foundation
-- Core helper functions
--
-- @author Stefan Geiger (mailto:stegei@real-world.ch)
-- @date  14/07/06


-- Quit application at the end of this frame
function quit()
	requestExit("OpenAnno")
end;

-- Print to console
function print(str1, str2, str3, str4)
	printToConsole("OpenAnno", str1, str2, str3, str4);
end;
