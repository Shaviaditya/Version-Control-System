#!/bin/bash

function imperium() {
    DIR = $PWD
    export dir = $DIR
    cd ~/imperium/bin || echo "Error"
    # $@ to check list of all commands 
    ./main "$@"
    cd "$DIR" || echo "Error" 
}