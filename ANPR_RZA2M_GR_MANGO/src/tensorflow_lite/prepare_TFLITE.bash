#!/bin/bash -f

# step 1: clone tensorflow "tensorflow" repository:
git clone https://github.com/tensorflow/tensorflow.git

export branch="r2.6"

cd tensorflow
git checkout ${branch}

### step 2: generate all projects to get all required downloads (the python script to create the tflm tree may fail to download certain libraries)
make -f tensorflow/lite/micro/tools/make/Makefile TARGET_ARCH=cortex-a9 OPTIMIZED_KERNEL_DIR=cmsis_nn generate_projects

### step 3: prepare tflm tree (with cmsis_nn)
python3 tensorflow/lite/micro/tools/project_generation/create_tflm_tree.py --makefile_options="TARGET_ARCH=cortex-a9 OPTIMIZED_KERNEL_DIR=cmsis_nn" "tensorflow_lite"


### step 4: modifications (CMSIS_NN)

mkdir ./tensorflow_lite/patches

cp ../scripts/patches/patches_${branch}/fully_connected.h.patch ./tensorflow_lite/patches
patch -b tensorflow_lite/tensorflow/lite/micro/kernels/fully_connected.h ./tensorflow_lite/patches/fully_connected.h.patch


###

cp ./tensorflow/lite/micro/tools/make/templates/library.properties ./tensorflow_lite
cp ../scripts/prepare_TFLITE.bash ./tensorflow_lite

zip -r - tensorflow_lite > tensorflow_lite_${branch}.zip
cd ..

cp tensorflow/tensorflow_lite_${branch}.zip .
