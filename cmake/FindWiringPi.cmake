find_path(
        WIRINGPI_INCLUDE_DIRS
        NAMES wiringPi.h
        HINTS ${WIRINGPI_ROOT_DIR} ${WIRINGPI_ROOT_DIR}/wiringPi
)
find_library(
        WIRINGPI_LIBRARIES
        NAMES wiringPi
        HINTS ${WIRINGPI_ROOT_DIR} ${WIRINGPI_ROOT_DIR}/wiringPi
)


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(wiringPi DEFAULT_MSG WIRINGPI_LIBRARIES WIRINGPI_INCLUDE_DIRS)
