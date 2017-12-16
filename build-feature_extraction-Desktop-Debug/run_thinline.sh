#!/bin/bash

for i in {1..9}; do
  ./feature_extration marker_thinline/marker_thinline_0$i.png
for i in {10..30}; do
  ./feature_extration marker_thinline/marker_thinline_$i.png

done
