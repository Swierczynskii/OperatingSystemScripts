#!/bin/bash


## Arguments reference
# $# --> number of arguments 
# $0 --> filename of script
# $1 --> 1st argument // could be [-l] or [-u] or [-h] or [-r]
# $2 --> 2nd argument // could be [-l] or [-u] or sed or <dir/file names...>

prompt_help(){
    echo "HELP:
Sample usage of the function:
    ./modify.sh -r -l|-u dir/filename           //not working
    ./modify.sh -r <sed pattern> dir/filename   //not working
    ./modify.sh -h              //working
    ./modify.sh -u dir/filename dir/filename //working
    ./modify.sh -u dir/filename //working
    ./modify.sh -l filename filename//working
        
The script is dedicated to lowerizing [-l] 
file names, uppercasing [-u] file names or internally calling sed
command with the given sed pattern which will operate on file names.
Changes may be done either with recursion [-r] or without it. "
}

change_size(){
    if [ -z "$1" ]; then                            # could use 'test' instead of '[]'
        echo "No directories/files were given"  
    elif [ -e "$1" ]; then
        local filename=$(basename $1)
        case "$up_or_low" in

            l)
                local new_filename="$(echo "$filename" | tr A-Z a-z)"
                ;;
            u)
                local new_filename="$(echo "$filename" | tr a-z A-Z)"
                ;;
            *)
                echo "Error"
                ;;
        esac
        local new="${pathname}/${new_filename}"
        mv "$1" "$new"
    else
        echo "No "$1" file exists"
    fi
}




### MAIN SCRIPT ###
if [ -z "$1" ]; then
    echo "Wrong input.
Type ./modify.sh -h for help."
    exit 0
fi
case "$1" in
    -R | -r)
        echo recursive
        shift
        ;;
    -h | -H)
        prompt_help
        exit 0
        ;;

esac 
case "$1" in

    -u | -U)
        up_or_low=u
        shift
        pathname=$(dirname $1)
        while [ -n "$1" ]; do
        change_size "$1" "$up_or_low" "$pathname"
        shift
        done 
        ;;
    -l | -L)
        up_or_low=l
        shift
        pathname=$(dirname $1)
        while [ -n "$1" ]; do
        change_size "$1" "$up_or_low" "$pathname"
        shift
        done 
        ;;
    *)
        echo "Wrong input.
Type ./modify.sh -h for help."
        exit 0
        ;;
esac


