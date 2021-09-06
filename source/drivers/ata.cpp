#include <drivers/ata.hpp>
#include <drivers/system.hpp>
#include <drivers/pci.hpp>

#pragma region defs
#define ATA_SR_BSY        0x80
#define ATA_SR_DRDY        0x40
#define ATA_SR_DF        0x20
#define ATA_SR_DSC        0x10
#define ATA_SR_DRQ        0x08
#define ATA_SR_CORR        0x04
#define ATA_SR_IDX        0x02
#define ATA_SR_ERR        0x01

#define ATA_CMD_READ_PIO        0x20
#define ATA_CMD_READ_PIO_EXT    0x24
#define ATA_CMD_READ_DMA        0xC8
#define ATA_CMD_READ_DMA_EXT    0x25
#define ATA_CMD_WRITE_PIO        0x30
#define ATA_CMD_WRITE_PIO_EXT    0x34
#define ATA_CMD_WRITE_DMA        0xCA
#define ATA_CMD_WRITE_DMA_EXT    0x35
#define ATA_CMD_CACHE_FLUSH        0xE7
#define ATA_CMD_CACHE_FLUSH_EXT    0xEA
#define ATA_CMD_PACKET            0xA0
#define ATA_CMD_IDENTIFY_PACKET    0xA1
#define ATA_CMD_IDENTIFY        0xEC

#define ATA_REG_DATA        0x00
#define ATA_REG_ERROR        0x01
#define ATA_REG_FEATURES    0x01
#define ATA_REG_SECCOUNT0    0x02
#define ATA_REG_LBA0        0x03
#define ATA_REG_LBA1        0x04
#define ATA_REG_LBA2        0x05
#define ATA_REG_HDDEVSEL    0x06
#define ATA_REG_COMMAND        0x07
#define ATA_REG_STATUS        0x07
#define ATA_REG_SECCOUNT1    0x08
#define ATA_REG_LBA3        0x09
#define ATA_REG_LBA4        0x0A
#define ATA_REG_LBA5        0x0B
#define ATA_REG_CONTROL        0x0C
#define ATA_REG_ALTSTATUS    0x0C
#define ATA_REG_DEVADDRESS    0x0D
#pragma endregion

ataDevice ataDevices[4]{
    { 0x1F0, 0x3F6, 0 },
    { 0x1F0, 0x3F6, 1 },
    { 0x170, 0x376, 0 },
    { 0x170, 0x376, 1 }
};

ataDevice *getDevice(int index){
    return index < 4 ? &ataDevices[index] : (ataDevice*)NULL;
}

static void ataIOWait(ataDevice *device){
    for(int i = 0; i < 4; ++i){
        inb(device->base + ATA_REG_ALTSTATUS);
    }
}

static void ataDeviceDetect(ataDevice *device){
    outb(device->control, 1 << 1);
    ataIOWait(device);
    outb(device->base + ATA_REG_HDDEVSEL, 0xA0 | device->slave << 4);
    ataIOWait(device);
    outb(device->base + ATA_REG_COMMAND, ATA_CMD_IDENTIFY);
    ataIOWait(device);

    if(!inb(device->base + ATA_REG_STATUS)) return;

    while(inb(device->base + ATA_REG_STATUS) && ATA_SR_BSY);

    uint16_t c = (inb(device->base + ATA_REG_LBA2) | inb(device->base + ATA_REG_LBA1));
    if(c == 0xFFFF) return;
    if(c == 0xEB14 || c == 0x9669){
        device->isAtapi = 1;
        outb(device->base + ATA_REG_COMMAND, ATA_CMD_IDENTIFY_PACKET);
        ataIOWait(device);
    }

    outb(device->base + ATA_REG_COMMAND, ATA_CMD_IDENTIFY);
    ataIOWait(device);

    insl(device->base + ATA_REG_DATA, (uint64_t)&device->identity, 128);
    for (int i = 0; i < 40; i += 2) {
        char c = device->identity.model[i];
        device->identity.model[i] = device->identity.model[i + 1];
        device->identity.model[i + 1] = c;
    }
    char* k = &device->identity.model[39];
    while (*k == ' ')
        *k-- = '\0';
}

void initATA(){
    uint32_t device = pciScan(0x0101);
    if(device == (uint32_t) -1)return;
    for (int i = 0; i < 4; i++)
        ataDeviceDetect(&ataDevices[i]);
}

static void ataPioOutLBA(ataDevice *device, uint64_t lba, uint16_t count) {
    outb(device->control, (1 << 7) | (1 << 1));
    outb(device->base + ATA_REG_SECCOUNT1 - 6, 0);
    outb(device->base + ATA_REG_LBA3 - 6, (lba >> 24) & 0xFF);
    outb(device->base + ATA_REG_LBA4 - 6, (lba >> 32) & 0xFF);
    outb(device->base + ATA_REG_LBA5 - 6, (lba >> 40) & 0xFF);
    outb(device->control, (1 << 1));
    outb(device->base + ATA_REG_SECCOUNT0, count & 0xFF);
    outb(device->base + ATA_REG_LBA0, (lba >>  0) & 0xFF);
    outb(device->base + ATA_REG_LBA1, (lba >>  8) & 0xFF);
    outb(device->base + ATA_REG_LBA2, (lba >> 16) & 0xFF);
}

void ataReadsector(ataDevice *device, uint64_t lba, void* buffer){
    if (!(device->identity.capabilities & 0x200)) return;

    while (inb(device->base + ATA_REG_STATUS) & ATA_SR_BSY);

    outb(device->base + ATA_REG_HDDEVSEL, 0xE0 | (device->slave << 4));
    ataPioOutLBA(device, lba, 1);
    outb(device->base + ATA_REG_COMMAND, ATA_CMD_READ_PIO_EXT);
    ataIOWait(device);

    while (inb(device->base + ATA_REG_STATUS) & ATA_SR_BSY);

    insl(device->base + ATA_REG_DATA, (uint64_t) buffer, 128);
}

void ataWriteSector(ataDevice *device, uint64_t lba, const void* buffer){
    if (!(device->identity.capabilities & 0x200)) return;

    while (inb(device->base + ATA_REG_STATUS) & ATA_SR_BSY);

    outb(device->base + ATA_REG_HDDEVSEL, 0xE0 | (device->slave << 4));
    ataPioOutLBA(device, lba, 1);
    outb(device->base + ATA_REG_COMMAND, ATA_CMD_WRITE_PIO_EXT);
    ataIOWait(device);
    
    while (inb(device->base + ATA_REG_STATUS) & ATA_SR_BSY);

    outsl(device->base + ATA_REG_DATA, (uint64_t) buffer, 128);
    outb(device->base + ATA_REG_COMMAND, ATA_CMD_CACHE_FLUSH_EXT);
}
