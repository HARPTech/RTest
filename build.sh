#! /usr/bin/env bash

# Build Docker Image
docker build -t rtest-amd64 .
docker run --rm rtest-amd64 > ./rtest-amd64
chmod +x ./rtest-amd64

# Configure
./rtest-amd64 cmake -BbuildLinuxAmd64 -H. -G"Unix Makefiles" -DBUILD_NUMBER=$BUILD_NUMBER -DBUILD_URL=$BUILD_URL -DCMAKE_BUILD_TYPE=Release

# Build
./rtest-amd64 make -C buildLinuxAmd64 -j 2

# Package
./rtest-amd64 make -C buildLinuxAmd64 -j 2 package
