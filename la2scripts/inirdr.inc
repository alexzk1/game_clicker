-- INI Reader
--based on: http://sar.n1bus.eu/read-wall/4

-- Create new INI file object
function NewIniReader()
	-- reader "object" table
	local reader = {};
	-- Here's the actual read function.
	function reader:Read(fName)
		-- Create variables
		self.root = {};
		self.reading_section = "";
		-- Loop through each line
		for line in io.lines(fName) do
			-- Reading section
			if startsWith(line, "[") then
				-- Get the section name
				local section = string.sub(line,2,-2);
				-- Add variables for this section
				self.root[section] = {};
				self.reading_section = section;
			-- Not a comment
			elseif not startsWith(line, ";") then
				-- if its a value and a section is selected
				if self.reading_section then
					-- Get Key and Value
					local var,val = line:split("=");
					local var,val = var:trim(), val:trim();
					-- Check for comments
					if string.find(val, ";") then
						val,comment = val:split(";");
						val = val:trim();
					end

					-- Add var and val to the section array.
					self.root[self.reading_section] = self.root[self.reading_section] or {};
					self.root[self.reading_section][var] = val;
				else
					-- No section for this value
					return error("No element set for setting");
				end
			end
		end
	end

	-- get value 'Key' from section
	function reader:GetValue(Section, Key)
		return self.root[Section][Key];
	end

	-- get keys from a section
	function reader:GetKeys(Section)
		return self.root[Section];
	end

	-- return the object
	return reader;
end

-- String functions from Haywoods DROPP Script..
function startsWith(text,prefix)
	return string.sub(text, 1, string.len(prefix)) == prefix
end


function string:split(sep)
	return self:match("([^" .. sep .. "]+)[" .. sep .. "]+(.+)")
end

function string:trim()
	return self:match("^%s*(.-)%s*$")
end