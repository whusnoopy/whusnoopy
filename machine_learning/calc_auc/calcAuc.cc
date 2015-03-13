// Copyright 2011 yewen

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>

#include "base/flags.h"
#include "base/logging.h"
#include "base/util.h"

DEFINE_OPTIONAL_FLAGS(string, instance_file, \
        "./data/instance_for_eval.txt", \
        "the instance file for eval");
DEFINE_OPTIONAL_FLAGS(string, model_file, \
        "./output/model.txt", \
        "the model file to eval");

const int MAX_LINE_LEN = 1024000;

const int MAX_FEA_NUM = 10000000;
double mod_fea[MAX_FEA_NUM];

const int MAX_INS_NUM = 10000000;
struct ins_node {
    int num;
    double weight;
    double prob;
};
ins_node ins_res[MAX_INS_NUM];
int ins_num;

int loadModel(FILE* fp) {
    char buf[MAX_LINE_LEN] = {0};
    unsigned int id = 0LL;
    double weight = 0;

    while (NULL != fgets(buf, MAX_LINE_LEN, fp)) {
        char* cp = buf;

        char* token = getToken(cp);
        if (1 != sscanf(token, "%u", &id)) {
            return -1;
        }

        token = getToken(cp);
        if (1 != sscanf(token, "%lf", &weight)) {
            return -1;
        }
        mod_fea[id] = weight;
    }

    LOGF(DEBUG, "load [%u] feature weight", id);
    for (int i = 1; i <= id; ++i) {
        LOGF(DEBUG, "fea[%d] -> %lf", i, mod_fea[i]);
    }

    return 0;
}

double logTrans(double weight) {
    return (1.0 / (1 + exp(-weight)));
}

int loadInstance(FILE* fp) {
    char buf[MAX_LINE_LEN] = {0};
    int num = 0;
    int fea_id = 0;

    ins_num = 0;
    while (NULL != fgets(buf, MAX_LINE_LEN, fp)) {
        char* cp = buf;

        char* token = getToken(cp);
        if (1 != sscanf(token, "%u", &num)) {
            return -1;
        }
        ins_res[ins_num].num = num;
        ins_res[ins_num].weight = 0;

        while (*(token = getToken(cp)) != '\0') {
            if (1 != sscanf(token, "%u", &fea_id)) {
                return -1;
            }
            ins_res[ins_num].weight += mod_fea[fea_id];
            LOGF(DEBUG, "ins [%d] has feature [%d] with weight [%f], "
                    "and sum weight of ins [%d] is [%f] now",
                    ins_num, fea_id, mod_fea[fea_id],
                    ins_num, ins_res[ins_num].weight);
        }
        ins_res[ins_num].prob = logTrans(ins_res[ins_num].weight);

        ++ins_num;
    }

    return 0;
}

int cmp(const void *a, const void *b)
{
     ins_node *aa = (ins_node *)a;
     ins_node *bb = (ins_node *)b;
     return(((aa->prob)-(bb->prob)>0)?1:-1);
}

double calcAuc() {
    double res = 0;

    qsort(ins_res, ins_num, sizeof(ins_node), cmp);

    double sum_pos = 0;
    double sum_neg = 0;
    double s0 = 0;
    int count = 0;
    for (int i = 0; i < ins_num; ++i) {
        LOGF(DEBUG, "ins [%d] -> num [%d], weight [%f], prob [%f]",
                i, ins_res[i].num, ins_res[i].weight, ins_res[i].prob);
        int this_num = abs(ins_res[i].num);
        if (ins_res[i].num < 0) {
            sum_neg += this_num;
        } else {
            sum_pos += this_num;
            s0 += ((count + 1) + (count + this_num)) * this_num / 2;
        }

        count += this_num;
        LOGF(DEBUG, "s0 [%lf] sum_pos [%lf] sum_neg [%lf]", s0, sum_pos, sum_neg);
    }

    res = (s0 - (sum_pos*(sum_pos+1))/2.0)/(sum_pos*sum_neg);

    return res;
}

int main(int argc, char* argv[]) {
    if (parseFlags(argc, argv)) {
        LOG(FATAL) << "Cannot parse flags!" << endl;
        return 1;
    }

    FILE* mod_fp = fopen(FLAGS_model_file.c_str(), "r");
    if (NULL == mod_fp) {
        printUsage();
        return -2;
    }
    if (0 != loadModel(mod_fp)) {
        printUsage();
        fclose(mod_fp);
        return -2;
    }
    fclose(mod_fp);

    FILE* ins_fp = fopen(FLAGS_instance_file.c_str(), "r");
    if (NULL == ins_fp) {
        printUsage();
        return -2;
    }
    if (0 != loadInstance(ins_fp)) {
        printUsage();
        fclose(ins_fp);
        return -3;
    }
    fclose(ins_fp);

    double auc_res = 0;
    auc_res = calcAuc();
    LOGF(INFO, "Calc auc by model [%s] to instance [%s]: %lf", \
            FLAGS_model_file.c_str(), FLAGS_instance_file.c_str(), \
            auc_res);

    return 0;
}


