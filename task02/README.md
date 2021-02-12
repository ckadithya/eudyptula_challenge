# Eudyptula Challenge - Task02
## Getting started
- I partitioned my system with microsoft OS and Ubuntu 20.04. I have done this task02 entirely in my laptop using Ubuntu 20.4
- I have completed the `A BEGINNER'S GUIDE TO LINUX KERNEL DEVELOPMENT - LFD103` and I refered back all the commands I used in this task from there.
- I started by cloning Linus torvalds's latest git tree from git.kernel.org `https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/`
```
root@ckadithya:/home/adithya/Workspace/eudyptula_challenge/task02/linux_latest#git clone //git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git linux_latest
cd linux_latest
```
- I copied the configuration of my current kernel from my /boot and to the `.config`. It was suggested as the safe approach in the course.
```
root@ckadithya:/home/adithya/Workspace/eudyptula_challenge/task02/linux_latest# ls /boot
config-5.11.0-rc7-00084-g291009f656e8      initrd.img-5.4.0-65-generic                System.map-5.7.7
config-5.4.0-050400-generic                initrd.img.old                             vmlinuz
config-5.4.0-65-generic                    memtest86+.bin                             vmlinuz-5.11.0-rc7-00084-g291009f656e8
config-5.7.7                               memtest86+.elf                             vmlinuz-5.4.0-050400-generic
efi                                        memtest86+_multiboot.bin                   vmlinuz-5.4.0-65-generic
grub                                       System.map-5.11.0-rc7-00084-g291009f656e8  vmlinuz-5.7.7
initrd.img                                 System.map-5.4.0-050400-generic            vmlinuz.old
initrd.img-5.11.0-rc7-00084-g291009f656e8  System.map-5.4.0-65-generic
root@ckadithya:/home/adithya/Workspace/eudyptula_challenge/task02/linux_latest#cp /boot/config-5.4.0-65-generic .config
```
### Compiling the kernel:
- To generate a kernel configuration and base it on a working kernel configuration as suggested in the course. I used `make localmodconfig`. This option create configuration based on the modules listed in my system
```
root@ckadithya:/home/adithya/Workspace/eudyptula_challenge/task02/linux_latest#lsmod > /tmp/my-lsmod
root@ckadithya:/home/adithya/Workspace/eudyptula_challenge/task02/linux_latest#make LSMOD=/tmp/my-lsmod localmodconfig
```
- To compile the kernel I used `make -j3 all`, I learned the option `-j` through the course, which is used to run the make faster by telling how many jobs to run simultaneously. 

#### Installing the kernel
- After the compiling was done I proceeded with installing.
- Inorder to install the kerenl I used `su -c "make modules_install install"` and then the kernel was installed.
- Next, by running `update-grub` the new kernel was added to the grub menu.
- To compare and look for any regression the logs from the current kernel was saved in `current_dmesg` by performing the following commands
```
dmesg -t > dmesg_current
dmesg -t -k > dmesg_kernel
dmesg -t -l emerg > dmesg_current_emerg
dmesg -t -l alert > dmesg_current_alert
dmesg -t -l crit > dmesg_current_crit
dmesg -t -l err > dmesg_current_err
dmesg -t -l warn > dmesg_current_warn
```
- Likewise dmesg logs from the newly installed kernel was saved in `latest_dmesg`. Both of which are attached to the mail for your reference.

##### Booting the kernel
- After installation booting the kernel was done. Before that, I changed the default grub configuration file /etc/default/grub to the boot menu, to pause a bit so that we are able to select the kernel to boot(in case something goes wrong). This was done using the following steps:
```
vim /etc/default/grub
GRUB_TIMEOUT=10
#GRUB_TIMEOUT_STYLE=hidden
GRUB_CMDLINE_LINUX="earlyprink=vga"
```
- Now grub was updated using `sudo update-grub`
- Then the system was `reboot` into the newly installed kernel and the new dmesg was saved as metioned above. Boot option has multiple kernel and I chose the kernel 5.11.0-rc7-00084-g291009f656e8, as it is the recently compiled and installed one. Then I took the dmesg for the latest kernel using the previous steps.

```
root@ckadithya:/home/adithya/Workspace/eudyptula_challenge/task02/latest_dmesg# ls -lt
total 136
-rw-r--r-- 1 root root  1326 Feb 11 01:25 dmesg_latest_warn
-rw-r--r-- 1 root root   182 Feb 11 01:25 dmesg_latest_err
-rw-r--r-- 1 root root     0 Feb 11 01:25 dmesg_latest_crit
-rw-r--r-- 1 root root     0 Feb 11 01:25 dmesg_latest_alert
-rw-r--r-- 1 root root    44 Feb 11 01:25 dmesg_latest_emerg
-rw-r--r-- 1 root root 58011 Feb 11 01:25 dmesg_latest_kernel
-rw-r--r-- 1 root root 61548 Feb 11 01:25 dmesg_latest
```

##### Comparing Latest and previous Kernel dmesg
- The log messages of `dmesg_current` and `dmesg_latest` are as follows
```
root@ckadithya:/home/adithya/Workspace/eudyptula_challenge/task02/current_dmesg# cat dmesg_current
microcode: microcode updated early to revision 0xe2, date = 2020-07-14
Linux version 5.4.0-65-generic (buildd@lcy01-amd64-018) (gcc version 9.3.0 (Ubuntu 9.3.0-17ubuntu1~20.04)) #73-Ubuntu SMP Mon Jan 18 17:25:17 UTC 2021 (Ubuntu 5.4.0-65.73-generic 5.4.78)
Command line: BOOT_IMAGE=/boot/vmlinuz-5.4.0-65-generic root=UUID=db747fa5-b16c-4d3e-8911-a8778177263d ro earlyprink=vga quiet splash
KERNEL supported cpus:
  Intel GenuineIntel
  AMD AuthenticAMD
  Hygon HygonGenuine
  Centaur CentaurHauls
  zhaoxin   Shanghai  
x86/fpu: Supporting XSAVE feature 0x001: 'x87 floating point registers'
x86/fpu: Supporting XSAVE feature 0x002: 'SSE registers'
x86/fpu: Supporting XSAVE feature 0x004: 'AVX registers'
x86/fpu: Supporting XSAVE feature 0x008: 'MPX bounds registers'
x86/fpu: Supporting XSAVE feature 0x010: 'MPX CSR'
x86/fpu: xstate_offset[2]:  576, xstate_sizes[2]:  256
x86/fpu: xstate_offset[3]:  832, xstate_sizes[3]:   64
x86/fpu: xstate_offset[4]:  896, xstate_sizes[4]:   64
x86/fpu: Enabled xstate features 0x1f, context size is 960 bytes, using 'compacted' format.
BIOS-provided physical RAM map:
BIOS-e820: [mem 0x0000000000000000-0x0000000000057fff] usable
BIOS-e820: [mem 0x0000000000058000-0x0000000000058fff] reserved
BIOS-e820: [mem 0x0000000000059000-0x000000000009dfff] usable
BIOS-e820: [mem 0x000000000009e000-0x00000000000fffff] reserved
BIOS-e820: [mem 0x0000000000100000-0x0000000066957fff] usable
BIOS-e820: [mem 0x0000000066958000-0x0000000066958fff] ACPI NVS
BIOS-e820: [mem 0x0000000066959000-0x0000000066959fff] reserved
BIOS-e820: [mem 0x000000006695a000-0x0000000068387fff] usable
BIOS-e820: [mem 0x0000000068388000-0x0000000068c87fff] reserved
BIOS-e820: [mem 0x0000000068c88000-0x000000007ee9dfff] usable
BIOS-e820: [mem 0x000000007ee9e000-0x000000007f88dfff] reserved
BIOS-e820: [mem 0x000000007f88e000-0x000000007ff7dfff] ACPI NVS
BIOS-e820: [mem 0x000000007ff7e000-0x000000007fffdfff] ACPI data
BIOS-e820: [mem 0x000000007fffe000-0x000000007fffefff] usable
BIOS-e820: [mem 0x000000007ffff000-0x000000008affffff] reserved
BIOS-e820: [mem 0x00000000e0000000-0x00000000efffffff] reserved
BIOS-e820: [mem 0x00000000fd000000-0x00000000fe7fffff] reserved
BIOS-e820: [mem 0x00000000fec00000-0x00000000fec00fff] reserved
BIOS-e820: [mem 0x00000000fed00000-0x00000000fed00fff] reserved
BIOS-e820: [mem 0x00000000fed10000-0x00000000fed19fff] reserved
BIOS-e820: [mem 0x00000000fed84000-0x00000000fed84fff] reserved
BIOS-e820: [mem 0x00000000fee00000-0x00000000fee00fff] reserved
BIOS-e820: [mem 0x00000000ffa10000-0x00000000ffffffff] reserved
BIOS-e820: [mem 0x0000000100000000-0x0000000573ffffff] usable
.....
.....
QNX4 filesystem 0.2.3 registered.
raid6: avx2x4   gen()  3817 MB/s
raid6: avx2x4   xor() 12049 MB/s
raid6: avx2x2   gen() 17433 MB/s
raid6: avx2x2   xor() 10782 MB/s
raid6: avx2x1   gen() 14185 MB/s
raid6: avx2x1   xor()  9918 MB/s
raid6: sse2x4   gen()  8164 MB/s
raid6: sse2x4   xor()  5062 MB/s
raid6: sse2x2   gen()  6821 MB/s
raid6: sse2x2   xor()  4633 MB/s
raid6: sse2x1   gen()  6079 MB/s
raid6: sse2x1   xor()  3549 MB/s
raid6: using algorithm avx2x2 gen() 17433 MB/s
raid6: .... xor() 10782 MB/s, rmw enabled
raid6: using avx2x2 recovery algorithm
xor: automatically using best checksumming function   avx       
Btrfs loaded, crc32c=crc32c-intel
perf: interrupt took too long (6513 > 6495), lowering kernel.perf_event_max_sample_rate to 30500
```
- From the above log we can see that I have the `Linux version 5.4.0-65-generic (buildd@lcy01-amd64-018) (gcc version 9.3.0 (Ubuntu 9.3.0-17ubuntu1~20.04)) #73-Ubuntu SMP Mon Jan 18 17:25:17 UTC 2021 (Ubuntu 5.4.0-65.73-generic 5.4.78)` the old kernel installed in my system through the installation of Ubuntu 20.04.

- The logs of the latest kernel 5.11.0-rc7-00084-g291009f656e8 are as follows
```
root@ckadithya:/home/adithya/Workspace/eudyptula_challenge/task02/latest_dmesg# cat dmesg_latest
microcode: microcode updated early to revision 0xe2, date = 2020-07-14
Linux version 5.11.0-rc7-00084-g291009f656e8 (root@ckadithya) (gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0, GNU ld (GNU Binutils for Ubuntu) 2.34) #1 SMP Thu Feb 11 00:49:32 PST 2021
Command line: BOOT_IMAGE=/boot/vmlinuz-5.11.0-rc7-00084-g291009f656e8 root=UUID=db747fa5-b16c-4d3e-8911-a8778177263d ro earlyprink=vga quiet splash
KERNEL supported cpus:
  Intel GenuineIntel
  AMD AuthenticAMD
  Hygon HygonGenuine
  Centaur CentaurHauls
  zhaoxin   Shanghai  
x86/fpu: Supporting XSAVE feature 0x001: 'x87 floating point registers'
x86/fpu: Supporting XSAVE feature 0x002: 'SSE registers'
x86/fpu: Supporting XSAVE feature 0x004: 'AVX registers'
x86/fpu: Supporting XSAVE feature 0x008: 'MPX bounds registers'
x86/fpu: Supporting XSAVE feature 0x010: 'MPX CSR'
x86/fpu: xstate_offset[2]:  576, xstate_sizes[2]:  256
x86/fpu: xstate_offset[3]:  832, xstate_sizes[3]:   64
x86/fpu: xstate_offset[4]:  896, xstate_sizes[4]:   64
x86/fpu: Enabled xstate features 0x1f, context size is 960 bytes, using 'compacted' format.
BIOS-provided physical RAM map:
BIOS-e820: [mem 0x0000000000000000-0x0000000000057fff] usable
BIOS-e820: [mem 0x0000000000058000-0x0000000000058fff] reserved
BIOS-e820: [mem 0x0000000000059000-0x000000000009dfff] usable
BIOS-e820: [mem 0x000000000009e000-0x00000000000fffff] reserved
BIOS-e820: [mem 0x0000000000100000-0x0000000066957fff] usable
BIOS-e820: [mem 0x0000000066958000-0x0000000066958fff] ACPI NVS
BIOS-e820: [mem 0x0000000066959000-0x0000000066959fff] reserved
BIOS-e820: [mem 0x000000006695a000-0x0000000068387fff] usable
BIOS-e820: [mem 0x0000000068388000-0x0000000068c87fff] reserved
BIOS-e820: [mem 0x0000000068c88000-0x000000007ee9dfff] usable
BIOS-e820: [mem 0x000000007ee9e000-0x000000007f88dfff] reserved
BIOS-e820: [mem 0x000000007f88e000-0x000000007ff7dfff] ACPI NVS
BIOS-e820: [mem 0x000000007ff7e000-0x000000007fffdfff] ACPI data
BIOS-e820: [mem 0x000000007fffe000-0x000000007fffefff] usable
BIOS-e820: [mem 0x000000007ffff000-0x000000008affffff] reserved
BIOS-e820: [mem 0x00000000e0000000-0x00000000efffffff] reserved
BIOS-e820: [mem 0x00000000fd000000-0x00000000fe7fffff] reserved
BIOS-e820: [mem 0x00000000fec00000-0x00000000fec00fff] reserved
BIOS-e820: [mem 0x00000000fed00000-0x00000000fed00fff] reserved
BIOS-e820: [mem 0x00000000fed10000-0x00000000fed19fff] reserved
BIOS-e820: [mem 0x00000000fed84000-0x00000000fed84fff] reserved
BIOS-e820: [mem 0x00000000fee00000-0x00000000fee00fff] reserved
BIOS-e820: [mem 0x00000000ffa10000-0x00000000ffffffff] reserved
BIOS-e820: [mem 0x0000000100000000-0x0000000573ffffff] usable
....
....
bridge: filtering via arp/ip/ip6tables is no longer available by default. Update your scripts to load br_netfilter if you need this.
Bridge firewalling registered
bpfilter: Loaded bpfilter_umh pid 1339
Started bpfilter
rfkill: input handler disabled
Initializing XFRM netlink socket
process 'docker/tmp/qemu-check138357611/check' started with executable stack
rfkill: input handler enabled
Bluetooth: RFCOMM TTY layer initialized
Bluetooth: RFCOMM socket layer initialized
Bluetooth: RFCOMM ver 1.11
rfkill: input handler disabled

```
- As you can see 5.11.0-rc7-00084-g291009f656e8 kernel dmesg shows `Linux version 5.11.0-rc7-00084-g291009f656e8 (root@ckadithya) (gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0, GNU ld (GNU Binutils for Ubuntu) 2.34) #1 SMP Thu Feb 11 00:49:32 PST 2021` which is compiled and installed by me, the host `root@ckadithya`, whereas previous kernel show `buildd@lcy01-amd64-018` where the host is `lcy01-amd64-018` a auto-mode builder for the amd64 and i386 binaries[https://launchpad.net/builders/lcy01-amd64-018](https://launchpad.net/builders/lcy01-amd64-018).

- This shows the proof that the kernel is compiled and installed by me in my system.
