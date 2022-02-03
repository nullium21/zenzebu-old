local short_type_names = {
  float = 'f', uint = 'ui', int = 'i'
}

local function gen_glm_vec_fn(size, el_type, fields)
  local fmt = [[
    inline void uniform(std::string name, glm::vec<%d, %s> value) { uniform(name, %s); }
  ]]

  local unboxed_args do
    unboxed_args = {}

    for _, f in ipairs(fields) do table.insert(unboxed_args, 'value.' .. f) end
  end

  return fmt:format(size, el_type, table.concat(unboxed_args, ', '))
end

local function gen_no_glm_vec_fn(size, el_type, fields)
  local fmt = [[
    void uniform(std::string name, %s) {
      int loc = glGetUniformLocation(id, name.c_str());
      glUniform%d%s(loc, %s);
    }
  ]]

  local fn_args do
    fn_args = {}

    for _, f in ipairs(fields) do table.insert(fn_args, el_type .. ' ' .. f) end
  end

  return fmt:format(table.concat(fn_args, ', '), size, short_type_names[el_type], table.concat(fields, ', '))
end

local vec_fields   = { 'x', 'y', 'z', 'w' }

local vec_max_sz = #vec_fields

for i = 1, vec_max_sz do
  local fields = {}
  for j = 1, i do table.insert(fields, vec_fields[j]) end

  for t, _ in pairs(short_type_names) do
    print(gen_no_glm_vec_fn(i, t, fields))
    print(gen_glm_vec_fn(i, t, fields))
  end
end
