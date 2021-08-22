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
        '%{inc_dir.zz}'
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
        '%{inc_dir.zz}',
        '%{inc_dir.entt}'
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
        '%{inc_dir.zz}',
        '%{inc_dir.entt}'
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

project 'ex.window'
    location 'window/'
    kind 'consoleapp'

    dependson { 'zenzebu' }

    language 'c++'
    cppdialect 'c++17'

    targetdir (tgt_dir)
    objdir    (obj_dir)

    files {
        'window/src/**.cc'
    }

    includedirs {
        '%{inc_dir.zz}',
        '%{inc_dir.entt}',
        '%{inc_dir.glfw}'
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
