sudo apt install libncurses5-dev libncursesw5-dev libssl-dev libelf-dev
cp /boot/config-$(uname -r) .config
date >>date.log
make -j24
date >>date.log
for i in {1..10}; do
    echo -e "\a"
    sleep 0.5
done
