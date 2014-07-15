set(SRCS
    UserMeasurement.cpp
    UserData.cpp

    UserDataDB.cpp
)
set(HDRS
    UserMeasurement.hpp
    UserData.hpp

    UserDataDB.hpp
)

qt4_wrap_cpp(SRCS ${HDRS})
add_library(Data OBJECT ${SRCS})

add_subdirectory(Models)

set(BSM_SRCS ${BSM_SRCS} $<TARGET_OBJECTS:Data> PARENT_SCOPE)
