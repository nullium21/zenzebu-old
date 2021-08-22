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

    pchheader "zzpch.h"
    pchheader "src/zzpch.cc"

    files {
        'src/**.cc', 'src/**.h',
        'entt/single_include/**.hpp'
    }

    includedirs {
        '%{prj.location}',
        '%{prj.location}/entt/single_include',
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

    filter "system:linux"
        files { "platform/linux/**.cc", "platform/linux/**.h" }
    filter "system:windows"
        files { "platform/windows/**.cc", "platform/linux/**.h" }
