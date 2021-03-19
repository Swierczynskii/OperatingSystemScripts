#!/bin/bash


## Arguments reference
# $# --> number of arguments 
# $0 --> filename of script
# $1 --> 1st argument // could be [-l] or [-u] or [-h] or [-r]
# $2 --> 2nd argument // could be [-l] or [-u] or sed or <dir/file names...>
R=0
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

change(){
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
    local new="${pathname}/${new_filename}"
    if [ "$1" != "$new" ];then     
        mv "$1" "$new"                      # overwriting
    fi
}

rec(){
    
    for file in "$1"/*                      # Iterate over files in $1 directory
    do
        if [ -d "$file" ]; then             # If $file is a directory -> go into it and iterate over its files
            rec "$file" "$action" "$sed_p"
        elif [ -f "$file" ]; then           # If $file is a file -> modify its name
            change "$file" "$action" "$sed_p"
        else
            break
        fi
    done
}

### MAIN SCRIPT ###
if [ -z "$1" ]; then
    echo "Wrong input.
Type './modify.sh -h' for help."
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

while [ -n "$1" ]; do                   # while $1 is not a null string
        
    if [ $R -eq 1 -a -d "$1" ]; then    # if R mode is on and $1 is directory
        rec "$1" "$action" "$sed_p"
    elif [ -f "$1" ]; then              # if $1 is a regular file (not directory nor device)
        change "$1" "$action" "$sed_p"
    else
        echo "Wrong input.
Type './modify.sh -h' for help!"
    fi
    shift
done

