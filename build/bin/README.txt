
The ABI (reads executables) need to know where shared libraries are found.
Add:
export LD_LIBRARY_PATH=/home/odroid/Documents/cross_compile/build/bin


Easy way to copy libs:
cp /usr/local/armhf/lib/libopencv_imgcodecs.so* ./build/bin/
