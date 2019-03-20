# Calls asmoses a number of times and reports the mean and std time of
# its runs

# This script relies on https://github.com/nferraz/st

# Check unbound variables
set -u

# # Debug trace
# set -x

#############
# Functions #
#############

# Get the real time in second (given the output of time command)
get_real_time() {
    grep "real" | cut -d' ' -f2
}

########
# Main #
########

# Parse comment argument
if [ $# == 0 ]; then
    echo "Wrong number of arguments."
    echo
    echo "Usage: $0 REPEAT [ASMOSES-OPTIONS]"
    echo
    echo "Simple benchmarking tool for asmoses."
    echo "Calls asmoses REPEAT times with the provided options"
    echo "and return statistics about its execution time."
    exit 1
else
    REPEAT=$1
fi

# Discard REPEAT argument
shift

# Run asmoses REPEAT times
for i in $(seq 1 $REPEAT); do
    if [[ "$i" != "$REPEAT" ]]; then
        echo -en "\rRun asmoses $@ ($i/$REPEAT)" 1>&2
    else
        echo -e "\rRun asmoses $@ ($i/$REPEAT)" 1>&2
    fi
    (time -p asmoses $@ 1>/dev/null) 2>&1
done | get_real_time | st | column -t
