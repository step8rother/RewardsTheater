#!/bin/bash

if [[ ! -e ./CMakeLists.txt ]]; then
  echo "cwd should be the plugin directory!"
  exit 1
fi

if [[ ! -d ./boost ]]; then
  echo "Building Boost"
  mkdir boost
  pushd boost
  wget -O boost.tar.gz https://archives.boost.io/release/1.85.0/source/boost_1_85_0.tar.gz
  tar -xzf boost.tar.gz --strip-components=1
  rm boost.tar.gz
  ./bootstrap.sh link=static
  ./b2
  popd
else
  echo "Boost directory exists, skipping build"
fi
