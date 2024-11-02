#!/bin/bash

# Run the server and capture its output (the PID)
server_pid=$(./server)

# Check if the server returned a valid PID
if [[ -n "$server_pid" ]]; then
    echo "Server PID: $server_pid"

    # Run the client with the server PID and message "abc"
    ./client "$server_pid" "abc"
else
    echo "Failed to get server PID."
    exit 1
fi
