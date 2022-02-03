local short_type_names = {
  float = 'f'
}

local function gen_glm_mat_fn(rows, cols, el_type)
  local fmt = [[
    void uniform(std::string name, glm::mat<%d, %d, %s> value) {
      int loc = glGetUniformLocation(id, name.c_str());
      glUniformMatrix%s%sv(loc, 1, false, glm::value_ptr(value));
    }
  ]]

  local size_str = (rows == cols) and tostring(rows) or (tostring(rows) .. 'x' .. tostring(cols))

  return fmt:format(rows, cols, el_type, size_str, short_type_names[el_type])
end

local mat_sizes = { {2,2}, {3,3}, {4,4}, {2,3}, {3,2}, {2,4}, {4,2}, {3,4}, {4,3} }

for _, sz in ipairs(mat_sizes) do
  for t, _ in pairs(short_type_names) do
    print(gen_glm_mat_fn(sz[1], sz[2], t))
  end
end
