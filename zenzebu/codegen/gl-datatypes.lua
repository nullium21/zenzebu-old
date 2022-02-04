local vec_sizes = { 1, 2, 3, 4 }
local mat_sizes = { {2,2}, {3,3}, {4,4}, {2,3}, {3,2}, {2,4}, {4,2}, {3,4}, {4,3} }

local special_types = { 'sampler2d' }
local el_types = { 'int', 'float', 'uint' }

for _, sz in ipairs(vec_sizes) do
  local vecs = ''

  for _, t in ipairs(el_types) do vecs = vecs .. t .. tostring(sz) .. ', ' end

  print(vecs)
end

for _, ms in ipairs(mat_sizes) do
  print(('float%dx%d, '):format(table.unpack(ms)))
end

for _, st in ipairs(special_types) do print(st .. ', ') end
