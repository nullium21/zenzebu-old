project 'zenzebu'
    kind 'sharedlib'

    -- dependson {}

    language 'c++'
    cppdialect 'c++17'

    targetdir (tgt_dir)
    objdir    (obj_dir)

    files {
        'src/**.cc', 'src/**.h'
    }

    includedirs {}

    filter "system:linux"
        defines { "ZZ_LINUX", "ZZ_SHAREDLIB" }

    filter "configurations:debug"
        defines { "ZZ_DEBUG" }
        symbols "on"

    filter "configurations:release"
        defines { "ZZ_RELEASE" }
        optimize "on"
