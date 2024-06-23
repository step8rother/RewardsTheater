#!/usr/bin/env zsh

if [[ ! -e ./CMakeLists.txt ]]; then
  echo "cwd should be the plugin directory!"
  exit 1
fi

project_root=$(pwd)

export MACOSX_DEPLOYMENT_TARGET=11.0

if [[ ! -d ./.deps/boost ]]; then
  echo "Building Boost universal binary"
  mkdir -p .deps/boost
  pushd .deps/boost
  curl https://archives.boost.io/release/1.85.0/source/boost_1_85_0.tar.gz > boost.tar.gz
  tar -xzf boost.tar.gz --strip-components=1
  rm boost.tar.gz
  cp ${project_root}/.github/scripts/utils.zsh/macos-boost.zsh .
  ./macos-boost.zsh
  popd
else
  echo "Boost directory exists, skipping build"
fi

if [[ ! -d ./.deps/openssl ]]; then
  # Based on https://stackoverflow.com/a/75250222/6120487
  echo "Building OpenSSL universal binary"
  mkdir -p .deps/openssl
  pushd .deps/openssl
  curl https://www.openssl.org/source/openssl-3.3.1.tar.gz > openssl.tar.gz
  tar -xzf openssl.tar.gz --strip-components=1
  rm openssl.tar.gz
  cp ${project_root}/.github/scripts/utils.zsh/macos-openssl.zsh .
  export CC=$(pwd)/macos-openssl.zsh
  ./Configure enable-rc5 zlib no-asm darwin64-x86_64-cc
  make -j16
  popd
else
  echo "OpenSSL directory exists, skipping build"
fi
