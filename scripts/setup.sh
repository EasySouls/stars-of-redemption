#!/bin/bash

pushd ..
Walnut/vendor/bin/premake/Linux/premake5 --cc=clang --file=build-headless.lua gmake2
popd