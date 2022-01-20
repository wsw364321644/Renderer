cmake_minimum_required (VERSION 3.16)

FUNCTION (EXCLUDE_FILES_FROM_DIR_IN_LIST _InFileList _excludeDirName )
  foreach (ITR ${_InFileList})
    if ("${ITR}" MATCHES "(.*)${_excludeDirName}(.*)")                   # Check if the item matches the directory name in _excludeDirName

      list (REMOVE_ITEM _InFileList ${ITR})                              # Remove the item from the list
    endif ("${ITR}" MATCHES "(.*)${_excludeDirName}(.*)")

  endforeach(ITR)
  set(EXCLUDED_FILES ${_InFileList} PARENT_SCOPE)                          # Return the SOURCE_FILES variable to the calling parent
ENDFUNCTION (EXCLUDE_FILES_FROM_DIR_IN_LIST)

macro(SearchSourceFiles FolderPath IsRecurse)	
	set(temppath ${FolderPath})
	cmake_path(APPEND  temppath  "*.h"  OUTPUT_VARIABLE  TmpHHeader)
	cmake_path(APPEND  temppath  "*.hpp" OUTPUT_VARIABLE  TmpHppHeader)
	cmake_path(APPEND  temppath  "*.c" OUTPUT_VARIABLE  TmpC)
	cmake_path(APPEND  temppath  "*.cpp" OUTPUT_VARIABLE  TmpCpp)
	cmake_path(APPEND  temppath  "*.ico" OUTPUT_VARIABLE  TmpIcon)
	cmake_path(APPEND  temppath  "*.rc" OUTPUT_VARIABLE  TmpRC)

	if(${IsRecurse})
		file(GLOB_RECURSE TmpSource LIST_DIRECTORIES false CONFIGURE_DEPENDS  ${TmpHHeader} ${TmpHppHeader} ${TmpC}  ${TmpCpp}  ${TmpIcon} ${TmpRC})
	else()
		file(GLOB TmpSource LIST_DIRECTORIES false CONFIGURE_DEPENDS  ${TmpHHeader} ${TmpHppHeader} ${TmpC}  ${TmpCpp}  ${TmpIcon} ${TmpRC})
	endif()

	if (WIN32)
		EXCLUDE_FILES_FROM_DIR_IN_LIST("${TmpSource}" "Linux")
		#message(STATUS "EXCLUDED_FILES ${EXCLUDED_FILES}")
	elseif (UNIX)
		EXCLUDE_FILES_FROM_DIR_IN_LIST("${TmpSource}" "Windows")
	endif()

	message(STATUS "EXCLUDED_FILES ${EXCLUDED_FILES}")
	if (NOT EXCLUDED_FILES STREQUAL "")
		set(SourceFiles "${SourceFiles};${EXCLUDED_FILES}")
	endif()
endmacro(SearchSourceFiles)

macro(AddSourceFolder)
	set(options INCLUDE RECURSE)
    set(oneValueArgs  )
    set(multiValueArgs  PUBLIC PRIVATE INTERFACE)
    cmake_parse_arguments(AddSourceFolder "${options}" "${oneValueArgs}"
                          "${multiValueArgs}" ${ARGN} )

	if(AddSourceFolder_INCLUDE)
		foreach(LETTER ${AddSourceFolder_PRIVATE})
			list(APPEND PrivateIncludeFolders ${LETTER}) 
			SearchSourceFiles(${LETTER} ${AddSourceFolder_RECURSE})
		endforeach()
	
		foreach(LETTER ${AddSourceFolder_PUBLIC})
			list(APPEND PublicIncludeFolders ${LETTER}) 
			SearchSourceFiles(${LETTER} ${AddSourceFolder_RECURSE})
		endforeach()
	
		foreach(LETTER ${AddSourceFolder_INTERFACE})
			list(APPEND InterfaceIncludeFolders ${LETTER})
			SearchSourceFiles(${LETTER} ${AddSourceFolder_RECURSE})
		endforeach()
	else()
		foreach(LETTER ${ARGN})
			SearchSourceFiles(${LETTER} ${AddSourceFolder_RECURSE})
		endforeach()
	endif()

endmacro(AddSourceFolder)