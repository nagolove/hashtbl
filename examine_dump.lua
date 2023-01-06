#!/usr/bin/env lua
local inspect = require "inspect"
--print('arg', inspect(arg))

local file = io.open(arg[1], 'r')
if not file then
    printf("could not open file")
    os.exit()
end
file:close()

local data = io.open(arg[1], "r"):read("*a")
local dump = load("return " .. data)()
--print('dump', inspect(dump))
local longest_chain = 0
local loaded = 0;
for k, v in ipairs(dump) do
    if v > longest_chain then
        longest_chain = v
    end
    if v > 0 then
        loaded = loaded + 1
    end
end
print('longest chain', longest_chain)
print('loaded', loaded)
print('array len', dump.len)
print('loaded %', loaded / dump.len)
