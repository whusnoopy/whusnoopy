#!/bin/bash

cur_dir=`pwd`
s_dir=`dirname $0`
if [ "${s_dir}" == "." ]; then
  util_dir="$cur_dir"
elif [ "${s_dir:0:2}" == "./" ]; then
  util_dir="$cur_dir/${s_dir:2:1024}"
fi

root_dir=`dirname $util_dir`
markdown_dir="${root_dir}/markdown"

source "${util_dir}/yewenus.conf"

function YWLOG()
{
  now_time=`date +%D_%H:%M:%S`
  log_message=$*

  echo -e "$now_time $log_message"
}

function genhtml() {
    YWLOG "Start to generate html from all text files"

    filelist=`ls $markdown_dir/*.text`

    for file in $filelist
    do
      basename=`basename $file`
      htmlpath="$root_dir/${basename%%.*}.html"
      `markdown_py $file | $util_dir/htmlregen.py > $htmlpath`
    done

    YWLOG "all html files generated"
}

function push() {
    local modified_files="$*"
    YWLOG "start to push local modify to yewen.us"
    cmd="cd public_html; put $modified_files; quit"
    lftp -e "$cmd" -u $USERNAME,$PASSWORD $HOST 1> /dev/null
}

if [ $# -lt 1 ]; then
    YWLOG "DEFAULT PROCESS"
    genhtml
elif [ "${1:0:3}" == "gen" ]; then
    YWLOG "Generate html only"
    genhtml
elif [ "${1:0:4}" == "push" ]; then
    shift;
    modified_files="$*"
    YWLOG "push files ${modified_files} to yewen.us"
    push ${modified_files}
else
    YWLOG "Unknown command"
fi

exit

