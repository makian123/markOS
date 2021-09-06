#pragma once

#include <include/types.hpp>

#define PCI_VENDOR_ID		0x00
#define PCI_HEADER_TYPE		0x0E
#define PCI_SUBCLASS		0x0A
#define PCI_CLASS			0x0B
#define PCI_BAR0			0x10
#define PCI_SECONDARY_BUS	0x19

uint32_t pciRead(uint32_t device, uint8_t field, uint8_t size);
uint32_t pciScan(uint16_t type);