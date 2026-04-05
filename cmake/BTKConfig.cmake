# *****************************************************************************
# BTK CMake package configuration
#
# This wrapper preserves the existing CopperSpice target exports while exposing
# BTK-branded imported target aliases for downstream consumers.
# *****************************************************************************

if (BTK_FOUND)
   return()
endif()

set(BTK_FOUND TRUE)

get_filename_component(BTK_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" DIRECTORY)
get_filename_component(BTK_PREFIX ${BTK_CMAKE_DIR}/ ABSOLUTE)

list(FIND CMAKE_MODULE_PATH "${BTK_CMAKE_DIR}" BTK_CMAKE_DIR_INDEX)
if (BTK_CMAKE_DIR_INDEX EQUAL -1)
   list(PREPEND CMAKE_MODULE_PATH "${BTK_CMAKE_DIR}")
endif()

include("${BTK_CMAKE_DIR}/CopperSpiceLibraryTargets.cmake")
include("${BTK_CMAKE_DIR}/CopperSpiceBinaryTargets.cmake")
include("${BTK_CMAKE_DIR}/BTKMacros.cmake")
include("${BTK_CMAKE_DIR}/BTKDeploy.cmake")

set(BTK_INCLUDES @CMAKE_INSTALL_FULL_INCLUDEDIR@)
set(BTK_LIBRARIES)
set(BTK_COMPONENTS @BUILD_COMPONENTS@)

foreach(component ${BTK_COMPONENTS})
   string(TOUPPER ${component} uppercomp)

   if (TARGET CopperSpice::Cs${component})
      if (NOT TARGET BTK::Btk${component})
         add_library(BTK::Btk${component} ALIAS CopperSpice::Cs${component})
      endif()

      if (NOT TARGET BTK::${component})
         add_library(BTK::${component} ALIAS CopperSpice::Cs${component})
      endif()

      if (component STREQUAL "Declarative")
         if (NOT TARGET BTK::Bml)
            add_library(BTK::Bml ALIAS CopperSpice::Cs${component})
         endif()
      endif()
   endif()

   set(BTK_INCLUDES
      ${BTK_INCLUDES}
      @CMAKE_INSTALL_FULL_INCLUDEDIR@/Qt${component}
   )

   set(BTK_${uppercomp}_INCLUDES
      @CMAKE_INSTALL_FULL_INCLUDEDIR@/Qt${component}
   )

   set(BTK_LIBRARIES
      ${BTK_LIBRARIES}
      BTK::Btk${component}
   )

   set(BTK_${uppercomp}_LIBRARIES
      BTK::Btk${component}
   )

   set(BTK_${uppercomp}_TARGET BTK::${component})
endforeach()

set(BTK_INSTALL_MODE     "@CS_INSTALL_MODE@")
set(BtkLibGuarded_Deploy "@CsLibGuarded_FOUND@")
set(BtkPointer_Deploy    "@CsPointer_FOUND@")
set(BtkSignal_Deploy     "@CsSignal_FOUND@")
set(BtkString_Deploy     "@CsString_FOUND@")

set(BTK_VERSION_MAJOR "@BUILD_MAJOR@")
set(BTK_VERSION_MINOR "@BUILD_MINOR@")
set(BTK_VERSION_PATCH "@BUILD_MICRO@")
set(BTK_VERSION       "@BUILD_MAJOR@.@BUILD_MINOR@.@BUILD_MICRO@")
set(BTK_VERSION_API   "@BUILD_MAJOR@.@BUILD_MINOR@")

# Compatibility bridge for deploy helpers and downstream CopperSpice-shaped
# tooling which still expects the legacy variable names to exist even when the
# package is discovered through BTK.
set(COPPERSPICE_VERSION_MAJOR "${BTK_VERSION_MAJOR}")
set(COPPERSPICE_VERSION_MINOR "${BTK_VERSION_MINOR}")
set(COPPERSPICE_VERSION_PATCH "${BTK_VERSION_PATCH}")
set(COPPERSPICE_VERSION       "${BTK_VERSION}")
set(COPPERSPICE_VERSION_API   "${BTK_VERSION_API}")
set(CS_INSTALL_MODE           "${BTK_INSTALL_MODE}")
set(CsLibGuarded_Deploy       "${BtkLibGuarded_Deploy}")
set(CsPointer_Deploy          "${BtkPointer_Deploy}")
set(CsSignal_Deploy           "${BtkSignal_Deploy}")
set(CsString_Deploy           "${BtkString_Deploy}")

set(BTK_CMAKE_MACROS_FILE "${BTK_CMAKE_DIR}/BTKMacros.cmake")
set(BTK_CMAKE_DEPLOY_FILE "${BTK_CMAKE_DIR}/BTKDeploy.cmake")
