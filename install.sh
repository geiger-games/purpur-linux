#!/bin/bash
read -p "enter device which termOS is in (ex. /dev/nvme0n1p3): " PART

UUID=$(lsblk -no UUID "$PART")

sudo tee /etc/grub.d/42_termOS > /dev/null <<EOF
#!/bin/sh
exec tail -n +3 \$0

menuentry "termOS" {
  search --fs-uuid --set=root $UUID
  linux /kernel root=UUID=$UUID rw init=/purpur-linux-PID1 quiet
  initrd /initrd
}
EOF

sudo chmod 755 /etc/grub.d/42_termOS
sudo update-grub

bash compile.sh

echo "termOS compiled & installed"
