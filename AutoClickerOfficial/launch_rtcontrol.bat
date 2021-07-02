plink test@10.0.20.57 -pw Kinova123! -no-antispoof pkill -9 rtcontrol
timeout /t 8 /nobreak
plink test@10.0.20.57 -pw Kinova123! -no-antispoof /usr/share/RTLinux/3.1.4/rtcontrol --ifacename=enp3s0f1   --actuator-count=7
timeout /t 10 /nobreak
exit