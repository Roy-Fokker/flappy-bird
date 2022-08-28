# find all libraries defined in vcpkg
find_package(fmt CONFIG REQUIRED)
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

# update executable name
add_executable(flappy-bird)

# set C++ standard to use
target_compile_features(flappy-bird
	PRIVATE 
		cxx_std_20)

# set preprocessor defines for executable
target_compile_definitions(flappy-bird
	PRIVATE
		UNICODE _UNICODE 
		_SILENCE_CXX17_C_HEADER_DEPRECATION_WARNING 
		NOMINMAX 
		WIN32_LEAN_AND_MEAN)

# executable specific target options
target_link_options(flappy-bird
	PRIVATE
	$<$<CXX_COMPILER_ID:MSVC>:/entry:mainCRTStartup>)

# libraries that are used by executable
target_link_libraries(flappy-bird
	PRIVATE
		fmt::fmt
		sfml-system sfml-network sfml-graphics sfml-window)

# Use Precompiled headers for std/os stuff
target_precompile_headers(flappy-bird
	PRIVATE
		pch.h)

# sources to be used
target_sources(flappy-bird
	PRIVATE
		main.cpp
		flappy.cpp)