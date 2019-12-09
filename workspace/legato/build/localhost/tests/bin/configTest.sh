#!/bin/bash


# Make sure that the shared libraries are available.
_script="$(readlink -f ${BASH_SOURCE[0]})"
_base="$(dirname $_script)"

export LD_LIBRARY_PATH=$_base/../lib

echo "---------------------------------------------------------------------------------------------"
echo "Lib path: $LD_LIBRARY_PATH"
echo "---------------------------------------------------------------------------------------------"




# The command line option to look for.  If this text is found that means that the caller wants us to
# make sure that the required legato system services are running.
SERVEROPT="withserver"
SERVER_PARAM=$1




if [ "$SERVER_PARAM" = "$SERVEROPT" ]; then
    # Make sure that the service directory isn't already running.
    killall serviceDirectory || true

    # Start up the system services.  But give the service directory a little extra time to startup
    # as the others depend on it.
    echo "Starting the system services."

    /home/shrunoti/Downloads/workspace/legato/build/localhost/framework/bin/serviceDirectory &
    sleep 1
    /home/shrunoti/Downloads/workspace/legato/build/localhost/framework/bin/logCtrlDaemon &
    /home/shrunoti/Downloads/workspace/legato/build/localhost/framework/bin/configTree &
fi



# When the unit tests are done, this is called to make sure that we don't leave any extra processes
# running.
function CleanUp
{
    echo "Shutting down configTree unit tests."

    if [ "$SERVER_PARAM" = "$SERVEROPT" ]; then
        killall serviceDirectory || true
    fi
}




function ExecWithTimeout
{
    # Extract the parameters passed to this funciton.
    TIMEOUT=$1
    EXIT_EXPECTED=$2
    COMMAND=$3
    COMMAND_ARGS=${*:4}

    # Start a timer process, if it isn't stopped before the sleep returns the process will kill our
    # script.  Grab our PID so that the child can kill this script if it times out.
    PARENT=$$

    {
        sleep $TIMEOUT
        echo "EXEC FAILED"
        kill $PARENT
    } &

    TIMER_PROCID=$!
    echo "Timeout started with PID $TIMER_PROCID"

    # Now, execute the command the user wanted to run.  If the command returns before the timer
    # expires kill the timer sub-process.
    $COMMAND $COMMAND_ARGS
    RET_VAL=$?

    # Get the PID of the "sleep" process that the timer process is waiting for.
    # We have to kill that or ctest will wait for it to finish, even after we kill the timer (shell)
    # process that started it.
    SLEEP_PROCID=$(pgrep -P $TIMER_PROCID)

    echo "Subprocess complete, killing timeout processes $TIMER_PROCID and $SLEEP_PROCID."
    kill $TIMER_PROCID || true
    if [ "$SLEEP_PROCID" ]
    then
        kill $SLEEP_PROCID || true
    fi

    # Report the exit status of the subprocess, and if required, kill the script.
    [ $RET_VAL -eq $EXIT_EXPECTED ] && echo "Subprocess had: SUCCEEDED."

    if [ $RET_VAL -ne $EXIT_EXPECTED ]; then
        echo "Subprocess had: FAILED, with exit code: $RET_VAL"
        CleanUp
        exit $RET_VAL
    fi
}




# Make sure everything is started up and ready to go, then clear out our test count.
sleep 1
/home/shrunoti/Downloads/workspace/legato/build/localhost/framework/bin/config set /configTest/testCount 0


# Set the configTree transaction timeout to something more manageable.
/home/shrunoti/Downloads/workspace/legato/build/localhost/framework/bin/config set /configTree/transactionTimeout 5 int


# These two executables create and fail to close a read and write transaction respectivly.  If the
# configTree doesn't handle this situation correctly then things will start hanging.
ExecWithTimeout 10 0 /home/shrunoti/Downloads/workspace/legato/build/localhost/tests/bin/configDropReadExe
ExecWithTimeout 10 0 /home/shrunoti/Downloads/workspace/legato/build/localhost/tests/bin/configDropWriteExe


# Now, tell them to hang around to make sure that the automatic timeouts work.
ExecWithTimeout 10 1 /home/shrunoti/Downloads/workspace/legato/build/localhost/tests/bin/configDropReadExe hangAround
ExecWithTimeout 10 1 /home/shrunoti/Downloads/workspace/legato/build/localhost/tests/bin/configDropWriteExe hangAround



# Now, fire up a bunch of instances of the config test executable, all in parallel.  If any of them
# return an error, just simply pass that back to our caller.
for i in `seq 1 50`;
do
    MY_STR+="$i "
done

echo $MY_STR | ExecWithTimeout 60 0 xargs -n 1 -P 0 /home/shrunoti/Downloads/workspace/legato/build/localhost/tests/bin/configTestExe

# Report the number of tests that were run.
echo "Number of tests run:"
/home/shrunoti/Downloads/workspace/legato/build/localhost/framework/bin/config get /configTest/testCount


# Now, as a final test and to clean up after ourselves.  Delete the trees from the system.
ExecWithTimeout 10 0 /home/shrunoti/Downloads/workspace/legato/build/localhost/tests/bin/configDelete


# All, done, so try to shutdown cleanly.
CleanUp
