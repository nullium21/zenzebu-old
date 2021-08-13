project 'zenzebu-lua'
    kind 'sharedlib'

    language 'c'
    cdialect 'c11'

    targetdir (tgt_dir)
    objdir    (obj_dir)

    files {
        'lua/*.c', 'lua/*.h'
    }

    removefiles {
        'lua/onelua.c', 'lua/lua.c',
        'lua/testes/**',
        'lua/manual/**'
    }

    filter "configurations:debug"
        symbols "on"
    filter "configurations:release"
        optimize "on"


project 'zenzebu'
    kind 'sharedlib'

    dependson { 'zenzebu-lua' }

    language 'c++'
    cppdialect 'c++17'

    targetdir (tgt_dir)
    objdir    (obj_dir)

    files {
        'src/**.cc', 'src/**.h',
        'entt/src/**.hpp'
    }

    includedirs {
        '%{prj.location}',
        '%{prj.location}/entt/src',
        '%{prj.location}/src'
    }

    links { 'zenzebu-lua' }

    filter "system:linux"
        defines { "ZZ_LINUX", "ZZ_SHAREDLIB" }

    filter "configurations:debug"
        defines { "ZZ_DEBUG" }
        symbols "on"

    filter "configurations:release"
        defines { "ZZ_RELEASE" }
        optimize "on"
