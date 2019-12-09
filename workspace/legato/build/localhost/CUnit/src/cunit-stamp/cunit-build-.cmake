

set(command "make")
execute_process(
  COMMAND ${command}
  RESULT_VARIABLE result
  OUTPUT_FILE "/home/shrunoti/Downloads/workspace/legato/build/localhost/CUnit/src/cunit-stamp/cunit-build-out.log"
  ERROR_FILE "/home/shrunoti/Downloads/workspace/legato/build/localhost/CUnit/src/cunit-stamp/cunit-build-err.log"
  )
if(result)
  set(msg "Command failed: ${result}\n")
  foreach(arg IN LISTS command)
    set(msg "${msg} '${arg}'")
  endforeach()
  set(msg "${msg}\nSee also\n  /home/shrunoti/Downloads/workspace/legato/build/localhost/CUnit/src/cunit-stamp/cunit-build-*.log")
  message(FATAL_ERROR "${msg}")
else()
  set(msg "cunit build command succeeded.  See also /home/shrunoti/Downloads/workspace/legato/build/localhost/CUnit/src/cunit-stamp/cunit-build-*.log")
  message(STATUS "${msg}")
endif()
