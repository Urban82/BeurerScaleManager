set(SRCS
    UserMeasurement.cpp
    UserData.cpp

    UserDataModel.cpp
    UserMeasurementModel.cpp
)
set(HDRS
    UserMeasurement.hpp
    UserData.hpp

    UserDataModel.hpp
    UserMeasurementModel.hpp
)

qt4_wrap_cpp(SRCS ${HDRS})
add_library(Data OBJECT ${SRCS})
set(BSM_SRCS ${BSM_SRCS} $<TARGET_OBJECTS:Data> PARENT_SCOPE)
