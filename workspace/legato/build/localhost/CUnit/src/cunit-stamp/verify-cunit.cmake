set(file "/home/shrunoti/Downloads/workspace/legato/3rdParty/CUnit/src/CUnit-2.1-2-src.tar.bz2")
message(STATUS "verifying file...
     file='${file}'")
set(expect_value "31c62bd7a65007737ba28b7aafc44d3a")
set(attempt 0)
set(succeeded 0)
while(${attempt} LESS 0 OR ${attempt} EQUAL 0 AND NOT ${succeeded})
  file(MD5 "${file}" actual_value)
  if("${actual_value}" STREQUAL "${expect_value}")
    set(succeeded 1)
  elseif(${attempt} LESS 0)
    message(STATUS "MD5 hash of ${file}
does not match expected value
  expected: ${expect_value}
    actual: ${actual_value}
Retrying download.
")
    file(REMOVE "${file}")
    execute_process(COMMAND ${CMAKE_COMMAND} -P "")
  endif()
  math(EXPR attempt "${attempt} + 1")
endwhile()

if(${succeeded})
  message(STATUS "verifying file... done")
else()
  message(FATAL_ERROR "error: MD5 hash of
  ${file}
does not match expected value
  expected: ${expect_value}
    actual: ${actual_value}
")
endif()
