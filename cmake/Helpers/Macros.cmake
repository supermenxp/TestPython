# 将文件按文件夹分组
MACRO(source_group_by_dir source_files)
    if(MSVC)
        set(sgbd_cur_dir ${CMAKE_CURRENT_SOURCE_DIR})

        foreach(sgbd_file ${${source_files}})
            string(REGEX REPLACE ${sgbd_cur_dir}/\(.*\) \\1 sgbd_fpath ${sgbd_file})
            string(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_name ${sgbd_fpath})
            string(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)
            string(REPLACE "/" "\\" sgbd_group_name ${sgbd_group_name})

            if(sgbd_nogroup)
                set(sgbd_group_name "\\")
            endif(sgbd_nogroup)

            source_group(${sgbd_group_name} FILES ${sgbd_file})
        endforeach(sgbd_file)
    endif(MSVC)
ENDMACRO(source_group_by_dir)

MACRO(SetupPython)
    set(PYTHON_INCLUDE_DIRS $ENV{Python_DIR}/Include)
    set(PYTHON_LIBRARY_DIRS $ENV{Python_DIR}/PCbuild/amd64)
    set(PYTHONINTERP_FOUND True)

    if(CMAKE_BUILD_TYPE AND(CMAKE_BUILD_TYPE STREQUAL "Release"))
        set(PYTHON_EXECUTABLE $ENV{Python_DIR}/PCbuild/amd64/python.exe)
        set(PYTHON_LIBRARIES $ENV{Python_DIR}/PCbuild/amd64/python39.lib)
    else()
        set(PYTHON_EXECUTABLE $ENV{Python_DIR}/PCbuild/amd64/python_d.exe)
        set(PYTHON_LIBRARIES $ENV{Python_DIR}/PCbuild/amd64/python39_d.lib)
    endif()
endmacro(SetupPython)