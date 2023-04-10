cmake_minimum_required(VERSION 3.16...3.26)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/common/bootstrap.cmake" NO_POLICY_SCOPE)

project(${_name} VERSION ${_version})

option(ENABLE_FRONTEND_API "Use obs-frontend-api for UI functionality" ON)
option(ENABLE_QT "Use Qt functionality" ON)

include(compilerconfig)
include(defaults)
include(helpers)

add_library(${CMAKE_PROJECT_NAME} MODULE)

# Add your custom source files here - header files are optional and only required for visibility
# e.g. in Xcode or Visual Studio
target_sources(
  ${CMAKE_PROJECT_NAME}
  PRIVATE src/RewardsTheaterMain.cpp
          src/SettingsDialog.cpp
          src/SettingsDialog.h
          src/TwitchAuth.cpp
          src/TwitchAuth.h
          src/Reward.h
          src/RewardsQueue.h
          src/RewardsQueue.cpp
          src/TwitchRewardsApi.h
          src/TwitchRewardsApi.cpp
          src/AuthenticateWithTwitchDialog.cpp
          src/AuthenticateWithTwitchDialog.h
          src/RewardsTheaterPlugin.cpp
          src/RewardsTheaterPlugin.h
          src/Settings.h
          src/Settings.cpp
          src/TwitchApi.h
          src/TwitchApi.cpp
          src/Log.h
          src/BoostAsio.h)

set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED 20)

# Libraries from vcpkg
find_package(Boost REQUIRED COMPONENTS system url)
find_package(OpenSSL REQUIRED)
find_package(fmt REQUIRED)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC Boost::system Boost::url OpenSSL::SSL
                                                   OpenSSL::Crypto fmt::fmt)

find_package(libobs REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE OBS::libobs)

if(ENABLE_FRONTEND_API)
  find_package(obs-frontend-api REQUIRED)
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE OBS::obs-frontend-api)
endif()

if(ENABLE_QT)
  find_qt(COMPONENTS Widgets Core)
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt::Core Qt::Widgets)
  target_compile_options(
    ${CMAKE_PROJECT_NAME} PRIVATE $<$<C_COMPILER_ID:Clang,AppleClang>:-Wno-quoted-include-in-framework-header
                                  -Wno-comma>)
  set_target_properties(
    ${CMAKE_PROJECT_NAME}
    PROPERTIES AUTOMOC ON
               AUTOUIC ON
               AUTORCC ON)
endif()


set_target_properties_plugin(${CMAKE_PROJECT_NAME} PROPERTIES OUTPUT_NAME ${_name})
