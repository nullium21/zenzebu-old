group 'examples'

project 'ex.logging'
    location 'logging/'
    kind 'consoleapp'

    dependson { 'zenzebu' }

    language 'c++'
    cppdialect 'c++17'

    targetdir (tgt_dir)
    objdir    (obj_dir)

    files {
        'logging/src/**.cc'
    }

    includedirs {
        '%{wks.location}/zenzebu/src'
    }

    links { 'zenzebu' }

    filter "system:linux"
        defines { "ZZ_LINUX" }

    filter "configurations:debug"
        defines { "ZZ_DEBUG" }
        symbols "on"

    filter "configurations:release"
        defines { "ZZ_RELEASE" }
        optimize "on"

project 'ex.ecs'
    location 'ecs/'
    kind 'consoleapp'

    dependson { 'zenzebu' }

    language 'c++'
    cppdialect 'c++17'

    targetdir (tgt_dir)
    objdir    (obj_dir)

    files {
        'ecs/src/**.cc'
    }

    includedirs {
        '%{wks.location}/zenzebu/src',
        '%{wks.location}/zenzebu/entt/single_include'
    }

    links { 'zenzebu' }

    filter "system:linux"
        defines { "ZZ_LINUX" }

    filter "configurations:debug"
        defines { "ZZ_DEBUG" }
        symbols "on"

    filter "configurations:release"
        defines { "ZZ_RELEASE" }
        optimize "on"

project 'ex.nativescript'
    location 'nativescript/'
    kind 'consoleapp'

    dependson { 'zenzebu' }

    language 'c++'
    cppdialect 'c++17'

    targetdir (tgt_dir)
    objdir    (obj_dir)

    files {
        'nativescript/src/**.cc'
    }

    includedirs {
        '%{wks.location}/zenzebu/src',
        '%{wks.location}/zenzebu/entt/single_include'
    }

    links { 'zenzebu' }

    filter "system:linux"
        defines { "ZZ_LINUX" }

    filter "configurations:debug"
        defines { "ZZ_DEBUG" }
        symbols "on"

    filter "configurations:release"
        defines { "ZZ_RELEASE" }
        optimize "on"

group ''
