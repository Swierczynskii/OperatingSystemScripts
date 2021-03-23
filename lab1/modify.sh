#!/bin/bash

R=0                 # Recursive mode set to zero

prompt_help(){
    echo "HELP:
Sample usage of the function:

    ./modify.sh -r -l|-u directory           
    ./modify.sh -r <sed pattern> directory   
    ./modify.sh -h              
    ./modify.sh -u directory/filename directory/filename 
    ./modify.sh -u directory/filename 
    ./modify.sh -l filename filename
    ./modify.sh <sed pattern> directory/filename
    ./modify.sh <sed pattern> filename

Where:
    -l script lowerizes given filename
    -u script uppercases given filename
    <sed pattern> script uses sed command to change some pattern in filename
    (eg. s/test/success/g -> changing 'test' for 'success')
    -r script uses recursive directory listing to find and change filenames
    (takes only directories as argument)"
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
            exit 1
            ;;
    esac

    local new="${pathname}/${new_filename}"

    if [ "$1" != "$new" ];then     
        mv "$1" "$new"                      # overwriting
    else
        echo "Failed to rename, check input correctness!"
        exit 1
    fi
}

rec(){

    for file in "$1"/* ; do                 # Iterate over files in $1 directory
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
    echo "Wrong input!
Type './modify.sh -h' for help."
    exit 1
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
    -*)
        echo "Wrong input!
Type './modify.sh -h' for help."
        exit 1
        ;;
    *)
        action=sed
        sed_p=$1
        shift
        ;;
esac

if [ -z "$1" ]; then
    echo "Wrong input!
Type './modify.sh -h' for help."
    exit 1
fi

while [ -n "$1" ]; do                   # while $1 is not a null string
    if [ $R -eq 1 -a -f "$1" ]; then    # if R mode is on and $1 is a file
        echo "Wrong input!
Type './modify.sh -h' for help!"
        exit 1
    elif [ $R -eq 1 -a -d "$1" ]; then  # if R mode is on and $1 is a directory
        rec "$1" "$action" "$sed_p"
    elif [ -f "$1" ]; then              # if $1 is a regular file (not directory nor device)
        change "$1" "$action" "$sed_p"
    else
        echo "Wrong input!
Type './modify.sh -h' for help!"
        exit 1
    fi
    shift
done
