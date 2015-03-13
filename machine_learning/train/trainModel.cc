// Copyright 2011 yewen

#include <cstdio>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "base/flags.h"
#include "base/logging.h"
#include "base/util.h"

#include <iostream>

using namespace std;

DEFINE_OPTIONAL_FLAGS(int, stop_times, 5, \
        "when to stop after round diff less than eps");
DEFINE_OPTIONAL_FLAGS(double, eps, 1e-6, \
        "the stop value of fabs(sum(w_k - w_k-1))");
DEFINE_OPTIONAL_FLAGS(double, theta, 1.0, \
        "the learning rate in LR");
DEFINE_OPTIONAL_FLAGS(double, c, 2.0, \
        "the regularization factor");
DEFINE_OPTIONAL_FLAGS(int, max_round, 100, \
        "max round the training program runs");
DEFINE_OPTIONAL_FLAGS(string, instance_file, \
        "./data/instance_for_training.txt", \
        "the instance file for training");
DEFINE_OPTIONAL_FLAGS(string, model_file, \
        "./output/model.txt", \
        "the output model file");
DEFINE_OPTIONAL_FLAGS(int, self_beta0, 0, \
        "is the beta0 feature include already");

const int MAX_LINE_LEN = 1024000;

const int MAX_INS_NUM = 1000000;
const int MAX_INS_FEA = 50;
struct InsNode {
    int times;
    int fea_num;
    int feature[MAX_INS_FEA];
};
InsNode g_ins[MAX_INS_NUM];
int g_sum_ins_num;
int g_sum_fea_num;

const int MAX_FEA_NUM = 10000000;
double g_w[2][MAX_FEA_NUM];

int g_curr_w = 0;

int loadInstance() {
    LOG(INFO) << "Start to load instance from " << FLAGS_instance_file;

    FILE* fp = fopen(FLAGS_instance_file.c_str(), "r");
    if (NULL == fp) {
        LOG(FATAL) << "Cannot open " << FLAGS_instance_file \
            << " to load instance";
        return -1;
    }

    g_sum_fea_num = 0;
    char buf[MAX_LINE_LEN] = {0};
    int ins_id = 0;
    int times = 0;
    int fea_num = 0;
    int fea_id = 0;

    while (NULL != fgets(buf, MAX_LINE_LEN, fp)) {
        char* cp = buf;

        char* token = getToken(cp);
        if (1 != sscanf(token, "%u", &times)) {
            return -1;
        }
        g_ins[ins_id].times = times;
        fea_num = 0;

        while (*(token = getToken(cp)) != '\0') {
            if (1 != sscanf(token, "%u", &fea_id)) {
                return -1;
            }
            if (fea_id > g_sum_fea_num) {
                g_sum_fea_num = fea_id;
            }
            if (fea_num >= MAX_INS_FEA) {
                continue;
            }
            g_ins[ins_id].feature[fea_num] = fea_id;
            ++fea_num;
        }
        g_ins[ins_id].fea_num = fea_num;

        ++ins_id;
    }
    g_sum_ins_num = ins_id;

    fclose(fp);

    LOG(INFO) << "finish load instance file with [" << g_sum_ins_num \
        << "] instance and [" << (g_sum_fea_num + 1 - FLAGS_self_beta0) \
        << "] featues";

    return 0;
}

double logisticTrans(double weight) {
    return (1.0 / (1 + exp(-weight)));
}

// Gradient Descent
int nextRoundGradient() {
    double zi = 0;
    double gi = 0;

    for (int i = 0; i <= g_sum_ins_num; ++i) {
        zi = g_w[g_curr_w][0];
        for (int j = 0; j < g_ins[i].fea_num; ++j) {
            zi += g_w[g_curr_w][g_ins[i].feature[j]];
        }
        if (g_ins[i].times > 0) {
            gi = logisticTrans(-zi);
        } else {
            gi = logisticTrans(zi);
        }
        if (!FLAGS_self_beta0) {
            g_w[g_curr_w][0] += FLAGS_theta * g_ins[i].times * gi;
        }
        for (int j = 0; j < g_ins[i].fea_num; ++j) {
            g_w[g_curr_w][g_ins[i].feature[j]] +=
                FLAGS_theta * g_ins[i].times * gi;
        }
    }

    // regularization
    int last_w = 1 - g_curr_w;
    for (int k = FLAGS_self_beta0; k <= g_sum_fea_num; ++k) {
        g_w[g_curr_w][k] -= FLAGS_theta * FLAGS_c * g_w[last_w][k];
    }

    return 0;
}

int nextRoundNewton() {
    // TODO: get the Hessian matrix and get the [H]^-1
    //       seems like 1G memory can support 10K features only?

    // TODO: update the g_w{t+1}

    return 0;
}

double nextRound() {
    // init this round
    int last_w = 1 - g_curr_w;
    for (int k = FLAGS_self_beta0; k <= g_sum_fea_num; ++k) {
        g_w[g_curr_w][k] = g_w[last_w][k];
    }

    nextRoundGradient();
    // nextRoundNewton();

    // calc diff
    double diff = 0;
    double sum = 0;
    for (int i = FLAGS_self_beta0; i <= g_sum_fea_num; ++i) {
        diff += fabs(g_w[g_curr_w][i] - g_w[last_w][i]);
        sum += fabs(g_w[g_curr_w][i]);
        LOG(DEBUG) << "    Feature " << i << " value: " << g_w[g_curr_w][i]
            << ". diff: " << fabs(g_w[g_curr_w][i] - g_w[last_w][i]);
    }

    LOG(DEBUG) << "sum of |w|: " << sum;

    return diff;
}

int trainModel() {
    memset(g_w, 0, sizeof(g_w));
    int less_times = 0;
    int round = 0;
    double diff = 0;
    g_curr_w = 0;
    while (less_times < FLAGS_stop_times && round < FLAGS_max_round) {
        g_curr_w = 1 - g_curr_w;
        ++round;

        diff = nextRound();

        if (diff > FLAGS_eps) {
            less_times = 0;
        } else {
            ++less_times;
        }
        LOG(INFO) << "Round: " << round << ", diff = " << diff \
            << ", less_times = " << less_times;
    }

    return 0;
}

int outputModel() {
    LOG(INFO) << "start output model to " << FLAGS_model_file;

    FILE* fp = fopen(FLAGS_model_file.c_str(), "w");
    if (NULL == fp) {
        LOG(FATAL) << "Cannot open " << FLAGS_model_file \
            << " for output model";
        return -1;
    }

    for (int i = FLAGS_self_beta0; i <= g_sum_fea_num; ++i) {
        fprintf(fp, "%d\t%f\n", i, g_w[g_curr_w][i]);
    }

    fclose(fp);

    LOG(INFO) << "finish output model with [" \
        << (g_sum_fea_num + 1 - FLAGS_self_beta0) \
        << "] features, model file = " << FLAGS_model_file;

    return 0;
}

int main(int argc, char* argv[]) {
    if (parseFlags(argc, argv)) {
        LOG(FATAL) << "Cannot parse flags!" << endl;
        return 1;
    }

    LOG(INFO) << "start to train model for instance file [" \
        << FLAGS_instance_file << "]";
    LOG(INFO) << "  with eps = " << stringPrintf("%lf", FLAGS_eps);
    LOG(INFO) << "       stop_times = " << FLAGS_stop_times;
    LOG(INFO) << "       learing_rate = " << FLAGS_theta;
    LOG(INFO) << "       regularization_factor = " << FLAGS_c;

    loadInstance();
    trainModel();
    outputModel();

    return 0;
}

