

set(main_window_source
  main_window.cpp
  ruler_widget.cpp
  qnetwork.cpp
  main.cpp
)

set(main_window_header
  main_window.hpp
  ruler_widget.hpp
)

QT4_WRAP_CPP(main_window_header_moc ${main_window_header})

ADD_EXECUTABLE(main_window 
  ${main_window_source}
  ${main_window_header_moc}
)

ADD_EXECUTABLE(test test_main.cpp)
ADD_EXECUTABLE(sfsw sfsw_main.cpp)

TARGET_LINK_LIBRARIES(test
  network
)

TARGET_LINK_LIBRARIES(sfsw
  network
)

TARGET_LINK_LIBRARIES(main_window
  ${QT_LIBRARIES}
  network
)

if(CMAKE_COMPILER_IS_GNUCXX)
  add_definitions(-std=c++11)
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-Wall -g)
  else()#Release mode
    #common optimizers
    add_definitions(-fno-exceptions -fopenmp -O3)
    if(WIN32)

    elseif(APPLE)
      
    else()
    #linux
    add_definitions(-march=native -mtune=native -fguess-branch-probability -finline-functions -finline-functions-called-once -fjump-tables -fschedule-insns2 -ftree-vectorize -fprefetch-loop-arrays -fivopts -ftree-loop-linear -ftree-loop-im -ftree-loop-ivcanon -mfpmath=sse,387 -fomit-frame-pointer)
    endif()
  endif()
endif()