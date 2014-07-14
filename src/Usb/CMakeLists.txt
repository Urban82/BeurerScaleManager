set(SRCS
    UsbDownloader.cpp
    UsbData.cpp
)
set(HDRS
    UsbDownloader.hpp
    UsbData.hpp
)

qt4_wrap_cpp(SRCS ${HDRS})
add_library(Usb OBJECT ${SRCS})
set(BSM_SRCS ${BSM_SRCS} $<TARGET_OBJECTS:Usb> PARENT_SCOPE)
