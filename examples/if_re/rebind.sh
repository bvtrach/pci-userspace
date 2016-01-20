#!/bin/bash

modprobe uio_pci_generic

echo '0000:02:00.0' > /sys/bus/pci/drivers/r8169/unbind
echo '10ec 8168' > /sys/bus/pci/drivers/uio_pci_generic/new_id
echo '0000:02:00.0' > /sys/bus/pci/drivers/uio_pci_generic/bind
