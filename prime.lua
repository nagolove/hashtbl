#!/usr/bin/env lua

function seq(primes_num)
    local primes = {}
    for i = 2, primes_num do
        primes[#primes + 1] = i
    end
    return primes
end

local inspect = require "inspect"

function eratosfen(primes, value)
    local tmp = {}
    for k, v in pairs(primes) do
        tmp[v] = true
    end

    for i = 1, #primes do
        if primes[i] > value then
            local modulo = math.fmod(primes[i], value)
            if modulo == 0. then
                tmp[primes[i]] = false
            end
        end
    end

    local new = {}
    for k, v in pairs(tmp) do
        if v then
            table.insert(new, k)
        end
    end

    table.sort(new, function(a, b) return a < b end)

    return new
end

function make_primes(max)
    local primes = seq(max)
    --print('seq', inspect(primes))
    for i = 2, max do
        primes = eratosfen(primes, i)
    end
    return primes
end

local primes_100_control = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 
    67, 71, 73, 79, 83, 89, 97
}

--print('result', inspect(make_primes(100)))
--print('result', inspect(make_primes(10000)))

local maxnum = tonumber(arg[1])
if type(maxnum) == 'number' then
    print(inspect(make_primes(maxnum)))
end

--[[
local primes_100_experiment = make_primes(100)
assert(#primes_100_experiment == #primes_100_control)
for k, v in ipairs(primes_100_control) do
    assert(primes_100_experiment[k] == v)
end
--]]
