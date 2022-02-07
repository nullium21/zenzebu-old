local filename = ...

local file = io.open(filename, 'r')

local data = file:read 'a'

local axes = { 'x', 'y', 'z', 'w' }

local gfmt = 'std::get<%s>(p.value).%s'
local vfmt = 'case datatype::%s.id: uniform(p.name, %s); break;'
local mfmt = 'case datatype::%s.id: uniform(p.name, std::get<%s>(p.value)); break;'

for ctype, targs, name in data:gmatch '// glm::([a-z]+)<(.-)>\nconst datatype ([a-z0-9]+)' do
	local cfull = ('glm::%s<%s>'):format(ctype, targs)

	if ctype == 'vec' then
		local size, eltype = targs:match '([0-9]+), ([a-z]+)'
		local args = {}
		for i = 1, tonumber(size) do
			args[i] = gfmt:format(cfull, axes[i])
		end
		print(vfmt:format(name, table.concat(args, ', ')))
	elseif ctype == 'mat' then
		print(mfmt:format(name, cfull))
	end
end
