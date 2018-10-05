./UTIL/atob < output_data.txt> output_data.anet.bin
./UTIL/reorder < output_data.anet.bin > tensor.bin
convert -depth 8 -size 660x440+0 gray:tensor.bin tensor.png
xv tensor.png

