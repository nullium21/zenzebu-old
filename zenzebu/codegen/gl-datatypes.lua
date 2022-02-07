--[[

Script to generate zz::render::datatype constant definitions.

The comment before the actual definition is needed for shaders-uniform-switch.lua, which then
generates a switch-case for zz::render::shader::uniforms, converting objects to actual
shader::uniform(name, ...) invocations.

Vector/scalar format:
  // glm::vec<{el_count}, {type}>
  const datatype {type}{el_count} = { .size = sizeof({type})*{el_count}, .el_count = {el_count}, .glsl_name = {gl_vnames[type]}, .gl = {gl_types[type]}, .id = {cur_dt_id++} }

Matrix format:
  // glm::mat<{n_rows}, {n_cols}, {type}>
  const datatype mat{suffix} = { .size = sizeof({type})*{n_rows*n_cols}, .el_count = {n_rows*n_cols}, .glsl_name = mat{suffix}, .gl = GL_FLOAT, .id = {cur_dt_id++} }

  where {suffix} is the dimension for NxN matrices, and NxM for NxM matrices.

]]--

local cur_dt_id = 1 -- #0 = sampler2d

local vec_sizes = { 1, 2, 3, 4 }
local mat_sizes = { {2,2}, {3,3}, {4,4}, {2,3}, {3,2}, {2,4}, {4,2}, {3,4}, {4,3} }

local el_types = { 'int', 'float', 'uint' }
local gl_types = { int = 'GL_INT', float = 'GL_FLOAT', uint = 'GL_UNSIGNED_INT' }

local gl_vnames = { float = 'vec', int = 'ivec', uint = 'uvec' }

local all = {}
local all_cpp = {}

local vec_fmt = '// glm::vec<%d, %s>\nconstexpr const datatype %s%d { .size = sizeof(%s)*%d, .gl = %s, .el_count = %d, .id = %d, .glsl_name = "%s" };'
for _, vsz in ipairs(vec_sizes) do
  for _, t in ipairs(el_types) do
    local glsl_name = (vsz == 1) and t or (gl_vnames[t] .. tostring(vsz))

    print(vec_fmt:format(vsz, t, t, vsz, t, vsz, gl_types[t], vsz, cur_dt_id, glsl_name))
    cur_dt_id = cur_dt_id + 1
    table.insert(all, t .. tostring(vsz))
    table.insert(all_cpp, 'glm::vec<' .. tostring(vsz) .. ', ' .. t .. '>')
  end
end

local mat_fmt = '// glm::mat<%d, %d, float>\nconstexpr const datatype mat%s { .size = sizeof(float)*%d, .gl = %s, .el_count = %d, .id = %d, .glsl_name = "mat%s" };'
for _, msz in ipairs(mat_sizes) do
  local tsuffix = msz[1]==msz[2] and tostring(msz[1]) or msz[1]..'x'..msz[2]
  local elcount = msz[1]*msz[2]
  print(mat_fmt:format(msz[1], msz[2], tsuffix, elcount, gl_types.float, elcount, cur_dt_id, tsuffix))
  cur_dt_id = cur_dt_id + 1
  table.insert(all, 'mat' .. tsuffix)
  table.insert(all_cpp, 'glm::mat<' .. tostring(msz[1]) .. ', ' .. tostring(msz[2]) .. ', float>')
end

local lst_fmt = 'static const datatype all[%d] = { sampler2d, %s };'

print(lst_fmt:format(cur_dt_id, table.concat(all, ', ')))
