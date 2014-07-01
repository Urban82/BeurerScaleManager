set(SRCS
    UsbDownloader.cpp
)
set(HDRS
    UsbDownloader.hpp
)
set(UIS
)
list_add_prefix(SRCS ${CMAKE_CURRENT_SOURCE_DIR}/)
list_add_prefix(HDRS ${CMAKE_CURRENT_SOURCE_DIR}/)
list_add_prefix(UIS ${CMAKE_CURRENT_SOURCE_DIR}/)

set(BSM_SRCS ${BSM_SRCS} ${SRCS} PARENT_SCOPE)
set(BSM_HDRS ${BSM_HDRS} ${HDRS} PARENT_SCOPE)
set(BSM_UIS ${BSM_UIS} ${UIS} PARENT_SCOPE)
