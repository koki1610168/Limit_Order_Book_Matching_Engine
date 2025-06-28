# CMake generated Testfile for 
# Source directory: /home/koki/projects/matching-engine
# Build directory: /home/koki/projects/matching-engine/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(OrderBookTests "/home/koki/projects/matching-engine/build/tests")
set_tests_properties(OrderBookTests PROPERTIES  _BACKTRACE_TRIPLES "/home/koki/projects/matching-engine/CMakeLists.txt;59;add_test;/home/koki/projects/matching-engine/CMakeLists.txt;0;")
subdirs("external/googletest")
