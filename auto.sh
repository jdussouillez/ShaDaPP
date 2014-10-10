#!/bin/bash
RM="rm -vrf"
SRC_DIR="./src"
BUILD_DIR="./build"
MAKEFILE="$BUILD_DIR/Makefile"
EXEC="$BUILD_DIR/bin/shadapp/shadapp"
ECHO="echo -e"
SEP="="
MAX_LINE=80

# Uncomment for debug
#set -x

process_string() {
    local STR=$1
    local NB=$2
    local ODD=$3
    local i=0
    while [[ $i -lt $NB ]]; do
	STR="$SEP$STR$SEP"
	i=$((i+1))
    done
    # Add a SEP character at the end if the number of SEP is odd
    if [ "$ODD" -eq 1 ]; then
	STR="$STR$SEP"
    fi
    $ECHO "$STR"
}

format_string() {
    local PARAM=" $1 "
    local SIZE_PARAM=${#PARAM}
    local SIZE_DIFF=$((MAX_LINE-SIZE_PARAM))
    local SIZE_SEP=$((SIZE_DIFF/2))
    local SIZE_IS_ODD=$((SIZE_DIFF%2))
    process_string "$PARAM" "$SIZE_SEP" "$SIZE_IS_ODD"
}

print_done() {
    format_string "Done!"
    $ECHO "\n"
}

clean_tmp() {
    format_string "Cleaning temporary files..."
    find -name "*~" -o -name ".*~" -type f | xargs rm -vf
    print_done
}

clean() {
    format_string "Cleaning execs and libraries..."
    cd $BUILD_DIR
    make clean
    cd ..
    print_done
}

clean_all() {
    format_string "Cleaning all project..."
    $RM $BUILD_DIR/*
    print_done
}

build() {
    format_string "Building project..."
    [ -d $BUILD_DIR ] || mkdir $BUILD_DIR
    cd $BUILD_DIR
    if [ ! -f $MAKEFILE ]; then
	cmake ../$SRC_DIR
    fi
    make
    cd ..
    print_done
}

rebuild() {
    clean_all
    build
}

run() {
    build
    format_string "Running '$EXEC'..."
    $EXEC
    format_string "Done!"
}

help() {
    local ECHO="echo -e"
    $ECHO "Usage: $0 [OPTION]"
    $ECHO "Build, run, or clean the project.\n"
    $ECHO "Options :"
    $ECHO "build, b\t\tBuild the project (run CMake if needed)"
    $ECHO "clean, c\t\tClean the project (tmp files + executables + libraries)"
    $ECHO "cleana, ca\t\tClean all the generated files (empty the folder $BUILD_DIR)"
    $ECHO "cleant, ct\t\tClean the temporary files (with extension '~')"
    $ECHO "help, h\t\t\tDisplay this help"
    $ECHO "rbuild, rb\t\tRebuild all the project (cleana + build)"
    $ECHO "run, r\t\t\tRun the binary file '$EXEC' (build the project if needed)"
}

# Open the script's directory
cd "$(dirname "$0")"

if [ $# -eq 0 ]; then
    help
    exit 1
fi

case "$1" in
    "build"|"b")
	build
	;;
    "clean"|"c")
	clean
	;;
    "cleana"|"ca")
	clean_all
	;;
    "cleant"|"ct")
	clean_tmp
	;;
    "help"|"h")
	help
	;;
    "rbuild"|"rb")
	rebuild
	;;
    "run"|"r")
	run
	;;
    *)
	help
	exit 1
	;;
esac
exit 0
