# Copyright (c) 2017 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license that
# can be found in the LICENSE file.

#
# Source files.
#

# Main executable sources.
set(ABBY_SRCS
  app_browser_minimal.cc
  client_minimal.cc
  client_minimal.h
  main_minimal.cc
  )
set(ABBY_SRCS_WINDOWS
  resource.h
  )
APPEND_PLATFORM_SOURCES(ABBY_SRCS)
  
if(OS_LINUX OR OS_WINDOWS)
  # On Windows and Linux the same executable is used for all processes.
  set(ABBY_SRCS
    ${ABBY_SRCS}
    app_other_minimal.cc
    app_renderer_minimal.cc
    )
elseif(OS_MAC)
  # On macOS a separate helper executable is used for subprocesses.
  set(ABBY_HELPER_SRCS
    app_other_minimal.cc
    app_renderer_minimal.cc
    process_helper_mac_minimal.cc
    )
endif()

# Main executable resources.
set(ABBY_RESOURCES_SRCS
  )
set(ABBY_RESOURCES_SRCS_WINDOWS
  # Resources that embed the *.html and *.png files in the executable.
  win/resource.rc
  )
APPEND_PLATFORM_SOURCES(ABBY_RESOURCES_SRCS)

if(OS_MACOSX OR OS_WINDOWS)
  # On macOS and Windows include the shared resources.
  set(ABBY_RESOURCES_SRCS
    ${ABBY_RESOURCES_SRCS}
    ${SHARED_RESOURCES_SRCS}
    )
endif()


#
# Shared configuration.
#

# Target executable names.
set(ABBY_TARGET "minimal")
if(OS_MAC)
  set(ABBY_HELPER_TARGET "minimal_Helper")
  set(ABBY_HELPER_OUTPUT_NAME "minimal Helper")
endif()


#
# Linux configuration.
#

if(OS_LINUX)
  # Executable target.
  add_executable(${ABBY_TARGET} ${ABBY_SRCS})
  SET_ABBY_EXECUTABLE_TARGET_PROPERTIES(${ABBY_TARGET})
  add_dependencies(${ABBY_TARGET} shared)
  target_link_libraries(${ABBY_TARGET} shared)
endif()


#
# Mac OS X configuration.
#

if(OS_MAC)
  # Create source groups for Xcode.
  SET_ABBY_SOURCE_GROUPS("${ABBY_SRCS}")
  SET_ABBY_SOURCE_GROUPS("${ABBY_HELPER_SRCS}")

  # Output path for the main app bundle.
  set(ABBY_APP "${ABBY_TARGET_OUT_DIR}/${ABBY_TARGET}.app")

  # Variables referenced from the main Info.plist file.
  set(EXECUTABLE_NAME "${ABBY_TARGET}")
  set(PRODUCT_NAME "${ABBY_TARGET}")

  # Main app bundle target.
  add_executable(${ABBY_TARGET} MACOSX_BUNDLE ${ABBY_SRCS} ${ABBY_RESOURCES_SRCS})
  SET_ABBY_EXECUTABLE_TARGET_PROPERTIES(${ABBY_TARGET})
  add_dependencies(${ABBY_TARGET} shared)
  target_link_libraries(${ABBY_TARGET} shared)
  set_target_properties(${ABBY_TARGET} PROPERTIES
    RESOURCE "${ABBY_RESOURCES_SRCS}"
    MACOSX_BUNDLE_INFO_PLIST "${SHARED_INFO_PLIST}"
    )

  # Copy the CEF framework into the Frameworks directory and create the necessary symlinks.
  COPY_MAC_FRAMEWORK("${ABBY_TARGET}" "${CEF_BINARY_DIR}" "${ABBY_APP}")

  # Create the multiple Helper app bundle targets.
  foreach(_suffix_list ${CEF_HELPER_APP_SUFFIXES})
    # Convert to a list and extract the suffix values.
    string(REPLACE ":" ";" _suffix_list ${_suffix_list})
    list(GET _suffix_list 0 _name_suffix)
    list(GET _suffix_list 1 _target_suffix)
    list(GET _suffix_list 2 _plist_suffix)

    # Define Helper target and output names.
    set(_helper_target "${ABBY_HELPER_TARGET}${_target_suffix}")
    set(_helper_output_name "${ABBY_HELPER_OUTPUT_NAME}${_name_suffix}")

    # Create Helper-specific variants of the helper-Info.plist file. Do this
    # manually because the configure_file command (which is executed as part of
    # MACOSX_BUNDLE_INFO_PLIST) uses global env variables and would insert the
    # wrong values with multiple targets.
    set(_helper_info_plist "${CMAKE_CURRENT_BINARY_DIR}/helper-Info${_target_suffix}.plist.in")
    file(READ "${SHARED_HELPER_INFO_PLIST}" _plist_contents)
    string(REPLACE "\${EXECUTABLE_NAME}" "${_helper_output_name}" _plist_contents ${_plist_contents})
    string(REPLACE "\${PRODUCT_NAME}" "${_helper_output_name}" _plist_contents ${_plist_contents})
    string(REPLACE "\${BUNDLE_ID_SUFFIX}" "${_plist_suffix}" _plist_contents ${_plist_contents})
    file(WRITE ${_helper_info_plist} ${_plist_contents})

    # Create Helper executable target.
    add_executable(${_helper_target} MACOSX_BUNDLE ${ABBY_HELPER_SRCS})
    SET_ABBY_EXECUTABLE_TARGET_PROPERTIES(${_helper_target})
    add_dependencies(${_helper_target} shared_helper)
    target_link_libraries(${_helper_target} shared_helper)
    set_target_properties(${_helper_target} PROPERTIES
      MACOSX_BUNDLE_INFO_PLIST ${_helper_info_plist}
      OUTPUT_NAME ${_helper_output_name}
      )

    # Add the Helper as a dependency of the main executable target.
    add_dependencies(${ABBY_TARGET} "${_helper_target}")

    # Copy the Helper app bundle into the Frameworks directory.
    add_custom_command(
      TARGET ${ABBY_TARGET}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory
              "${ABBY_TARGET_OUT_DIR}/${_helper_output_name}.app"
              "${ABBY_APP}/Contents/Frameworks/${_helper_output_name}.app"
      VERBATIM
      )
  endforeach()

  # Manually process and copy over resource files.
  # The Xcode generator can support this via the set_target_properties RESOURCE
  # directive but that doesn't properly handle nested resource directories.
  # Remove these prefixes from input file paths.
  set(PREFIXES
    "../shared/mac/"
    )
  COPY_MAC_RESOURCES("${ABBY_RESOURCES_SRCS}" "${PREFIXES}" "${ABBY_TARGET}" "${CMAKE_CURRENT_SOURCE_DIR}" "${ABBY_APP}")
endif()


#
# Windows configuration.
#

if(OS_WINDOWS)
    # Add resources to the sources variable for convenience.
  set(ABBY_SRCS
    ${ABBY_SRCS}
    ${ABBY_RESOURCES_SRCS}
    )

  # Create source groups for Visual Studio.
  SET_ABBY_SOURCE_GROUPS("${ABBY_SRCS}")

  if(USE_SANDBOX)
    # Shared library (DLL) target.
    add_library(${ABBY_TARGET} SHARED ${ABBY_SRCS})
    SET_ABBY_LIBRARY_TARGET_PROPERTIES(${ABBY_TARGET})

    # Copy and rename the bootstrap binary.
    COPY_SINGLE_FILE(${ABBY_TARGET} ${CEF_BINARY_DIR}/bootstrap.exe ${ABBY_TARGET_OUT_DIR}/${ABBY_TARGET}.exe)
    if (CMAKE_GENERATOR MATCHES "Visual Studio")
      # Make the bootstrap binary the default command for the Visual Studio debugger.
      set_target_properties(${ABBY_TARGET} PROPERTIES
        VS_DEBUGGER_COMMAND "$<TARGET_FILE_DIR:${ABBY_TARGET}>/${ABBY_TARGET}.exe"
      )
    endif()
  else()
    # Executable target (sandbox disabled).
    add_executable(${ABBY_TARGET} WIN32 ${ABBY_SRCS})
    SET_ABBY_EXECUTABLE_TARGET_PROPERTIES(${ABBY_TARGET})
  endif()

  add_dependencies(${ABBY_TARGET} shared)
  target_link_libraries(${ABBY_TARGET} shared)
endif()
