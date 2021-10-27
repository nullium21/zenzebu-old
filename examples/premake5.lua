function example_project(name, incdirs)
    project('ex.' .. name)
    location(name .. '/')
    kind 'consoleapp'
    dependson { 'zenzebu' }
    language 'c++'
    cppdialect 'c++17'
    targetdir(tgt_dir)
    objdir   (obj_dir)
    files { name .. '/src/**.cc' }
    includedirs(incdirs)
    links { 'zenzebu' }

    filter "system:linux"
        defines { "ZZ_LINUX" }
    filter "configurations:debug"
        defines { "ZZ_DEBUG" }
        symbols "on"
    filter "configurations:release"
        defines { "ZZ_RELEASE" }
        optimize "on"
end

group 'examples'

example_project('logging', {
    '%{inc_dir.zz}'
})

example_project('ecs', {
    '%{inc_dir.zz}',
    '%{inc_dir.entt}'
})

example_project('nativescript', {
    '%{inc_dir.zz}',
    '%{inc_dir.entt}'
})

example_project('window', {
    '%{inc_dir.zz}',
    '%{inc_dir.entt}',
    '%{inc_dir.glfw}',
    '%{inc_dir.glm}'
})

example_project('layers', {
    '%{inc_dir.zz}',
    '%{inc_dir.entt}'
})

group ''
