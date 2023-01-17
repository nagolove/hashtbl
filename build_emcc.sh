#!/usr/bin/env bash

emcc -c hashtbl.c

emar rcs libhashtbl.a hashtbl.o
