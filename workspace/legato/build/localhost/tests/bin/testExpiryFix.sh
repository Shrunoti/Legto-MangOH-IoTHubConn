# This test script should be executed from the localhost/bin directory

# Enable debug messages
export LE_LOG_LEVEL=DEBUG

# Start legato system processes; returns warning if the processes are already running.
startlegato

tests/testExpiryFix

