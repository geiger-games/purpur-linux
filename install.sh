#!/bin/bash
# termOS grub installer, auto-resolves termOS path

# get the directory of this script
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# termOS binary path
TERMOS_PATH="${SCRIPT_DIR}/termOS"

if [[ ! -f "$TERMOS_PATH" ]]; then
    echo "termOS binary not found at $TERMOS_PATH"
    exit 1
fi

# detect root partition
ROOT_PART=$(df / | tail -1 | awk '{print $1}')

# detect current kernel and initrd symlinks
KERNEL_PATH=$(readlink -f /boot/vmlinuz)
INITRD_PATH=$(readlink -f /boot/initrd.img)

if [[ ! -f "$KERNEL_PATH" || ! -f "$INITRD_PATH" ]]; then
    echo "Kernel or initrd not found in /boot"
    exit 1
fi

# grub menuentry template
ENTRY="menuentry \"termOS\" {
    linux ${KERNEL_PATH} root=${ROOT_PART} rw init=${TERMOS_PATH}
    initrd ${INITRD_PATH}
}"

# write to /etc/grub.d/42_termOS
echo "$ENTRY" | sudo tee /etc/grub.d/42_termOS > /dev/null
sudo chmod +x /etc/grub.d/42_termOS

# regenerate grub.cfg
sudo update-grub

echo "termOS menu entry installed! Reboot and select 'termOS' in grub."
