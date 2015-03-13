#!/bin/bash

./train/trainModel \
        --root_dir="./" \
        --logtostderr=false \
        --log_level=4 \
        --instance_file="./data/instance_for_train.txt" \
        --model_file="./output/model.txt" \
        --eps=1e-4 \
        --theta=4.0 \
        --max_round=1000 \
        --stop_times=3 \
        --c=0.1 \
        --self_beta0=0 \
