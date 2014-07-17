include(MacroLogFeature)

find_package(Qt4 4.8.0 COMPONENTS QtCore QtGui QtSql)
macro_log_feature(QT4_FOUND "Qt 4" "Qt 4 framework" "http://qt-project.org/" TRUE 4.8.0)

find_package(LibUSB 1.0)
macro_log_feature(LIBUSB_FOUND "libusb" "Userspace access to USB devices" "http://libusb.sourceforge.net" TRUE 1.0)

find_package(Doxygen)
macro_log_feature(DOXYGEN_FOUND "Doxygen" "Documentation system" "http://www.doxygen.org/" FALSE)

macro_display_feature_log()
