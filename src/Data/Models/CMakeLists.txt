set(SRCS
    UserDataModel.cpp
    UserMeasurementModel.cpp
)
set(HDRS
    UserDataModel.hpp
    UserMeasurementModel.hpp
)

qt4_wrap_cpp(SRCS ${HDRS})
add_library(DataModels OBJECT ${SRCS})
set(BSM_SRCS ${BSM_SRCS} $<TARGET_OBJECTS:DataModels> PARENT_SCOPE)
