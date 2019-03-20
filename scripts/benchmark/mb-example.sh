# Example program using asmoses-bm.sh to benchmark a series of runs
# with some given options

#############
# Constants #
#############

# Run of times to repeat each run (as to gather statistics)
REPEAT=10

# Get the script directory
PRG_PATH="$(readlink -f "$0")"
PRG_DIR="$(dirname "$PRG_PATH")"

########
# Main #
########

echo "Run asmoses $REPEAT times on multiple problems of various complexity"

for pbl in pa dj mux maj sr; do
    for k in {3..5}; do
        $PRG_DIR/asmoses-bm.sh $REPEAT -H$pbl -k$k -m10000
    done
done
