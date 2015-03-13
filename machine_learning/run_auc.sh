#!/bin/bash

./calc_auc/calcAuc \
        --root_dir="./" \
        --logtostderr \
        --model_file="./output/model.txt" \
        --instance_file="./data/instance_for_train.txt" \
#        --instance_file="./data/instance_for_eval.txt" \
