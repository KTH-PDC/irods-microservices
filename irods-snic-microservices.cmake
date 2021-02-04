set(
  MICROSERVICES
  msitemplate
  )

set(
  IRODS_COMPILE_DEFINITIONS_MICROSERVICES
  RODS_SERVER
  ENABLE_RE
  )

foreach(MICROSERVICE ${MICROSERVICES})
  add_library(
    ${MICROSERVICE}
    MODULE
    ${CMAKE_SOURCE_DIR}/${MICROSERVICE}/lib${MICROSERVICE}.cpp
    )

  target_include_directories(
    ${MICROSERVICE}
    PRIVATE
    ${IRODS_INCLUDE_DIRS}
    ${IRODS_EXTERNALS_FULLPATH_BOOST}/include
    ${IRODS_EXTERNALS_FULLPATH_JSON}/include
    )

  target_link_libraries(
    ${MICROSERVICE}
    PRIVATE
    irods_server
    irods_client
    irods_common
    ${IRODS_EXTERNALS_FULLPATH_BOOST}/lib/libboost_filesystem.so
    ${IRODS_EXTERNALS_FULLPATH_BOOST}/lib/libboost_system.so
    )

  target_compile_definitions(${MICROSERVICE} PRIVATE ${IRODS_COMPILE_DEFINITIONS_MICROSERVICES} ${IRODS_COMPILE_DEFINITIONS} BOOST_SYSTEM_NO_DEPRECATED)
  set_property(TARGET ${MICROSERVICE} PROPERTY CXX_STANDARD ${IRODS_CXX_STANDARD})

  install(
    TARGETS
    ${MICROSERVICE}
    LIBRARY
    DESTINATION usr/lib/irods/plugins/microservices
    )
endforeach()

install(
  FILES
  ${CMAKE_SOURCE_DIR}/template_rule.r
  DESTINATION var/lib/irods
  )
