#find_package( Boost 1.67 )
#include_directories( ${Boost_INCLUDE_DIR} )
add_library(potentialfields "")
add_library(planning::potentialfields ALIAS potentialfields)
target_sources(potentialfields
        PRIVATE
            PotentialFieldsPlanner.cpp
        PUBLIC
         #   ${CMAKE_CURRENT_LIST_DIR}/PotentialFieldsPlanner.h
        )
target_include_directories(potentialfields
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>

        )
target_link_libraries(potentialfields utils iplanner)


install(TARGETS potentialfields EXPORT planning-export DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES ${CMAKE_CURRENT_LIST_DIR}/PotentialFieldsPlanner.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
