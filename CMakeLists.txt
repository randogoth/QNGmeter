option(QNGMETER "Build qngMeter program" ON)

if (QNGMETER)

	FIND_PACKAGE( OpenMP REQUIRED)
	if(OPENMP_FOUND)
		message("OPENMP FOUND")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
	endif()

	ADD_DEFINITIONS("-std=c++0x")

    # Includes
    include_directories( ${CMAKE_CURRENT_BINARY_DIR}
						${CMAKE_CURRENT_SOURCE_DIR}
			)

    message(STATUS "Building qngMeter program.")

    # Source includes
    include_directories(${CMAKE_SOURCE_DIR}/src)
	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/RandTest)

    # Target
    add_executable(qngmeter QNGmeter.cpp RandTest/BiasAndAC.cpp RandTest/Entropy.cpp RandTest/Serial.cpp RandTest/Monkey.cpp 
							RandTest/MonkeyBitmap.cpp RandTest/Bias.cpp RandTest/AutoCorrelation.cpp RandTest/Gamma.cpp 
							RandTest/KolmogorovSmirnov.cpp RandTest/Stat.cpp RandTest/BitCount.cpp)

    # Dependencies
    target_link_libraries(qngmeter)

else(QNGMETER)
    message(STATUS "Not building QngMeter.")
endif(QNGMETER)
