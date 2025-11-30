TRACE_FILE=$1
BINARY=$2
./bin/$BINARY --warmup_instructions 25000000 --simulation_instructions 50000000 $TRACE_FILE