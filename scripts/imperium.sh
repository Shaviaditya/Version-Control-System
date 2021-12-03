#!/bin/bash

function kronos() {
    DIR=$(pwd)
    export dir=$DIR
    cd ~/kronos/bin || echo "Error"
    # $@ to check list of all commands 
    ./main "$@"
    cd "$DIR" || echo "Error" 
}