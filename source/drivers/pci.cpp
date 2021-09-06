#include <drivers/pci.hpp>
#include <drivers/system.hpp>

#define PCI_CONFIG_ADDRESS	0xCF8
#define PCI_CONFIG_DATA		0xCFC

#define PCI_BUS(d) ((uint8_t) (d >> 16))
#define PCI_SLOT(d) ((uint8_t) (d >> 8))
#define PCI_FUNCTION(d) ((uint8_t) (d))
#define PCI_DEVICE(b, s, f) ((uint32_t) ((b << 16) | (s << 8) | f))

uint32_t pciRead(uint32_t device, uint8_t field, uint8_t size){
    uint32_t addr = 0x80000000;

    addr |= PCI_BUS(device) << 16;
	addr |= PCI_SLOT(device) << 11;
	addr |= PCI_FUNCTION(device) << 8;
	addr |= field & 0xFC;

    outl(PCI_CONFIG_ADDRESS, addr);
	if (size == 1) return inb(PCI_CONFIG_DATA + (field & 3));
	if (size == 2) return inw(PCI_CONFIG_DATA + (field & 2));
	if (size == 4) return inl(PCI_CONFIG_DATA);
	return null;
}

uint32_t pciScanBus(uint16_t type, uint8_t bus);

uint32_t pciScanFunction(uint16_t type, uint8_t bus, uint8_t slot, uint8_t function) {
	uint32_t device = PCI_DEVICE(bus, slot, function);
	uint16_t device_type = pciRead(device, PCI_CLASS, 1) << 8 | pciRead(device, PCI_SUBCLASS, 1);
	if (device_type == type)
		return device;
	if (device_type == 0x0604)
		return pciScanBus(type, pciRead(device, PCI_SECONDARY_BUS, 1));
	return -1;
}

uint32_t pciScanSlot(uint16_t type, uint8_t bus, uint8_t slot) {
	uint32_t device = PCI_DEVICE(bus, slot, 0);
	if (pciRead(device, PCI_VENDOR_ID, 2) == 0xFFFF)
		return -1;
	if (pciScanFunction(type, bus, slot, 0) != (uint32_t) -1)
		return device;
	if (!pciRead(device, PCI_HEADER_TYPE, 1))
		return -1;
	for (uint8_t function = 1; function < 8; function++) {
		uint32_t device = PCI_DEVICE(bus, slot, function);
		if (pciRead(device, PCI_VENDOR_ID, 2) != 0xFFFF)
			if (pciScanFunction(type, bus, slot, function) != (uint32_t) -1)
				return device;
	}
	return -1;
}

uint32_t pciScanBus(uint16_t type, uint8_t bus) {
	for (uint8_t slot = 0; slot < 32; slot++) {
		uint32_t out = pciScanSlot(type, bus, slot);
		if (out != (uint32_t) -1)
			return out;
	}
	return -1;
}

uint32_t pciScan(uint16_t type){
    if (!(pciRead(0, PCI_HEADER_TYPE, 1) & 0x80))
		return pciScanBus(type, 0);
	for (uint8_t function = 0; function < 8; function++) {
		uint32_t device = PCI_DEVICE(0, 0, function);
		if (pciRead(device, PCI_VENDOR_ID, 2) == 0xFFFF)
			break;
		uint32_t out = pciScanBus(type, function);
		if (out != (uint32_t) -1)
			return out;
	}
	return -1;
}