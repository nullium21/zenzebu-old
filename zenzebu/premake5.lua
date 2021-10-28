inc_dir = {}
inc_dir.entt = '%{wks.location}/zenzebu/entt/src'
inc_dir.zz   = '%{wks.location}/zenzebu/src'

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
inc_dir.lua = '%{wks.location}/zenzebu/lua'

project 'zenzebu-glfw'
    kind 'sharedlib'

    language 'c'
    cdialect 'c11'

    targetdir (tgt_dir)
    objdir    (obj_dir)

    files {
        'glfw/include/GLFW/*.h',
        'glfw/src/glfw_config.h',
        'glfw/src/context.c',
        'glfw/src/init.c',
        'glfw/src/input.c',
        'glfw/src/monitor.c',
        'glfw/src/vulkan.c',
        'glfw/src/window.c'
    }

    filter "system:linux"
        files {
            'glfw/src/x11_*.c',
            'glfw/src/xkb_unicode.c',
            'glfw/src/posix_*.c',
            'glfw/src/glx_context.c',
            'glfw/src/egl_context.c',
            'glfw/src/osmesa_context.c',
            'glfw/src/linux_*.c'
        }

        defines { '_GLFW_X11' }
        links { 'X11', 'pthread' }
inc_dir.glfw = '%{wks.location}/zenzebu/glfw/include'

project 'zenzebu-glm'
    kind 'sharedlib'

    language 'c++'
    cppdialect 'c++11'

    targetdir (tgt_dir)
    objdir    (obj_dir)

    files {
        'glm/**.hpp'
    }

inc_dir.glm = '%{wks.location}/zenzebu/glm'

project 'zenzebu-glad'
    kind 'sharedlib'

    language 'c++'
    cppdialect 'c++11'

    targetdir (tgt_dir)
    objdir    (obj_dir)

    files {
        'glad/include/**.h',
        'glad/src/**.c'
    }

    includedirs { 'glad/include' }
inc_dir.glad = '%{wks.location}/zenzebu/glad/include'

project 'zenzebu'
    kind 'sharedlib'

    dependson { 'zenzebu-lua', 'zenzebu-glfw' }

    language 'c++'
    cppdialect 'c++17'

    targetdir (tgt_dir)
    objdir    (obj_dir)

    pchheader "zzpch.h"
    pchheader "src/zzpch.cc"

    files {
        'src/**.cc', 'src/**.h',
        '%{inc_dir.entt}/**.hpp'
    }

    includedirs {
        '%{prj.location}',
        '%{inc_dir.entt}',
        '%{inc_dir.glfw}',
        '%{inc_dir.glad}',
        '%{inc_dir.glm}',
        '%{inc_dir.zz}'
    }

    links { 'zenzebu-lua', 'zenzebu-glfw', 'zenzebu-glad' }

    defines { 'ZZ_USEPCH' }

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
