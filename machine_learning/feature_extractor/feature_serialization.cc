// Copyright 2011 yewen

#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <tr1/unordered_map>

#include "base/flags.h"
#include "base/logging.h"
#include "base/util.h"

using namespace std;

DEFINE_OPTIONAL_FLAGS(string, instance_file, \
        "./data/instance_for_serialize.txt", \
        "the instance file for serialize");
DEFINE_OPTIONAL_FLAGS(string, serialized_instance_file, \
        "./output/instance_for_train.txt", \
        "where to put the serialized instance file");

const int MAX_LINE_LEN = 1024000;

int g_sum_fea_num;
int g_sum_ins_num;

std::tr1::unordered_map< const char *, int, std::tr1::hash<const char *> > g_fea_map;

int serializeInstance(FILE* ins_fp, FILE* out_fp) {
    char buf[MAX_LINE_LEN] = {0};
    int num = 0;

    while (NULL != fgets(buf, MAX_LINE_LEN, ins_fp)) {
        char* cp = buf;

        char* token = getToken(cp);
        if (1 != sscanf(token, "%u", &num)) {
            return -1;
        }
        fprintf(out_fp, "%d", num);

        while (*(token = getToken(cp)) != '\0') {
            if (g_fea_map.find(token) == g_fea_map.end()) {
                ++g_sum_fea_num;
                g_fea_map[token] = g_sum_fea_num;
                LOG(DEBUG) << "feature [" << token << "] assigned to [" \
                    << g_sum_fea_num << "]";
            }
            fprintf(out_fp, "\t%d", g_fea_map[token]);
        }
        fprintf(out_fp, "\n");

        ++g_sum_ins_num;
    }

    return 0;
}

int main(int argc, char* argv[]) {
    if (parseFlags(argc, argv)) {
        LOG(FATAL) << "Cannot parse flags!" << endl;
        return 1;
    }

    g_sum_fea_num = 0;
    g_sum_ins_num = 0;

    FILE* ins_fp = fopen(FLAGS_instance_file.c_str(), "r");
    if (NULL == ins_fp) {
        printUsage();
        return -2;
    }
    FILE* out_fp = fopen(FLAGS_serialized_instance_file.c_str(), "w");
    if (NULL == ins_fp) {
        printUsage();
        fclose(ins_fp);
        return -2;
    }
    if (0 != serializeInstance(ins_fp, out_fp)) {
        printUsage();
        fclose(ins_fp);
        fclose(out_fp);
        return -3;
    }
    fclose(ins_fp);
    fclose(out_fp);

    LOG(INFO) << "serialized [" << FLAGS_instance_file << "] to [" \
        << FLAGS_serialized_instance_file << "] with [" << g_sum_ins_num \
        << "] lines and [" << g_sum_fea_num << "] features";

    return 0;
}

