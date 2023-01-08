workspace "hashtbl"
    configurations { "Debug", "Release", "Sanitizer" }

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

    local include_prefix = 
[[struct HashTestCase {
    int minlen, maxlen, count;
    struct Pair *ht;
};
struct HashTestCase hash_data_cases[] = {
]]
    local include_vars = {}
    local include_suffix = [[
};]]

    local function command(minlen, maxlen, count)
        local cmd = "lua ./gen_random_strings "
        --local fmt = "minlen=%d maxlen=%d count=%d suffix=\\\"_rnd_%d_%d_%d\\\""
        local fmt = "minlen=%d maxlen=%d count=%d"
        return cmd .. string.format(
            fmt, 
            minlen, maxlen, count,
            minlen, maxlen, count)
    end

    local tests_used = false

    local function test_case(minlen, maxlen, count)
        assert(minlen > 0)
        assert(maxlen > minlen)
        assert(count > 0)
        tests_used = true

        include_prefix = include_prefix .. string.format([[
    { 
        .minlen =  %d, 
        .maxlen = %d, 
        .count = %d, 
        .ht = data_%d_%d_%d,
    }, ]],  minlen, maxlen, count,
            minlen, maxlen, count)
        --prebuildcommands(command(minlen, maxlen, count))
        
        local var_name = string.format(
            "data_%d_%d_%d", minlen, maxlen, count
        )
        
        local accum = {}
        local file = io.popen(command(minlen, maxlen, count), "r");
        print('-----------------------')
        for k, v in file:lines() do
            --print(k)
            --accum = accum .. k .. '\n'
            --accum = accum .. k .. '\n'
            table.insert(accum, k)
            table.insert(accum, '\n')
        end
        table.insert(include_vars, {
            var_name = var_name, 
            data = table.concat(accum)
        });
    end

    local function write_test_cases()
        if tests_used then
            file = io.open("test_cases.h", "w")

            for k, v in pairs(include_vars) do
                file:write(
                    string.format([[
static struct Pair %s[] = {
                    ]], v.var_name)
                )
                print(v.var_name)
                file:write(v.data)
                file:write([[};
]])
            end

            file:write(include_prefix .. include_suffix)
            file:close()
        end
    end

    project "hashtbl_test"

        --test_case(3, 5, 100)

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

        write_test_cases()

        files { "**.h", "hashtbl_test.c", "munit.c", "hashtbl.c"}
        
    filter "configurations:Sanitizer"
    defines { "DEBUG" }
    symbols "On"
    linkoptions {
        "-fsanitize=address",
    }
    buildoptions { 
        "-fsanitize=address",
    }
    --]]

    filter "configurations:Debug"
    defines { "DEBUG" }
    symbols "On"
    --[[
    linkoptions {
        "-fsanitize=address",
    }
    buildoptions { 
        "-fsanitize=address",
    }
    --]]

    filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "On"
    print('RELEASE mode')
