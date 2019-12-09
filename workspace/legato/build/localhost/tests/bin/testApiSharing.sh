# This test script should be executed from the localhost/bin directory

# Enable debug messages
export LE_LOG_LEVEL=DEBUG

# Start legato system processes; returns warning if the processes are already running.
startlegato

# Add bindings.
config set users/$USER/bindings/clientSideA/user $USER
config set users/$USER/bindings/clientSideA/interface serverSide1
config set users/$USER/bindings/clientSideB/user $USER
config set users/$USER/bindings/clientSideB/interface serverSide2
sdir load

# Start the server in the background.
echo "==== Starting Server..."
tests/testApiSharing_server &
SERVER_PID=$!

# Wait for server to start and connect to Service Directory, then
# dump diagnostic info from the Service Directory.
sleep 0.1
echo "==== With server running:"
sdir list

# Start the client and wait for it to exit.  Capture exit code.
tests/testApiSharing_client
CLIENT_RESULT=$?

# Kill the server.
kill -9 $SERVER_PID
KILL_RESULT=$?

# If unable to kill the server, it must have died for some reason.
if [ $KILL_RESULT -ne 0 ]
then
    echo "FAILED: Server died unexpectedly. (Client exit code: $CLIENT_RESULT)."
    exit $KILL_RESULT
fi

# Check the client's exit code.
if [ $CLIENT_RESULT -ne 0 ]
then
    echo "FAILED: Client reported an error."
fi

exit $CLIENT_RESULT
