rm image.iso
xorriso -as mkisofs -b limine-cd.bin         -no-emul-boot -boot-load-size 4 -boot-info-table         --efi-boot limine-eltorito-efi.bin         -efi-boot-part --efi-boot-image --protective-msdos-label         ../boot/ -o image.iso
../../../limine/limine/limine-install image.iso