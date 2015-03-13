#!/bin/bash

./feature_extractor/feature_serialization \
        --root_dir="./" \
        --logtostderr \
        --log_level=5 \
        --instance_file="./data/instance_for_train.txt" \
        --serialized_instance_file="./output/instance_for_train.txt" \
