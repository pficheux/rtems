3/3/2018
========

- Nouveau test RTEMS 5

* nécessaire d'installer python2.7-dev

$ sudo apt-get install python2.7-dev -> https://learnsom2day.wordpress.com/2015/04/21/getting-started-with-rtems-hello-word

* Installation compilateur croisé  (QEMU/i386) avec:

https://docs.rtems.org/branches/master/rsb/source-builder.html

$ git clone git://git.rtems.org/rtems-source-builder.git

$ cd rtems-source-builder/

$ ./source-builder/sb-check
RTEMS Source Builder - Check, 5 (d255e812abd5 modified)
Environment is ok

$ cd rtems/
$ ../source-builder/sb-set-builder --log=l-i386.txt --prefix=$HOME/RTEMS/rtems-i386 5/rtems-i386

* Compilation SDK

$ export PATH=$PATH:~/RTEMS/rtems-i386/bin

$ git clone git://git.rtems.org/rtems.git rtems_git
$ cd rtems_git

$ ./bootstrap

$ mkdir build && build
$ $HOME/RTEMS/rtems_git/configure --target=i386-rtems5 --disable-cxx --enable-rtemsbsp=pc386 --prefix=$HOME/RTEMS/target_i386

$ make -j 4
$ make install

* Test application

$ . ./set_env_i386.sh 
$ cd exemples/POSIX/helloworld
$ make
$ qemu-system-i386 -kernel o-optimize/helloworld.exe

idem avec timer POSIX

- Voir portage BBB -> https://devel.rtems.org/wiki/Developer/Projects/Open/ImproveBeagleBSP

20/10/2018
==========

- Test RTEMS 5 / RPi

* Production toolchain

$ cd rtems-source-builder/
$ ./source-builder/sb-check

$ cd rtems/
$ ../source-builder/sb-set-builder --log=l-arm.txt --prefix=$HOME/RTEMS/rtems-arm  5/rtems-arm

* Production SDK

$ export PATH=$PATH:~/RTEMS/rtems-arm/bin
$ cd rtems_git/
$ mkdir build && cd build
$ $HOME/RTEMS/rtems_git/configure --target=arm-rtems5 --disable-cxx --enable-rtemsbsp=raspberrypi --prefix=$HOME/RTEMS/target_rpi
$ make
$ make install

* Création set_env_rpi.sh

#!/bin/sh

export PATH=$PATH:~/RTEMS/rtems-arm/bin
export RTEMS_MAKEFILE_PATH=~/RTEMS/target_rpi/arm-rtems5/raspberrypi


* Exemple timer

$ . ~/RTEMS/set_env_rpi.sh 
$ make clean
$ make
$ cp o-optimize/timer.ralf /media/pierre/48BD-FC67/

-> OK

- Test BBB

$ mkdir build && cd build
$ $HOME/RTEMS/rtems_git/configure --target=arm-rtems5 --disable-cxx --enable-rtemsbsp=beagleboneblack --prefix=$HOME/RTEMS/target_bbb
$ make
$ make install

* Création set_env_bbb.sh

#!/bin/sh

export PATH=$PATH:~/RTEMS/rtems-arm/bin
export RTEMS_MAKEFILE_PATH=~/RTEMS/target_bbb/arm-rtems5/beagleboneblack


* Exemple timer

$ . ~/RTEMS/set_env_bbb.sh 
$ make clean
$ make

Suite d'après le post https://lists.rtems.org/pipermail/users/2018-March/032133.html

$ arm-rtems5-objcopy timer.exe -O binary timer.bin
$ gzip -9 timer.bin 
$ mkimage -A arm -O linux -T kernel -a 0x80000000 -e 0x80000000 -n RTEMS -d timer.bin.gz timer.img

Chargement de timer.img sur la avec SD-card ou TFTP

1- SD

On crée le fichier uEnv.txt avec les lignes suivantes dans uEnv.txt (voir le lien)

setenv bootdelay 5
uenvcmd=run boot
boot=fatload mmc 0 0x80800000 timer.img ; fatload mmc 0 0x88000000!! am335x-boneblack.dtb ; bootm 0x80800000 - 0x88000000


2- TFT

On charge l'image timer.img avec TFTP

=> dhcp 0x80800000 192.168.1.23:timer.img                                       
link up on port 0, speed 100, full duplex                                       
BOOTP broadcast 1                                                               
*** Unhandled DHCP Option in OFFER/ACK: 125                                     
*** Unhandled DHCP Option in OFFER/ACK: 125                                     
DHCP client bound to address 192.168.1.39 (93 ms)                               
Using ethernet@4a100000 device                                                  
TFTP from server 192.168.1.23; our IP address is 192.168.1.39                   
Filename 'timer.img'.                                                           
Load address: 0x80800000                                                        
Loading: ##############                                                         
         160.2 KiB/s                                                            
done                                                                            
Bytes transferred = 66601 (10429 hex)                                           

On charge le DT depuis la SD

=> fatload mmc 0 0x88000000 am335x-boneblack.dtb
reading am335x-boneblack.dtb                                                    
37853 bytes read in 18 ms (2 MiB/s)

On démarre l'appli

=> bootm 0x80800000 - 0x88000000 
## Booting kernel from Legacy Image at 80800000 ...                             
   Image Name:   RTEMS                                                          
   Created:      2018-10-20  11:08:29 UTC                                       
   Image Type:   ARM Linux Kernel Image (gzip compressed)                       
   Data Size:    66537 Bytes = 65 KiB                                           
   Load Address: 80000000                                                       
   Entry Point:  80000000                                                       
   Verifying Checksum ... OK                                                    
## Flattened Device Tree blob at 88000000                                       
   Booting using the fdt blob at 0x88000000                                     
   Uncompressing Kernel Image ... OK                                            
   Loading Device Tree to 8fff3000, end 8ffff3dc ... OK                         
                                                                                
Starting kernel ...                                                             
                                                                                
RTEMS Beagleboard: am335x-based                                                 
                                                                                
 *** Ticks per second in your system: 100                                       
Got signal 14 (1)                                                               
Got signal 14 (2)                                                               
Got signal 14 (3)                                                               
Got signal 14 (4)                
...


22/04/2021
==========

- Test rtems-5.1 (version stable)

https://ftp.rtems.org/pub/rtems/releases/5/5.1/#rtems-documentation
https://ftp.rtems.org/pub/rtems/releases/5/5.1/sources/rtems-5.1.tar.xz

* Nouveau test QEMU/i386

-> recompilation de GCC afin d'éviter l'erreur SOCKCLOSE (newlib)
-> compilation BSP

$ . ./set_env_i386.sh
$ cd rtems-5.1
$ mkdir build && cd build
$ /media/pierre/SANDISK_PF/RTEMS/rtems_git/configure  --target=i386-rtems5 --disable-cxx --enable-rtemsbsp=pc386 --prefix=/media/pierre/SANDISK_PF/RTEMS/target_i386

-> test des applications POSIX OK :-)

$ qemu-system-i386 -kernel o-optimize/timer.exe (ou helloworld.exe)

-> même test sur la branche 5.1 de la version Git

* Création du compilateur RISC-V 

$ git clone git://git.rtems.org/rtems-source-builder.git
$ git checkout -b v5.1 5.1

$ cd rtems-source-builder/
$ ./source-builder/sb-check
$ cd rtems
$ ../source-builder/sb-set-builder --log=l-riscv.txt --prefix=/media/pierre/SANDISK_PF/RTEMS/rtems-riscv 5/rtems-riscv

* Compilation BSP rv32imafdc

voir https://www.mail-archive.com/users@rtems.org/msg02775.html


$ /media/pierre/SANDISK_PF/RTEMS/rtems-5.1/configure --target=riscv-rtems5 --disable-cxx --enable-rtemsbsp=rv32imafdc --prefix=/media/pierre/SANDISK_PF/RTEMS/target_rv32imafdcb
$ make -j 12
$ make install

* Compilation QEMU RISC-V 32 et 64

-> voir https://risc-v-getting-started-guide.readthedocs.io/en/latest/linux-qemu.html

$ sudo apt-get install ninja-build libpixman-1-dev

$ git clone https://github.com/qemu/qemu.git qemu_git
$ cd qemu_git/
$ git checkout -b 5.2.0 v5.2.0
$ make build && cd build
$  ../configure --target-list=riscv64-softmmu,riscv32-softmmu --prefix=/opt/qemu
$ sudo make install

* Test des application RTEMS (POSIX)

Nouveau fichier set_env_riscv32.sh

#!/bin/sh

BASE=/media/pierre/SANDISK_PF

export PATH=$PATH:$BASE/RTEMS/rtems-riscv/bin
export RTEMS_MAKEFILE_PATH=$BASE/RTEMS/target_rv32imafdc/riscv-rtems5/rv32imafdc

$ . ./set_env_riscv32.sh

$ cd exemples/POSIX/timer
$ make clean && make

$ export PATH=/opt/qemu/bin:$PATH
$ qemu-system-riscv32 -machine virt -m 2G -serial stdio -monitor none -bios o-optimize/timer.exe -nographic
Got signal 14 (1)
Got signal 14 (2)
Got signal 14 (3)

-> l'option "-monitor none" permet de fermer QEMU avec Ctrl-C
-> l'option -m 2G n'est pas obligatoire

-> Même test avec helloworld OK

-> ne fonctionne pas avec rv64imafdc (pb de compilateur ??)

23/4/2021
=========

- Test BSP rv32imacfdc -> pb avec test pl06

création d'un fichier rv32imafdc-testsuite.tcfg pour l'exclure mais ne change rien

https://www.mail-archive.com/users@rtems.org/msg02746.html

- Test avec QEMU/x86

https://docs.rtems.org/branches/master/user/tools/tester.html#

* Installation outils rtems-test (Git -> branche 5)

$ ./waf configure --prefix=/media/pierre/SANDISK_PF/RTEMS/5
$ ./waf 
$ ./waf install
$ ./waf test

* Test psx01.exe en mode trace !

-> le nom du BSP est différent (pc-qemu) !

$ rtems-test --list-bsps | grep pc
  pc-qemu
  pc

$ cd build
$ rtems-test --trace --rtems-bsp=pc-qemu  --rtems-tools=/media/pierre/SANDISK_PF/RTEMS/5 i386-rtems5/c/pc386/testsuites/psxtests/psx02.exe
...
[1/1] p:0 f:0 u:0 e:0 I:0 B:0 t:0 i:0 W:0 | i386/pc686: psx01.exe
exe: spawn: qemu-system-i386 -no-reboot -nographic -monitor none -serial stdio -append --console=/dev/com1 -kernel i386-rtems5/c/pc386/testsuites/psxtests/psx01.exe
[1/1] p:0 f:0 u:0 e:0 I:0 B:0 t:0 i:0 W:0 | i386/pc686: psx01.exe

Passed:        1
Failed:        0
User Input:    0
Expected Fail: 0
Indeterminate: 0
Benchmark:     0
Timeout:       0
Invalid:       0
Wrong Version: 0
Wrong Build:   0
Wrong Tools:   0
----------------
Total:         1

Average test time: 0:00:02.504795
Testing time     : 0:00:02.504795


-> à refaire avec BSP RISC-V !

25/4/2021
=========

- Test Pi 1 B+ avec version 4.1 KO

26/4/2021
=========

- Nouveau test avec version master (6)

-> Compilateur

$ cd rtems
$ ../source-builder/sb-set-builder --log=l-i386-master.txt --prefix=/media/pierre/SANDISK_PF/RTEMS/rtems-i386-master 6/rtems-i386

-> BSP

OK en configurant avec 'waf' (rtems-bootstrap / autotools ne fonctionne pas), voir https://docs.rtems.org/branches/master/user/bld/#migration-from-autoconf-automake


$ export PATH=<path>/rtems-i386-master/bin:$PATH

$ ./waf configure --prefix=<bsp-install-dir>
$ ./waf
$ ./waf install

Le config.ini contient :

[i386/pc386]
RTEMS_POSIX_API=True

-> test timer.exe OK avec :

export PATH=$PATH:$BASE/RTEMS/rtems-i386-master/bin
export RTEMS_MAKEFILE_PATH=$BASE/RTEMS/target_i386_master/i386-rtems6/pc386

Même test pour Pi 1 -> toujours rien !

- Test 4.12 (perso/RTEMS)

OK avec un ancien firmware Pi 1 B+ (22d8c910f4c53118f9cf85c038d7d8e307efc110 de 2015)

commit 22d8c910f4c53118f9cf85c038d7d8e307efc110 (HEAD)
Author: popcornmix <popcornmix@gmail.com>
Date:   Tue Dec 15 16:29:19 2015 +0000

    kernel: Bump to 4.1.15

KO avec un firmware récent (Yocto / Dunfell)

-> Passage de 4.12 à 5 dans le dépôt RTEMS

commit 003b19282ce09363c9f0b8730e1514c0b5f22374
Author: Joel Sherrill <joel@rtems.org>
Date:   Mon May 14 15:11:15 2018 -0500

    rtems-bsps: Change 4.12 to 5

le test Pi sur RTEMS 5 (qui était OK)

commit ab77c85fe0d973595fb35cfde1abd3f9e3c5450b
Author: Pierre FICHEUX <pierre.ficheux@smile.fr>
Date:   Sat Oct 20 12:47:19 2018 +0200

    Test RPi

-> tester la version 5 (003b19282ce09363c9f0b8730e1514c0b5f22374)

27/4/2021
=========

- Test de 5.0.0-m1911, aucun résultat y compris sur i386 (erreurs de compilation sur __RTEMS_VERSION__

28/4/2021
=========

- Test sur BBB, tout fonctionne sur 5 et 6 (master) :-)

$ arm-rtems6-objcopy timer.exe -O binary timer.bin
$ mkimage -A arm -O linux -T kernel -C none -a 0x80000000 -e 0x80000000 -n RTEMS -d timer.bin rtems.img

=> setenv rtems 'fatload mmc 0 0x80800000 rtems.img ; fatload mmc 0 0x88000000 am335x-boneblack.dtb ; bootm 0x80800000 - 0x88000000'

29/4/2021
=========

- Nouveau test RISC-V 64

$ ./waf configure --prefix=/media/pierre/SANDISK_PF/RTEMS/target_rv64imafd_medany ./waf 
$ ./waf install

$ export PATH=$PATH:/opt/qemu/bin/
$ rtems-test --trace --rtems-bsp=rv64imafd_medany riscv/rv64imafd_medany/testsuites/psxtests/psx01.exe 
...
exe: spawn: qemu-system-riscv64 -no-reboot -nographic -net none -machine virt -m 64M -kernel riscv/rv64imafd_medany/testsuites/psxtests/psx01.exe
[1/1] p:0 f:0 u:0 e:0 I:0 B:0 t:0 L:0 i:0 W:0 | riscv64/rv64imafd_medany: psx01.exe
Result: invalid    Time: 0:00:00.251285 psx01.exe
=>  exe: qemu-system-riscv64 -no-reboot -nographic -net none -machine virt -m 64M -kernel riscv/rv64imafd_medany/testsuites/psxtests/psx01.exe
] rom: requested regions overlap (rom phdr #0: riscv/rv64imafd_medany/testsuites/psxtests/psx01.exe. free=0x0000000080011488, addr=0x0000000080000000)
] qemu-system-riscv64: rom check and register reset failed

Passed:        0
Failed:        0
User Input:    0
Expected Fail: 0
Indeterminate: 0
Benchmark:     0
Timeout:       0
Test too long: 0
Invalid:       1
Wrong Version: 0
Wrong Build:   0
Wrong Tools:   0
----------------
Total:         1
Invalid:
 psx01.exe
Average test time: 0:00:00.752206
Testing time     : 0:00:00.752206

-> Idem avec l'exemple timer :

$ qemu-system-riscv64  -no-reboot -nographic -net none -machine virt -m 64M -kernel o-optimize/timer.exe 
rom: requested regions overlap (rom phdr #0: o-optimize/timer.exe. free=0x0000000080011488, addr=0x0000000080000000)
qemu-system-riscv64: rom check and register reset failed

30/4/2021
=========

- Pour Pi3 tester le commit  19efa9a0b9692ffdc1d46bc81284e415ec50e60e, voir https://lists.rtems.org/pipermail/devel/2020-February/057403.html

I was successfully able to boot through tftp using these commands
but still I had to manually load the DTB
my uboot commands were
1) tftp 0x200000 rtems.img
2) fatload mmc 0 0x1000 bcm2710-rpi-3-b.dtb
3) bootm 0x200000 - 0x1000
this works for me on pi3

$ cd rtems_pi3
$ ./bootstrap
$ mkdir build && cd build
$ <path>/rtems_pi3/configure --target=arm-rtems5 --disable-cxx --enable-rtemsbsp=raspberrypi2 --prefix=<path>/target_rpi3

Création .img

$ mkimage -A arm -O linux -T kernel -a 0x200000 -e 0x200080 -C none -d timer.ralf rtems.img


Traces sur la console :
                                                                                
Starting kernel ...                                                             
      Z5����{N                                                               
���`h

Option --console=/dev/ttyS0 nécessaire mais ou la mettre ?

- Utilisation de config.txt (voir https://github.com/gs-niteesh/rpi3_RTEMS) avec

core_freq=250 <-- indispensable
enable-uart=1
kernel=u-boot.bin <-- celui de Yocto/dunfell (32 bits)

- Modif de console-config.c pour utilise mini-UART par défaut

-> OK :-)

U-Boot> pri rtems                                                               
rtems=fatload mmc 0 0x200000 rtems.img; fatload mmc 0 0x1000 bcm2710-rpi-3-b.dtb
; bootm 0x200000 - 0x1000                                                       
U-Boot> run rtems

Starting kernel ...                                                             
...                                                                                
                                                                                
RTEMS RPi 3B 1.2 (1GB) [00a02082]                                               
Got signal 14 (1)                                                               
Got signal 14 (2)                                                               
Got signal 14 (3)   
...



U-Boot> pri rtems                                                               
rtems=fatload mmc 0 0x200000 rtems.img; fatload mmc 0 0x1000 bcm2710-rpi-3-b.dtb
; bootm 0x200000 - 0x1000                                                       
U-Boot> run rtems

Starting kernel ...                                                             
...                                                                                
                                                                                
RTEMS RPi 3B 1.2 (1GB) [00a02082]                                               
Got signal 14 (1)                                                               
Got signal 14 (2)                                                               
Got signal 14 (3)   
...

-> devrait fonctionner sans U-Boot


1/5/2021
========

- Modif example -> utiliser #define CONFIGURE_APPLICATION_NEEDS_SIMPLE_CONSOLE_DRIVER !!

- Modif example -> utiliser #define CONFIGURE_APPLICATION_NEEDS_SIMPLE_CONSOLE_DRIVER !!

Si on définit CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER on inclut

#ifdef CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
  #if defined(CONFIGURE_APPLICATION_NEEDS_SIMPLE_CONSOLE_DRIVER) \
    || defined(CONFIGURE_APPLICATION_NEEDS_SIMPLE_TASK_CONSOLE_DRIVER)
    #error "CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER, CONFIGURE_APPLICATION_NEEDS_SIMPLE_CONSOLE_DRIVER, and CONFIGURE_APPLICATION_NEEDS_SIMPLE_TASK_CONSOLE_DRIVER are mutually exclusive"
  #endif

  #include <rtems/console.h>
#endif


du fichier cpukit/include/rtems/confdefs/iodrivers.h

Dans rtems/console.h on ajoute donc :

#define CONSOLE_DRIVER_TABLE_ENTRY \
  { console_initialize, console_open, console_close, \
    console_read, console_write, console_control }

-> appel de console_initialize() qui définit PL011 par défaut (mais ça n'explique pas l'utilisation de --console=/dev/ttyS0

- Test GPIO BBB d'après https://jamesfitzsimons.com/2020/06/29/rtems-on-beaglebone-black-wireless/

14/5/2021
=========

- Test RED-V (RISC-V)

Proche de https://github.com/pragnesh26992/rtems/blob/master/README-frdme310arty.md mais utilise un bootloader pour charger le DT...

AFAIK Zephyr or FreedomStudio use a different approach as a header file is generated from the .dts and then
used to create the binary (.bin then .hex). It looks like the binary is  "standalone" with no use of a bootloader (RED-V includes a Segger firmware to flash the
board)...but I'm far from a RISC-V/SiFive expert :-)

https://docs.zephyrproject.org/latest/guides/dts/intro.html#input-and-output-files

-> paraît plutôt complexe à réaliser

17/5/2021
=========

- Test exemple led sur BBB 'industrial' OK (normal)

- Test timer 1ms sur Pi 3 -> un peu de glitch à l'oscillo

19/5/2021
=========

- Test timer 1ms sur BBB -> un peu de jitter

- Test pthread_create OK

- Test pthread_cond_timedwait -> ajout du lock/unlock du mutex pour que ça fonctionne

- Test QEMU/x86 sans graphique (mieux !) :

$ qemu-system-i386 -kernel <file.exe> -nographic -serial stdio -append --console=/dev/com1 -monitor none

- Ajout macro à beagleboneblack.cfg pour créer rtems.img

# Create binary and U-Boot image
define bsp-post-link
    $(OBJCOPY) -O binary --strip-all \
        $(basename $@)$(EXEEXT) $(basename $@)$(DOWNEXT)
    $(SIZE) $(basename $@)$(EXEEXT)
    mkimage -A arm -O linux -T kernel -C none -a 0x80000000 -e 0x80000000 -n RTEMS -d $(basename $@)$(DOWNEXT) rtems.img
endef

- Test TFTP BBB (ne fonctionne pas avec le Wi-Fi !)

$ scp rtems.img pierre@192.168.1.20:/tftpboot

nouvelle macros dans U-Boot :

=> pri rtems_sd                                                                 
rtems_sd=fatload mmc 0 0x80800000 rtems.img ; fatload mmc 0 0x88000000 am335x-bo
neblack.dtb ; bootm 0x80800000 - 0x88000000                                     

=> pri rtems_tftp                                  
rtems_tftp=tftp 0x80800000 $serverip:rtems.img ; fatload mmc 0 0x88000000 am335x
-boneblack.dtb ; bootm 0x80800000 - 0x88000000

- Ajout exemple driver 'dummy_drv' basé sur rpi_gpio

- Conclusion tests GPIO (BBB)

* Test GPIO avec API Classic -> OK mais du jitter à 1 ms (RMS)

* Exemple pthread (basé sur clock_nanosleep()) limité à 10 ms (et quelques décrochages dans la période)

* Exemple GPIO (sans thread) OK mais on ne peut pas utiliser sched_setscheduler(), car pas implémenté -> jitter

* Exemple 'condition' à corriger, retourne toujours une erreur pthread_cond_timedwait (voir https://stackoverflow.com/questions/17166083/how-to-use-pthread-cond-timedwait-with-millisecond) -> ajout tsnorm() mais irrégularité dans la période < 100 ms

-> Test sur Pi 3, moins de jitter sur gpio (out) et rtems_square (CLASSIC) mais même problèmes
-> Ajout priorité = 98 (Deterministic Priority Scheduler = sched par défaut) à gpio.c

  sc = rtems_task_set_priority(RTEMS_SELF, 90 , &the_priority); 
  sc = rtems_task_set_priority(RTEMS_SELF, RTEMS_CURRENT_PRIORITY, &the_priority); 
  printf("main-- initial current priority : %d\n", (int)the_priority); 


20/5/2021
=========

- Test l'envoi de signal à un thread ? https://devarea.com/linux-handling-signals-in-a-multithreaded-application/#.YKYBK6IzY5k

28/5/2021
=========

- Test SMP sur Pi3 + smp01 -> KO


4/6/2021
========

- Test I2C sur Pi 3 -> KO


22/11/2023
==========

- Test rtems-5.3

* i386 OK
* x86_64 (amd64 KO)

Test avec KVM :

$ kvm -kernel o-optimize/timer.exe -name RTEMS



8/11/2025
=========

Test RTEMS 6 pour ECE (?)

$ export RTEMS_BASE=/media/pierre/SAMSUNG_1TO_2/RTEMS

1- Construction RSB (compilateur)

$ git clone https://gitlab.rtems.org/rtems/tools/rtems-source-builder.git
$ cd rtems-source-builder/
$ git checkout 6
$ ./source-builder/sb-check 
$ cd rtems/
$ ../source-builder/sb-set-builder --log=l-i386.txt --prefix=$RTEMS_PATH/rtems-i386 6/rtems-i386

2- Construction BSP QEMU/x86

$ cd $RTEMS_BASE
$ git clone https://gitlab.rtems.org/rtems/rtos/rtems.git rtems_git
$ cd rtems_git/
$ git checkout 6  # RTEMS 6

Configuration, voir https://docs.rtems.org/docs/main/user/bld/

$ cat >> config.ini
[i386/pc386]
RTEMS_POSIX_API=True
^D

-> voir description de la variable dans https://docs.rtems.org/docs/6.1/posix-users.pdf

Ajouter le compilateur au path !!

$ export PATH=$PATH:$RTEMS_BASE/rtems-i386/bin

$ ./waf configure --prefix=$RTEMS_BASE/rtems/6
$ ./waf 
$ ./waf install

3- Test d'application (POSIX)

$ export RTEMS_MAKEFILE_PATH=$RTEMS_BASE/rtems/6/i386-rtems6/pc386
$ cd <path>/exemples/POSIX/helloworld
$ make

$ qemu-system-i386 -kernel o-optimize/helloworld.exe

Idem pour timer !

-> Création set_env_i386.sh (à "sourcer")

#!/bin/sh

export PATH=$PATH:$RTEMS_BASE/rtems-i386/bin 
export RTEMS_MAKEFILE_PATH=$RTEMS_BASE/rtems/6/i386-rtems6/pc386
1

TODO
====

- PWM sur Pi et/ou BBB voir

https://gist.github.com/ketul93/d717555951174a74c8b4
https://jamesfitzsimons.com/2020/06/29/rtems-on-beaglebone-black-wireless/

- RTEMS / libbsd à tester ? https://github.com/RTEMS/rtems-libbsd

