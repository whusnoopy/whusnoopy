#!/bin/bash

# This script used for update local ip to ddns on dnspod.cn

# get token from https://www.dnspod.cn/console/user/security follow https://support.dnspod.cn/Kb/showarticle/tsid/227/
TOKEN_ID="TODO"
TOKEN="TODO"
DOMAIN="TODO"
SUB_DOMAIN="TODO"

source "$(dirname $0)/ddns.conf"


#### parse ids

if [ $(grep -V | grep "GNU grep") ]
then
  GREP="grep"
else
  GREP="ggrep"
fi

REQUEST_PARAM="login_token=${TOKEN_ID},${TOKEN}&format=json&domain=${DOMAIN}&sub_domain=${SUB_DOMAIN}"

record_info=$(curl -s -X POST https://dnsapi.cn/Record.List -d "${REQUEST_PARAM}")
record_id=$(echo ${record_info} | ${GREP} -Po "(?<=\[\{\"id\":\")([0-9\.]*)")
record_value=$(echo ${record_info} | ${GREP} -Po "(?<=\"value\":\")([0-9\.]*)")
record_line_id=$(echo ${record_info} | ${GREP} -Po "(?<=\"line_id\":\")([0-9\.]*)")
echo "${SUB_DOMAIN}:${DOMAIN} record_id: ${record_id}, record_line_id: ${record_line_id}, record_value: ${record_value}"

local_ip_ret=$(curl -s "https://pv.sohu.com/cityjson?ie=utf-8")
local_ip=$(echo ${local_ip_ret} | ${GREP} -Po "(?<=\"cip\": \")([0-9\.]*)")
echo "local ip: ${local_ip}"

if [ $record_value != $local_ip ]
then
  echo "ip changed, will update"
  update_res=$(curl -s -X POST https://dnsapi.cn/Record.Ddns -d "${REQUEST_PARAM}&record_id=${record_id}&record_line_id=${record_line_id}&value=${local_ip}")
  echo $update_res
else
  echo "not changed, ignore"
fi

