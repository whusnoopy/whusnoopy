default : all

all : .base .train .calc_auc .feature_extractor

clean :
	@make -C base clean
	@make -C train clean
	@make -C calc_auc clean
	@make -C feature_extractor clean
	@rm -rf bin
	@rm -rf log/*

install : all
	@rm -rf bin
	@mkdir bin
	@mv calc_auc/calcAuc bin/
	@mv train/trainModel bin/
	@mv feature_extractor/feature_serialization bin/
	@mv feature_extractor/feature_extractor bin/

.base :
	make -C base

.train :
	make -C train

.calc_auc :
	make -C calc_auc

.feature_extractor :
	make -C feature_extractor
