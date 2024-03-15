# Vnigma

Custom parsing library for **Vnigma** protocol

# Usage with CMake

```cmake
# Declare that library should be fetched and specify tag to use
FetchContent_Declare(
  vnigma
  GIT_REPOSITORY
    https://github.com/Communication-and-Radionavigation-SPb/vnigma.git
  GIT_TAG 5f8698a)
FetchContent_GetProperties(vnigma)

set(VNIGMA_SHARED_LIBS ON) # Optional. ON for build as shared library. OFF for static

if(NOT vnigma)
  FetchContent_Populate(vnigma)
  add_subdirectory(${vnigma_SOURCE_DIR} ${vnigma_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
```
