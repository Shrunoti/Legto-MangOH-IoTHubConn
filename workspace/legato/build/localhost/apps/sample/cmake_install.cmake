# Install script for directory: /home/shrunoti/Downloads/workspace/legato/apps/sample

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/shrunoti/Downloads/workspace/legato/build/localhost/apps/sample/helloWorld/cmake_install.cmake")
  include("/home/shrunoti/Downloads/workspace/legato/build/localhost/apps/sample/javaHelloWorld/cmake_install.cmake")
  include("/home/shrunoti/Downloads/workspace/legato/build/localhost/apps/sample/helloIpc/cmake_install.cmake")
  include("/home/shrunoti/Downloads/workspace/legato/build/localhost/apps/sample/legacy/cmake_install.cmake")
  include("/home/shrunoti/Downloads/workspace/legato/build/localhost/apps/sample/modemDemo/cmake_install.cmake")
  include("/home/shrunoti/Downloads/workspace/legato/build/localhost/apps/sample/commandLine/cmake_install.cmake")
  include("/home/shrunoti/Downloads/workspace/legato/build/localhost/apps/sample/eCallDemo/cmake_install.cmake")
  include("/home/shrunoti/Downloads/workspace/legato/build/localhost/apps/sample/sms/cmake_install.cmake")
  include("/home/shrunoti/Downloads/workspace/legato/build/localhost/apps/sample/shellPiping/cmake_install.cmake")
  include("/home/shrunoti/Downloads/workspace/legato/build/localhost/apps/sample/assetData/cmake_install.cmake")
  include("/home/shrunoti/Downloads/workspace/legato/build/localhost/apps/sample/avcControl/cmake_install.cmake")
  include("/home/shrunoti/Downloads/workspace/legato/build/localhost/apps/sample/textLoc/cmake_install.cmake")
  include("/home/shrunoti/Downloads/workspace/legato/build/localhost/apps/sample/watchdog/cmake_install.cmake")
  include("/home/shrunoti/Downloads/workspace/legato/build/localhost/apps/sample/streamMedia/cmake_install.cmake")
  include("/home/shrunoti/Downloads/workspace/legato/build/localhost/apps/sample/httpGet/cmake_install.cmake")

endif()

