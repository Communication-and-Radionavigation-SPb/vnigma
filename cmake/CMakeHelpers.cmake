# Adds the given folder_name into the source files of the current project.
# Use this macro when your module contains .cpp and .h files in several subdirectories.
# Your sources variable needs to be WSPP_SOURCE_FILES and headers variable WSPP_HEADER_FILES.
macro(add_source_folder folder_name)
    file(GLOB H_FILES_IN_FOLDER_${folder_name} ${folder_name}/*.hpp ${folder_name}/*.h)
    file(GLOB CPP_FILES_IN_FOLDER_${folder_name} ${folder_name}/*.cpp ${folder_name}/*.c)
    source_group("Header Files\\${folder_name}" FILES ${H_FILES_IN_FOLDER_${folder_name}})
    source_group("Source Files\\${folder_name}" FILES ${CPP_FILES_IN_FOLDER_${folder_name}})
    set(VN_HEADER_FILES ${VN_HEADER_FILES} ${H_FILES_IN_FOLDER_${folder_name}})
    set(VN_SOURCE_FILES ${VN_SOURCE_FILES} ${CPP_FILES_IN_FOLDER_${folder_name}})
endmacro()