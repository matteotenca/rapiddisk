#!/bin/bash

cd ..
kernel="$(uname -r)"
hooks_dir="/usr/share/initramfs-tools/hooks"
[ -x "$hooks_dir/rapiddisk_hook" ] && installed=yes
echo -e "\nInstall test...\n"
sudo ./rapiddisk-on-boot --install --root=/dev/testZZZ --size=10 --kernel="$kernel" --cache-mode=wt --no-initramfs
#sudo ./rapiddisk-on-boot --install --root=/dev/testZZZ --size=10 --kernel="$kernel" --cache-mode=wa --no-initramfs
#sudo ./rapiddisk-on-boot --install --root=/dev/testZZZ --size=10 --kernel="$kernel" --cache-mode=wb --no-initramfs
sleep 1
echo -e "\nList test...\n"
list="$(sudo ./rapiddisk-on-boot --list --kernel="$kernel")"
mappings="$(echo "$list" | grep testZZZ | grep -oP '\d{3}')"
echo "$list"
sleep 1
echo -e "\nUninstalling test...\n"
for m in $mappings ; do
	echo -e "$m\nq\n" | sudo ./rapiddisk-on-boot --uninstall --kernel="$kernel" --no-initramfs
done
sleep 1
echo -e "\nList test...\n"
sudo ./rapiddisk-on-boot --list --kernel="$kernel"
sleep 1
echo -e "\nList all test...\n"
sudo ./rapiddisk-on-boot --list
if [ -z "$installed" ] ; then
	echo sudo ./rapiddisk-on-boot --global-uninstall --no-initramfs
fi
