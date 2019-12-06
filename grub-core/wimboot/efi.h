#ifndef _EFI_H
#define _EFI_H

/*
 * Copyright (C) 2014 Michael Brown <mbrown@fensystems.co.uk>.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

/**
 * @file
 *
 * EFI definitions
 *
 */

#include <grub/efi/api.h>
#include <grub/efi/efi.h>
#include <grub/mm.h>

#if   defined (__i386__)
// \\EFI\\BOOT\\BOOTIA32.EFI
#define EFI_BOOT_FILE_NAME \
{ \
    0x5c, 0x45, 0x46, 0x49, \
    0x5c, 0x42, 0x4f, 0x4f, 0x54,\
    0x5c, 0x42, 0x4f, 0x4f, 0x54, 0x49, 0x41, 0x33, 0x32, 0x2e, 0x45, 0x46, 0x49, \
    0x00 \
}
#define EFI_BOOT_FILE_NAME_LEN 23
#elif defined (__x86_64__)
// \\EFI\\BOOT\\BOOTX64.EFI
#define EFI_BOOT_FILE_NAME \
{ \
    0x5c, 0x45, 0x46, 0x49, \
    0x5c, 0x42, 0x4f, 0x4f, 0x54,\
    0x5c, 0x42, 0x4f, 0x4f, 0x54, 0x58, 0x36, 0x34, 0x2e, 0x45, 0x46, 0x49, \
    0x00 \
}
#define EFI_BOOT_FILE_NAME_LEN 22
#elif defined(__ia64__)
// \\EFI\\BOOT\\BOOTIA64.EFI
#define EFI_BOOT_FILE_NAME \
{ \
    0x5c, 0x45, 0x46, 0x49, \
    0x5c, 0x42, 0x4f, 0x4f, 0x54,\
    0x5c, 0x42, 0x4f, 0x4f, 0x54, 0x49, 0x41, 0x36, 0x34, 0x2e, 0x45, 0x46, 0x49, \
    0x00 \
}
#define EFI_BOOT_FILE_NAME_LEN 23
#elif defined(__aarch64__)
// \\EFI\\BOOT\\BOOTAA64.EFI
#define EFI_BOOT_FILE_NAME \
{ \
    0x5c, 0x45, 0x46, 0x49, \
    0x5c, 0x42, 0x4f, 0x4f, 0x54,\
    0x5c, 0x42, 0x4f, 0x4f, 0x54, 0x41, 0x41, 0x36, 0x34, 0x2e, 0x45, 0x46, 0x49, \
    0x00 \
}
#define EFI_BOOT_FILE_NAME_LEN 23
#elif defined(__arm__)
// \\EFI\\BOOT\\BOOTARM.EFI
#define EFI_BOOT_FILE_NAME \
{ \
    0x5c, 0x45, 0x46, 0x49, \
    0x5c, 0x42, 0x4f, 0x4f, 0x54,\
    0x5c, 0x42, 0x4f, 0x4f, 0x54, 0x41, 0x52, 0x4d, 0x2e, 0x45, 0x46, 0x49, \
    0x00 \
}
#define EFI_BOOT_FILE_NAME_LEN 22
#else
  #error this architecture is not supported by grub2
#endif

#define MBR_TYPE_PCAT             0x01
#define MBR_TYPE_EFI_PARTITION_TABLE_HEADER 0x02

#define NO_DISK_SIGNATURE         0x00
#define SIGNATURE_TYPE_MBR        0x01
#define SIGNATURE_TYPE_GUID       0x02

#define HW_VENDOR_DP              0x04
#define MSG_ATAPI_DP              0x01
#define MEDIA_HARDDRIVE_DP        0x01
#define MEDIA_FILEPATH_DP         0x04

#define MEDIA_DEVICE_PATH         0x04
#define MESSAGING_DEVICE_PATH     0x03
#define HARDWARE_DEVICE_PATH      0x01

#define TRUE  ((grub_efi_boolean_t)(1==1))

#define FALSE ((grub_efi_boolean_t)(0==1))

extern grub_efi_guid_t efi_block_io_protocol_guid;
extern grub_efi_guid_t efi_device_path_protocol_guid;
extern grub_efi_guid_t efi_graphics_output_protocol_guid;
extern grub_efi_guid_t efi_loaded_image_protocol_guid;

#endif /* _EFI_H */
