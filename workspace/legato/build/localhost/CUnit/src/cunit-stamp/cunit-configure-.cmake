

set(command "sh;/home/shrunoti/Downloads/workspace/legato/build/localhost/CUnit/configure.sh")
execute_process(
  COMMAND ${command}
  RESULT_VARIABLE result
  OUTPUT_FILE "/home/shrunoti/Downloads/workspace/legato/build/localhost/CUnit/src/cunit-stamp/cunit-configure-out.log"
  ERROR_FILE "/home/shrunoti/Downloads/workspace/legato/build/localhost/CUnit/src/cunit-stamp/cunit-configure-err.log"
  )
if(result)
  set(msg "Command failed: ${result}\n")
  foreach(arg IN LISTS command)
    set(msg "${msg} '${arg}'")
  endforeach()
  set(msg "${msg}\nSee also\n  /home/shrunoti/Downloads/workspace/legato/build/localhost/CUnit/src/cunit-stamp/cunit-configure-*.log")
  message(FATAL_ERROR "${msg}")
else()
  set(msg "cunit configure command succeeded.  See also /home/shrunoti/Downloads/workspace/legato/build/localhost/CUnit/src/cunit-stamp/cunit-configure-*.log")
  message(STATUS "${msg}")
endif()
