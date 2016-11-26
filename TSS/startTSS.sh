#!/bin/bash

DATE=`date +%Y%m%d_%H%M%S`
filename="${DATE}_TSSLog.txt"

./TSS |& tee ${filename}
