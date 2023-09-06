#!/bin/bash

# This script runs the specified command in a loop and prints the duration of
# each iteration. If the command takes longer than the specified timeout value,
# it is killed and the iteration is counted as a kill.

command_to_run=$1
timeout_value=${2:-120}
counter=0
kill_counter=0
current_pid=0

# Function to handle script interruption
handle_interrupt() {
  echo 'Script interrupted by user'
  
  if [ $current_pid -ne 0 ]; then
    kill $current_pid
    echo "Killed running command with PID: $current_pid"
  fi
  
  exit 0
}

trap handle_interrupt SIGINT

if [ -z "$1" ]; then
  echo "Usage: $0 <command> [timeout_in_seconds]"
  exit 1
fi

if [ $? -ne 0 ]; then
  echo "Failed to change directory to /workspaces/afw. Exiting."
  exit 1
fi

while true; do
  ((counter++))
  start_time=$(date +%s%N)
  
  # Use the timeout command to limit the command's execution time to the specified timeout value
  timeout $timeout_value $command_to_run &
  current_pid=$!
  wait $current_pid
  exit_status=$?
  
  end_time=$(date +%s%N)
  duration=$(echo "($end_time - $start_time) / 1000000000" | bc -l)
  duration=$(printf "%.1f" $duration)
  
  if [ $exit_status -eq 124 ]; then
    ((kill_counter++))
    echo "Command killed due to timeout on iteration: $counter"
  fi
  
  current_pid=0
  echo "Running '$command_to_run' iteration: $counter, Duration: $duration seconds, Kills: $kill_counter"
  sleep 1
done
