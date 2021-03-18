#!/bin/bash


## Arguments reference
# $# --> number of arguments 
# $0 --> filename of script
# $1 --> 1st argument // could be [-l] or [-u] or [-h] or [-r]
# $2 --> 2nd argument // could be [-l] or [-u] or sed or <dir/file names...>

prompt_help(){
    echo "HELP:
Sample usage of the function:
    ./modify.sh -r -l|-u directory           //not working
    ./modify.sh -r <sed pattern> directory   //not working
    ./modify.sh -h              //working
    ./modify.sh -u dir/filename dir/filename //working
    ./modify.sh -u dir/filename //working
    ./modify.sh -l filename filename//working
    ./modify.sh <sed pattern> filename
The script is dedicated to lowerizing [-l] 
file names, uppercasing [-u] file names or internally calling sed
command with the given sed pattern which will operate on file names.
Changes may be done either with recursion [-r] or without it. "
}

change_size(){
    if [ -z "$1" ]; then                            # could use 'test' instead of '[]'
        echo "No directories/files were given"      # if there are no arguments
    elif [ -e "$1" ]; then
        local filename=$(basename $1)               # taking file name
        local pathname=$(dirname $1)                # taking directory name
        case "$action" in

            l)
                local new_filename="$(echo "$filename" | tr A-Z a-z)"   #lowercasing
                ;;  
            u)
                local new_filename="$(echo "$filename" | tr a-z A-Z)"   #uppercasing
                ;;
            sed)
                local new_filename="$(echo "$filename" | sed $sed_p)"   #sed pattern
                ;;
            *)
                echo "Error"                                            #should not get here
                ;;
        esac
        local new="${pathname}/${new_filename}"     # overwriting: to do: exceptions/ifs
        mv "$1" "$new"
    else
        echo "No "$1" file exists"                  # we cannot modify something that doesn't exit
    fi
}

### MAIN SCRIPT ###
if [ -z "$1" ]; then
    echo "Wrong input.
Type ./modify.sh -h for help."
    exit 0
fi
case "$1" in
    -r | -R)
        R=1
        shift
        ;;
    -h | -H)
        prompt_help
        exit 0
        ;;

esac 
case "$1" in

    -u | -U)
        action=u
        shift
        ;;
    -l | -L)
        action=l
        shift
        ;;
    *)
        action=sed
        sed_p=$1
        shift
        ;;
esac

while [ -n "$1" ]; do
        change_size "$1" "$action" "$sed_p"
        shift
done

