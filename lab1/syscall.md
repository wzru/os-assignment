# Linux内核增加系统调用

```bash
sudo apt install git libncurses5-dev libncursesw5-dev libssl-dev libelf-dev
git clone git@gitee.com:mirrors/linux.git --depth 1
```

```bash
cd linux
cp /boot/config-$(uname -r) .config
```

首先增加函数声明和定义

```bash
vi include/linux/syscalls.h
```

```c
asmlinkage int sys_hello(void);
asmlinkage int sys_add(int a,int b);
```

```bash
vi kernel/sys.c
```

```c
SYSCALL_DEFINE0(hello) {
        printk("Hello world! From new syscall by Shaun.");
        return 1;
}
SYSCALL_DEFINE2(add, int, a, int, b) {
        return a+b;
}
```

添加x86_64的调用号，根据提示要放在440的后面

```bash
vi arch/x86/entry/syscalls/syscall_64.tbl
```

```
441 common  hello           sys_hello
442 common  add             sys_add
```

```bash
make -j10
sudo make modules_install
sudo make install
sudo reboot
```

