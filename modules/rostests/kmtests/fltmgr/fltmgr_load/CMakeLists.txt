
include_directories(../../include)

list(APPEND FLTMGR_TEST_DRV_SOURCE
    ../../kmtest_drv/kmtest_fsminifilter.c
    fltmgr_load.c)

add_library(fltmgr_load SHARED ${FLTMGR_TEST_DRV_SOURCE})
set_module_type(fltmgr_load kernelmodedriver)
target_link_libraries(fltmgr_load kmtest_printf ${PSEH_LIB})
add_importlibs(fltmgr_load fltmgr ntoskrnl hal)
add_target_compile_definitions(fltmgr_load KMT_STANDALONE_DRIVER KMT_FILTER_DRIVER NTDDI_VERSION=NTDDI_WS03SP1)
#add_pch(example_drv ../include/kmt_test.h)
add_rostests_file(TARGET fltmgr_load)
