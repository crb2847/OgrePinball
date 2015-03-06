cmake_minimum_required(VERSION 2.6)
 
project(OgrePinball)

if(EXISTS "/usr/local/lib/OGRE/cmake")
  set(CMAKE_MODULE_PATH "/usr/local/lib/OGRE/cmake/;/usr/local/lib/OgreBullet/cmake/;${CMAKE_MODULE_PATH}")
  set(OGRE_SAMPLES_INCLUDEPATH "/usr/local/share/OGRE/samples/Common/include/")
elseif(EXISTS "/usr/lib/OGRE/cmake")
  set(CMAKE_MODULE_PATH "/usr/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
  set(OGRE_SAMPLES_INCLUDEPATH "/usr/share/OGRE/samples/Common/include/")
elseif(EXISTS "/lusr/opt/ogre-1.9/lib/OGRE/cmake")
  # CS machines
  set(CMAKE_MODULE_PATH "/lusr/opt/ogre-1.9/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
endif(EXISTS "/usr/local/lib/OGRE/cmake")

# ogreoggsound stuff
set(OGREOGGSOUND_DIR ${CMAKE_SOURCE_DIR}/ogreoggsound)
set(OGREOGGSOUND_INCLUDE_DIRS ${OGREOGGSOUND_DIR}/include)
set(OGREOGGSOUND_LIBRARIES ${OGREOGGSOUND_DIR}/lib/libOgreOggSound.so)
link_directories(ogreoggsound/lib)
  
# OgreBullet stuff
set(OGRE_BULLET_DIR ${CMAKE_SOURCE_DIR}/ogrebullet)
set(OGRE_BULLET_INCLUDE_DIRS
  ${OGRE_BULLET_DIR}/include/Collisions
  ${OGRE_BULLET_DIR}/include/Dynamics)
set(OGRE_BULLET_LIBRARIES
  ${OGRE_BULLET_DIR}/lib/libOgreBulletDynamics.so
  ${OGRE_BULLET_DIR}/lib/libOgreBulletCollisions.so)
link_directories(ogrebullet/lib)

if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()
 
set(CMAKE_DEBUG_POSTFIX "_d")
 
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/dist")
 
find_package(OGRE REQUIRED)

find_package(OIS REQUIRED)

set (CMAKE_INCLUDE_PATH /usr/include/bullet)
find_package(Bullet REQUIRED)
message(STATUS "BULLET_INCLUDE_DIR " ${BULLET_INCLUDE_DIR})

# Find Boost
set(Boost_USE_STATIC_LIBS ${OGRE_STATIC})
set(Boost_ADDITIONAL_VERSIONS "1.44" "1.44.0" "1.42" "1.42.0" "1.41.0" "1.41" "1.40.0" "1.40" "1.39.0" "1.39" "1.38.0" "1.38" "1.37.0" "1.37" )
# Components that need linking (NB does not include header-only components like bind)
set(OGRE_BOOST_COMPONENTS thread date_time)
find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
if (NOT Boost_FOUND)
	# Try again with the other type of libs
	set(Boost_USE_STATIC_LIBS NOT ${Boost_USE_STATIC_LIBS})
	find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
endif()
find_package(Boost QUIET)
 
# Set up referencing of Boost
include_directories(${Boost_INCLUDE_DIR})
add_definitions(-DBOOST_ALL_NO_LIB)
set(OGRE_LIBRARIES ${OGRE_LIBRARIES} ${Boost_LIBRARIES})

include_directories( 
  ${OGREOGGSOUND_INCLUDE_DIRS}
  ${OGRE_BULLET_INCLUDE_DIRS}
  ${OIS_INCLUDE_DIRS}
	${OGRE_INCLUDE_DIRS}
	${BULLET_INCLUDE_DIRS}
	${OGRE_SAMPLES_INCLUDEPATH}
	${OGRE_Overlay_INCLUDE_DIRS}
)

FILE(GLOB SRCS code/*.cpp)
FILE(GLOB HDRS code/*.hpp)
 
add_executable(OgrePinball WIN32 ${HDRS} ${SRCS})
set_target_properties(OgrePinball PROPERTIES DEBUG_POSTFIX _d)
target_link_libraries(OgrePinball ${OGREOGGSOUND_LIBRARIES} ${OGRE_BULLET_LIBRARIES} ${BULLET_LIBRARIES} ${OGRE_LIBRARIES} ${OIS_LIBRARIES} ${OGRE_Overlay_LIBRARIES})

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/media)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

install(TARGETS OgrePinball
	RUNTIME DESTINATION bin
	CONFIGURATIONS All)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/media
	DESTINATION ./
	CONFIGURATIONS Release RelWithDebInfo Debug
)

install(FILES ${CMAKE_SOURCE_DIR}/plugins.cfg
	${CMAKE_SOURCE_DIR}/resources.cfg
	DESTINATION bin
	CONFIGURATIONS Release RelWithDebInfo Debug
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

