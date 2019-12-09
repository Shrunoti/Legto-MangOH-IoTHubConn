# This test script should be executed from the localhost/bin directory
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:lib

# Enable debug messages
export LE_LOG_LEVEL=DEBUG

mkdir -p sockets
sleep 0.5

./serviceDirectory &
sleep 0.5

./logCtrlDaemon &
sleep 0.5

tests/testAsync2_server &
sleep 0.5

tests/testAsync2_client

