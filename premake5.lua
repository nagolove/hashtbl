workspace "hashtbl"
    configurations { "Debug", "Release" }

    includedirs {}
    buildoptions { 
        "-ggdb3",
        "-fPIC",
        "-Wall",
        "-Werror",
        "-Wno-strict-aliasing",
        "-Wno-unused-function",
        "-Wno-unused-variable",
    }
    links {}
    libdirs {}
    language "C"
    kind "ConsoleApp"
    targetprefix ""
    targetdir "."

    local include_prefix = [[
        struct HashTestCase {
            int minlen, maxlen, count;
            struct HashTest *ht;
        };
        struct HashTestCase hash_data_cases[] = {
    ]]
    local include_suffix = [[
        };
    ]]

    local function command(minlen, maxlen, count)
        local cmd = "lua ./gen_random_strings "
        local fmt = "minlen=%d maxlen=%d count=%d suffix=\\\"_rnd_%d_%d_%d\\\""
        return cmd .. string.format(
            fmt, 
            minlen, maxlen, count,
            minlen, maxlen, count)
    end

    local function test_case(minlen, maxlen, count)
        assert(minlen > 0)
        assert(maxlen > minlen)
        assert(count > 0)
        include_prefix = include_prefix .. string.format([[
            { .minlen =  %d, .maxlen = %d, .count = %d, .ht = {
            #include "string_data_rnd_%d_%d_%d.h"
            } },
        ]], minlen, maxlen, count,
            minlen, maxlen, count)
        prebuildcommands(command(minlen, maxlen, count))
    end

    project "hash_tbl_test"

        --test_case(3, 5, 10)

        --[[
        test_case(3, 5, 1000)
        test_case(3, 5, 10000)
        test_case(3, 5, 100000)
        test_case(3, 15, 1000)
        test_case(3, 15, 10000)
        test_case(3, 15, 100000)
        test_case(3, 50, 1000)
        test_case(3, 50, 10000)
        test_case(3, 50, 100000)
        --]]

        file = io.open("test_cases.h", "w")
        file:write(include_prefix .. include_suffix)
        file:close()

        files { "**.h", "hash_tbl_test.c", "munit.c", "hash_tbl.c"}

    filter "configurations:Debug"
    defines { "DEBUG" }
    symbols "On"

    filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "On"
