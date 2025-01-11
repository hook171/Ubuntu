#!/bin/bash

git pull origin main

LABS=("2Laba" "4Laba" "5Laba")

for LAB in "${LABS[@]}"; do

  if [ ! -d "../$LAB/build_ubuntu" ]; then
    mkdir -p "../$LAB/build_ubuntu"
  fi

  cd "../$LAB/build_ubuntu"

  cmake ..
  cmake --build .

  cd ../../$LAB
done