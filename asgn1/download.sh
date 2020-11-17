wget "https://cdn.kernel.org/pub/linux/kernel/v5.x/linux-5.8.14.tar.xz"
# git clone git@github.com:torvalds/linux.git --depth 1
xz -d linux-5.8.14.tar.xz
tar -xvf linux-5.8.14.tar
cd linux-5.8.14