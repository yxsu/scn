AUX_SOURCE_DIRECTORY(. SRC_LIST)

ADD_LIBRARY(Math SHARED ${SRC_LIST})

TARGET_LINK_LIBRARIES(Math
  blas
  lapack
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