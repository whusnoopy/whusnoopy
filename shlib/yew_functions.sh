#!/bin/bash

function YWLOG()
{
    now_time=`date +%D_%H:%M:%S`
    log_message="$*"

    echo -e "$now_time" "$log_message"
}

function YWFLOG()
{
    YWLOG "\033[0;1;31mFATA\033[m" "$*"
}

function YWWLOG()
{
    YWLOG "\033[0;1;33mWARN\033[m" "$*"
}

function YWILOG()
{
    YWLOG "\033[0;1;32mINFO\033[m" "$*"
}

function YWDLOG()
{
    YWLOG "DEBUG" "$*"
}

function wget2local() {
    local remote_path=${1}; shift
    local local_path=${1}; shift

    YWILOG "prepare to download [$remote_path] to [$local_path]"
    if [[ -f $local_path && "$MODE" != "RETRY" ]]; then
        YWWLOG "[$local_path] exists, and NOT in RETRY mode, skip this step"
    else
        if [ -f ${local_path} ]; then
            YWWLOG "[$local_path] exists, in RETRY mode, remove it"
            rm ${local_path}
        fi

        wget $remote_path -O ${local_path}

        if [ "$?" != 0 ]; then
            YWFLOG "download [$remote_path] to [$local_path] failed, will remove [$local_path]"
            rm ${local_path}
        fi
    fi
}

function scp2local() {
    local remote_path=${1}; shift
    local loacl_path=${1}; shift

    YWILOG "prepare to download [$remote_path] to [$local_path]"
    if [[ -f $local_path && "$MODE" != "RETRY" ]]; then
        YWWLOG "[$local_path] exists, and NOT in RETRY mode, skip this step"
    else
        if [ -f ${local_path} ]; then
            YWWLOG "[$local_path] exists, in RETRY mode, remove it"
            rm ${local_path}
        fi

        scp $remote_path ${local_path}

        if [ "$?" != 0 ]; then
            YWFLOG "download [$remote_path] to [$local_path] failed, will remove [$local_path]"
            rm ${local_path}
        fi
    fi
}

function getpwd() {
    cur_dir=`pwd`
    s_dir=`dirname ${1}`
    if [ "${s_dir}" == "." ]; then
        script_dir="$cur_dir"
    elif [ "${s_dir:0:2}" == "./" ]; then
        script_dir="$cur_dir/${s_dir:2:1024}"
    fi
    local_dir=`dirname $script_dir`
    bin_dir="$local_dir/bin"
    res_dir="$local_dir/res"
}
