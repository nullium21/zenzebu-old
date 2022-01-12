function example_project(name)
    project('ex.' .. name)
    location(name .. '/')
    kind 'consoleapp'
    dependson { 'zenzebu' }
    language 'c++'
    cppdialect 'c++17'
    targetdir(tgt_dir)
    objdir   (obj_dir)
    files { name .. '/src/**.cc' }
    includedirs(inc_dir.all)
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

example_project 'logging'

example_project 'ecs'

example_project 'nativescript'

example_project 'window'

example_project 'layers'

group ''
