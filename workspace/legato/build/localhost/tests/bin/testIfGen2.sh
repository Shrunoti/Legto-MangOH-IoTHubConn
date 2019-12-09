# This test script should be executed from the localhost/tests/bin directory

# Enable debug messages
export LE_LOG_LEVEL=DEBUG

# Start legato system processes; returns warning if the processes are already running.
startlegato

# Add bindings for 'example' service
config set users/$USER/bindings/example/user $USER
config set users/$USER/bindings/example/interface example
sdir load

./testIfGen2_server &
sleep 0.5

./testIfGen2_client

