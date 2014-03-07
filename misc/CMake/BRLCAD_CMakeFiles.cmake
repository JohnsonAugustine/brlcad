#          B R L C A D _ C M A K E F I L E S . C M A K E
# BRL-CAD
#
# Copyright (c) 2011-2014 United States Government as represented by
# the U.S. Army Research Laboratory.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following
# disclaimer in the documentation and/or other materials provided
# with the distribution.
#
# 3. The name of the author may not be used to endorse or promote
# products derived from this software without specific prior written
# permission.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
# OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
# GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#


# Define a macro for building lists of files.  Distcheck needs to know
# what files are "supposed" to be present in order to make sure the
# source tree is clean prior to building a distribution tarball, hence
# this macro stores its results in files and not variables  It's a
# no-op in a SUBBUILD.
#
# For this macro to work correctly, it is important that any target
# definitions or explicit calls to this macro supply relative paths for
# files present in the source tree.  Generated files fed into
# compilation targets are not one of the things that should be in lists
# generated by this macro, and the only way to reliably recognize them
# is to reject files specified using their full path.  Such files must
# use their full path in the build logic in order for out-of-src-dir
# builds to function, so as long as no full paths are used for files
# actually IN the source tree this method is reliable.  The macro will
# try to catch improperly specified files, but if the build directory
# and the source directory are one and the same this will not be
# possible.

define_property(GLOBAL PROPERTY CMAKE_IGNORE_FILES BRIEF_DOCS "distcheck ignore files" FULL_DOCS "List of files known to CMake")
define_property(GLOBAL PROPERTY CMAKE_IGNORE_DIRS BRIEF_DOCS "distcheck ignore dirs" FULL_DOCS "List of directories marked as fully known to CMake")

macro(CMAKEFILES)
  if(NOT BRLCAD_IS_SUBBUILD)
    foreach(ITEM ${ARGN})
      set(CMAKEFILES_DO_TEST 1)
      # The build targets will use certain keywords for arguments -
      # before we proceed to do any ignoring based on those names,
      # make sure the file is there.  Normally attempting to ignore
      # a non-existent file is a fatal error, but these keywords
      # don't necessarily refer to files.
      set(TARGET_FLAGS SHARED STATIC OBJECT WIN32 UNKNOWN IMPORTED MODULE)
      foreach(TARGET_FLAG ${TARGET_FLAGS})
	if("${TARGET_FLAG}" STREQUAL "${ITEM}")
	  if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${ITEM})
            set(CMAKEFILES_DO_TEST 0)
	  endif(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${ITEM})
	endif("${TARGET_FLAG}" STREQUAL "${ITEM}")
      endforeach(TARGET_FLAG ${TARGET_FLAGS})
      set(FUZZY_TARGET_FLAGS TARGET_OBJECTS)
      foreach(TARGET_FLAG ${FUZZY_TARGET_FLAGS})
	if("${ITEM}" MATCHES "${TARGET_FLAG}")
	  if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${ITEM})
            set(CMAKEFILES_DO_TEST 0)
	  endif(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${ITEM})
	endif("${ITEM}" MATCHES "${TARGET_FLAG}")
      endforeach(TARGET_FLAG ${FUZZY_TARGET_FLAGS})

      # Handled target flags, proceeding with tests.
      if(CMAKEFILES_DO_TEST)
	get_filename_component(ITEM_PATH "${ITEM}" PATH)
	get_filename_component(ITEM_NAME "${ITEM}" NAME)
	if(NOT "${ITEM_PATH}" STREQUAL "")
	  # If the build directory is not the same as the source
          # directory, we can enforce the convention that only
          # generated files be specified with their full name.
          # If this becomes a problem with the third party build
          # systems at some point in the future, we may have to
          # exclude src/other paths from this check.
	  if(NOT "${CMAKE_BINARY_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
	    IS_SUBPATH("${CMAKE_BINARY_DIR}" "${ITEM_PATH}" SUBPATH_TEST)
	    if("${SUBPATH_TEST}" STREQUAL "0")
	      IS_SUBPATH("${CMAKE_SOURCE_DIR}" "${ITEM_PATH}" SUBPATH_TEST)
	      if("${SUBPATH_TEST}" STREQUAL "1")
	        message(FATAL_ERROR "${ITEM} is listed in ${CMAKE_CURRENT_SOURCE_DIR} using its absolute path.  Please specify the location of this file using a relative path.")
	      endif("${SUBPATH_TEST}" STREQUAL "1")
	    endif("${SUBPATH_TEST}" STREQUAL "0")
	  endif(NOT "${CMAKE_BINARY_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
	  # Ignore files specified using full paths, since they
          # should be generated files and are not part of the
          # source code repository.
	  get_filename_component(ITEM_ABS_PATH "${ITEM_PATH}" ABSOLUTE)
	  if(NOT "${ITEM_PATH}" STREQUAL "${ITEM_ABS_PATH}")
	    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${ITEM}")
	      message(FATAL_ERROR "Attempting to ignore non-existent file ${ITEM}, in directory ${CMAKE_CURRENT_SOURCE_DIR}")
	    endif(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${ITEM}")
	    # Append files and directories to their respective lists.
	    if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${ITEM}")
	      set_property(GLOBAL APPEND PROPERTY CMAKE_IGNORE_DIRS "${ITEM_ABS_PATH}/${ITEM_NAME}")
	    else(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${ITEM}")
	      set_property(GLOBAL APPEND PROPERTY CMAKE_IGNORE_FILES "${ITEM_ABS_PATH}/${ITEM_NAME}")
	    endif(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${ITEM}")
	    set_property(GLOBAL APPEND PROPERTY CMAKE_IGNORE_FILES "${ITEM_ABS_PATH}")
	    # Add the parent directories of the specified file or directory as well -
	    # the convention is that once at least one file or directory in a path is recorded
	    # by the build logic, the parent directories along that path have also been recorded.
	    while(NOT "${ITEM_PATH}" STREQUAL "")
	      get_filename_component(ITEM_NAME "${ITEM_PATH}" NAME)
	      get_filename_component(ITEM_PATH "${ITEM_PATH}" PATH)
	      if(NOT "${ITEM_PATH}" STREQUAL "")
		get_filename_component(ITEM_ABS_PATH "${ITEM_PATH}" ABSOLUTE)
		if(NOT "${ITEM_NAME}" STREQUAL "..")
	          set_property(GLOBAL APPEND PROPERTY CMAKE_IGNORE_FILES "${ITEM_ABS_PATH}")
		endif(NOT "${ITEM_NAME}" STREQUAL "..")
	      endif(NOT "${ITEM_PATH}" STREQUAL "")
	    endwhile(NOT "${ITEM_PATH}" STREQUAL "")
	  endif(NOT "${ITEM_PATH}" STREQUAL "${ITEM_ABS_PATH}")
	else(NOT "${ITEM_PATH}" STREQUAL "")
	  # The easy case - no path specified, so assume the current source directory.
	  if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${ITEM}")
	    message(FATAL_ERROR "Attempting to ignore non-existent file ${ITEM}, in directory ${CMAKE_CURRENT_SOURCE_DIR}")
	  endif(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${ITEM}")
	  if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${ITEM}")
	    set_property(GLOBAL APPEND PROPERTY CMAKE_IGNORE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/${ITEM_NAME}")
	  else(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${ITEM}")
	    set_property(GLOBAL APPEND PROPERTY CMAKE_IGNORE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/${ITEM_NAME}")
	  endif(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${ITEM}")
	endif(NOT "${ITEM_PATH}" STREQUAL "")
      endif(CMAKEFILES_DO_TEST)
    endforeach(ITEM ${ARGN})
  endif(NOT BRLCAD_IS_SUBBUILD)
endmacro(CMAKEFILES FILESLIST)

# Routine to tell distcheck to ignore a series of items in a directory.  Items may themselves
# be directories.  Primarily useful when working with src/other dist lists.
macro(CMAKEFILES_IN_DIR filestoignore targetdir)
  if(NOT BRLCAD_IS_SUBBUILD)
    set(CMAKE_IGNORE_LIST "")
    foreach(filepath ${${filestoignore}})
      set(CMAKE_IGNORE_LIST ${CMAKE_IGNORE_LIST} "${targetdir}/${filepath}")
    endforeach(filepath ${filestoignore})
    CMAKEFILES(${CMAKE_IGNORE_LIST})
  endif(NOT BRLCAD_IS_SUBBUILD)
endmacro(CMAKEFILES_IN_DIR)

# Local Variables:
# tab-width: 8
# mode: cmake
# indent-tabs-mode: t
# End:
# ex: shiftwidth=2 tabstop=8
