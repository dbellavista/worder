ADD_EXECUTABLE (test_elements test_main.c)

TARGET_LINK_LIBRARIES (test_elements worder)

INSTALL (TARGETS test_elements DESTINATION ${Worder_BIN_DIR})
