#ifndef WIFISERVER_H

#define WIFISERVER_H

#include <avr/pgmspace.h>
#include "wifiCore.h"
#include "../config/config.h"

#define wifiServerSendPGMP(str, stream) fputs_P(str, stream);

const PROGMEM char http200[] = "HTTP/1.1 200 OK\r\nConnection: close\r\n\r\n";
const PROGMEM char http200gzip[] = "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Encoding: gzip\r\n\r\n";
const PROGMEM char http200json[] = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n"
                                   "Connection: close\r\n\r\n";
const PROGMEM char http404[] = "HTTP/1.1 404 Not Found\r\nConnection: close\r\n\r\nHTTP 404 Not Found";
const PROGMEM char http400[] = "HTTP/1.1 400 Bad Request\r\nConnection: close\r\n\r\nHTTP 400 Bad Request";
const PROGMEM char success[] = "{\"result\":\"success\"}";
const PROGMEM unsigned char index[] = {
  0x1f, 0x8b, 0x08, 0x08, 0x58, 0x8d, 0xa4, 0x57, 0x02, 0x03, 0x69, 0x6e,
  0x64, 0x65, 0x78, 0x33, 0x2e, 0x68, 0x74, 0x6d, 0x6c, 0x00, 0xd5, 0x1a,
  0x6b, 0x6f, 0xdb, 0x46, 0xf2, 0xaf, 0x30, 0x0c, 0x12, 0x58, 0x17, 0x52,
  0x26, 0xf5, 0xb2, 0x4d, 0x8a, 0x6a, 0x7b, 0x75, 0x70, 0x31, 0xae, 0x45,
  0x8d, 0x3a, 0x07, 0xdc, 0xa1, 0xe9, 0x87, 0x15, 0xb9, 0x92, 0x36, 0xa6,
  0x48, 0x1e, 0xb9, 0xb4, 0xec, 0x08, 0xfa, 0xef, 0x37, 0xfb, 0x20, 0xb9,
  0x7c, 0x48, 0x56, 0x5c, 0x05, 0xc8, 0x25, 0xb0, 0x4d, 0xce, 0xcc, 0xce,
  0x6b, 0xe7, 0xb5, 0x2b, 0x4d, 0x5f, 0x5d, 0xff, 0xf6, 0xf3, 0xc7, 0xff,
  0xdc, 0xbe, 0xd7, 0x56, 0x74, 0x1d, 0xce, 0xa6, 0x6b, 0x4c, 0x91, 0xe6,
  0xc7, 0x11, 0xc5, 0x11, 0xf5, 0x74, 0x12, 0x11, 0x4a, 0x50, 0x68, 0x66,
  0x3e, 0x0a, 0xb1, 0x67, 0x1b, 0x79, 0x86, 0x53, 0xfe, 0x82, 0xe6, 0xf0,
  0x1e, 0xc5, 0x7a, 0x84, 0xd6, 0xd8, 0x7b, 0x20, 0x78, 0x93, 0xc4, 0x29,
  0x9d, 0x4d, 0x29, 0xa1, 0x21, 0x9e, 0x5d, 0xe3, 0x07, 0xe2, 0x63, 0xc6,
  0x65, 0x41, 0x96, 0x79, 0x8a, 0x28, 0x89, 0xa3, 0xe9, 0xb9, 0xc0, 0x4d,
  0x33, 0xfa, 0x04, 0x7f, 0x98, 0xb0, 0xed, 0x02, 0xc4, 0x98, 0x0b, 0xb4,
  0x26, 0xe1, 0x93, 0x93, 0xa1, 0x28, 0x33, 0x81, 0x3b, 0x59, 0xb8, 0xe6,
  0x3a, 0x33, 0x29, 0x7e, 0xa4, 0x66, 0x46, 0xbe, 0x60, 0x13, 0x05, 0x9f,
  0xf3, 0x8c, 0x3a, 0xb6, 0x65, 0xbd, 0x71, 0xcd, 0x0d, 0x9e, 0xdf, 0x13,
  0xda, 0x8d, 0xdd, 0xcd, 0xe3, 0xe0, 0x69, 0xbb, 0x46, 0xe9, 0x92, 0x44,
  0x8e, 0xb5, 0x9b, 0x0b, 0xf6, 0x1b, 0x4c, 0x96, 0x2b, 0xea, 0x5c, 0x58,
  0xd6, 0x6e, 0x65, 0x0b, 0x10, 0x5b, 0xe8, 0x0c, 0xf0, 0xda, 0x95, 0xb4,
  0xfd, 0xc9, 0x05, 0x5e, 0x6b, 0xb0, 0x22, 0xa7, 0x34, 0x8e, 0x0c, 0x12,
  0x25, 0x39, 0xdd, 0xfa, 0x71, 0x18, 0xa7, 0x0e, 0x89, 0x56, 0xa0, 0x12,
  0x75, 0xd9, 0xba, 0xf2, 0xa5, 0x12, 0xc1, 0x17, 0x6c, 0xe3, 0x07, 0x9c,
  0x2e, 0xc2, 0x78, 0xe3, 0x3c, 0x90, 0x8c, 0x80, 0x5f, 0x0a, 0x38, 0xd7,
  0x92, 0xa6, 0x60, 0xd8, 0x22, 0x4e, 0xd7, 0x4e, 0x14, 0x47, 0x25, 0xaa,
  0x30, 0x04, 0x25, 0x09, 0x46, 0x40, 0xe1, 0x63, 0x47, 0x60, 0x5c, 0x3f,
  0x4f, 0x33, 0x10, 0x9c, 0xc4, 0x04, 0xb6, 0x20, 0x95, 0xf4, 0x7f, 0x04,
  0x24, 0x63, 0x1e, 0x0f, 0xfe, 0xdc, 0x4a, 0x7c, 0x80, 0x17, 0x28, 0x0f,
  0xa9, 0xc4, 0x3b, 0x8e, 0xb9, 0x8e, 0xbf, 0x98, 0x8b, 0xd8, 0xcf, 0x33,
  0x93, 0x44, 0x11, 0x4e, 0x85, 0x15, 0x6d, 0xf8, 0x76, 0x1e, 0xa7, 0x01,
  0x4e, 0x1d, 0xcb, 0x4d, 0x50, 0x10, 0x90, 0x68, 0x09, 0x66, 0x08, 0x83,
  0x43, 0x12, 0x61, 0x73, 0x25, 0xbc, 0x15, 0x81, 0xbe, 0x28, 0x14, 0x88,
  0x3f, 0xe8, 0x53, 0x82, 0x3d, 0x7f, 0x85, 0xfd, 0xfb, 0x79, 0xfc, 0xf8,
  0x67, 0xa9, 0x3a, 0xbc, 0x30, 0x4f, 0x32, 0x16, 0x82, 0x27, 0x83, 0xb8,
  0x5c, 0x5e, 0x37, 0xaa, 0x1b, 0x5a, 0xa9, 0xf1, 0xb7, 0x13, 0xb3, 0xde,
  0x39, 0x68, 0x01, 0x2e, 0x34, 0x9c, 0x39, 0x06, 0xff, 0xe3, 0x53, 0x73,
  0xaf, 0x22, 0x98, 0xc7, 0x93, 0x6d, 0x25, 0x8f, 0x55, 0x7c, 0xa2, 0xc4,
  0x5c, 0x81, 0x27, 0x43, 0xe6, 0x4d, 0x53, 0x84, 0x12, 0x0f, 0x84, 0x04,
  0xa5, 0x90, 0x57, 0x22, 0x52, 0xd5, 0xe8, 0xff, 0x80, 0xc3, 0x07, 0x4c,
  0x89, 0x8f, 0x8c, 0x9f, 0x52, 0xc8, 0x37, 0x43, 0xc9, 0x06, 0x45, 0xc4,
  0x18, 0x44, 0xa8, 0xdb, 0x64, 0xf7, 0x47, 0x83, 0xcb, 0xf1, 0x85, 0x3d,
  0x1a, 0xba, 0x42, 0xc6, 0xeb, 0x81, 0x3f, 0xc4, 0x63, 0xcb, 0x9d, 0x23,
  0xff, 0x7e, 0x99, 0xc6, 0x79, 0x14, 0x48, 0xe1, 0xaf, 0x17, 0x8b, 0x45,
  0x3d, 0xbe, 0x55, 0xe9, 0x6a, 0x94, 0x0b, 0x51, 0x05, 0x44, 0x95, 0x26,
  0x61, 0x65, 0x1a, 0x1d, 0xa9, 0xba, 0x4c, 0xc0, 0x91, 0x65, 0x35, 0x74,
  0xb7, 0xdd, 0x5a, 0x8e, 0x31, 0xbe, 0x22, 0xb3, 0x4c, 0x1a, 0x27, 0xce,
  0xc0, 0x06, 0x63, 0xe5, 0xfb, 0x3c, 0x06, 0xc5, 0xd7, 0xe0, 0xe2, 0x3e,
  0x78, 0xa0, 0x9e, 0xc6, 0xc3, 0x2b, 0x80, 0xf4, 0x59, 0xbd, 0x42, 0xc0,
  0x3c, 0x35, 0x17, 0x61, 0x4e, 0x82, 0x82, 0x4f, 0xca, 0x25, 0xa1, 0x9c,
  0xc6, 0x05, 0xa7, 0x10, 0x2f, 0x24, 0x40, 0xc6, 0x9d, 0x80, 0x70, 0xcf,
  0x16, 0x10, 0xb1, 0x8a, 0x81, 0x18, 0x63, 0xa8, 0x7d, 0x6b, 0xd3, 0x36,
  0xca, 0x27, 0xab, 0x7c, 0x1c, 0x94, 0x4f, 0xc3, 0xf2, 0x69, 0x52, 0x3e,
  0x5d, 0x6d, 0x93, 0x38, 0x23, 0xac, 0xf2, 0x39, 0x29, 0x0e, 0xa1, 0x04,
  0x3e, 0x60, 0x77, 0x0d, 0x1a, 0x14, 0xe6, 0x2b, 0x02, 0x0f, 0xaa, 0xf0,
  0xe3, 0x1a, 0x07, 0x04, 0x69, 0x67, 0x6c, 0xed, 0x86, 0x04, 0x74, 0xe5,
  0x5c, 0x4c, 0x2e, 0x93, 0xc7, 0xde, 0xf6, 0x2f, 0xe9, 0x06, 0xe5, 0x0a,
  0x51, 0x87, 0x09, 0xae, 0x6c, 0xb4, 0xb6, 0x82, 0xff, 0xe5, 0xb0, 0x3f,
  0x94, 0xff, 0xde, 0xec, 0xaa, 0x25, 0x52, 0xf8, 0xb8, 0x82, 0x4d, 0x24,
  0x6c, 0x6c, 0x55, 0xb0, 0xa1, 0x84, 0x0d, 0x14, 0xba, 0x81, 0x84, 0xd9,
  0x93, 0xfe, 0x44, 0xfc, 0xbb, 0xa8, 0x90, 0x76, 0x21, 0xb5, 0x43, 0x68,
  0xbc, 0x58, 0x64, 0x98, 0x02, 0x4f, 0x75, 0xf7, 0x18, 0xe7, 0x1d, 0xf4,
  0x1f, 0x1c, 0x6e, 0xa1, 0x2a, 0x26, 0x21, 0x62, 0xf1, 0xcb, 0x23, 0x6b,
  0x1e, 0xc6, 0xfe, 0x3d, 0xec, 0xf4, 0xa3, 0x74, 0x14, 0x6f, 0x19, 0xf5,
  0x10, 0x62, 0x4e, 0x6e, 0xb6, 0x85, 0xce, 0x2a, 0x27, 0x6b, 0xfc, 0x28,
  0x79, 0xd4, 0x2c, 0xcd, 0x72, 0x95, 0xc0, 0x84, 0x9d, 0xfb, 0x74, 0xe5,
  0x1e, 0x59, 0x2d, 0x1d, 0x5e, 0x7a, 0xb7, 0x71, 0x4e, 0xd9, 0x02, 0x87,
  0xae, 0x48, 0xa4, 0x05, 0xa0, 0x0a, 0x0e, 0xdc, 0x02, 0x06, 0x2a, 0x69,
  0x2c, 0x22, 0xb5, 0xa2, 0x78, 0x88, 0x6a, 0x9d, 0xb2, 0x38, 0xe0, 0x09,
  0x52, 0x50, 0x4a, 0x77, 0x38, 0xe6, 0x80, 0x45, 0x26, 0x6b, 0x29, 0x26,
  0x8b, 0xfb, 0x34, 0xae, 0x1c, 0x21, 0x3c, 0xa0, 0x58, 0x2f, 0x35, 0x1c,
  0x29, 0xc1, 0xc5, 0x2b, 0x95, 0x66, 0x97, 0x9e, 0x38, 0x45, 0x69, 0x51,
  0xa1, 0x64, 0x8d, 0x96, 0x98, 0x37, 0x3b, 0x57, 0xf6, 0x1a, 0xf0, 0x98,
  0x96, 0xc5, 0x21, 0x09, 0xb4, 0xd7, 0x81, 0x8f, 0x47, 0xd8, 0x97, 0x08,
  0x33, 0x45, 0x01, 0xc9, 0x33, 0xe6, 0x64, 0xb7, 0x56, 0x9c, 0x57, 0x28,
  0x80, 0x6e, 0x4a, 0x22, 0xb0, 0x16, 0xbc, 0xcf, 0x96, 0xb3, 0x9f, 0x74,
  0x39, 0x47, 0x67, 0x96, 0xc1, 0xfe, 0xf7, 0xad, 0x8b, 0x71, 0xcf, 0xfd,
  0x1a, 0xda, 0xb2, 0x32, 0xb3, 0x2a, 0x2c, 0xd2, 0x52, 0x2a, 0xc1, 0xad,
  0xd0, 0x30, 0xca, 0x30, 0x74, 0x48, 0x13, 0x7c, 0xad, 0xf5, 0xed, 0x71,
  0x66, 0xb4, 0x15, 0x6a, 0xd1, 0xb8, 0x66, 0x7c, 0x3c, 0xbf, 0x43, 0x7c,
  0x4e, 0xc1, 0xa4, 0x1e, 0x11, 0x32, 0xee, 0x54, 0x6e, 0xe5, 0x0e, 0x16,
  0x81, 0x67, 0xbd, 0xc4, 0xe9, 0x06, 0x64, 0x83, 0x76, 0x59, 0x20, 0x46,
  0x23, 0x63, 0x32, 0x30, 0x2e, 0x01, 0x37, 0xf9, 0xaa, 0xed, 0xd8, 0xcf,
  0xa5, 0x61, 0x86, 0x98, 0x5f, 0x20, 0xb6, 0x7d, 0xbc, 0x8a, 0x43, 0xb0,
  0x45, 0xce, 0x65, 0xaf, 0x91, 0x3f, 0x9f, 0xf8, 0x60, 0x4b, 0x82, 0x7c,
  0x42, 0x9f, 0x1c, 0xbb, 0xb1, 0x8e, 0x8d, 0x90, 0x3c, 0x1f, 0xf7, 0xaf,
  0x6d, 0x4b, 0xca, 0x4c, 0xfc, 0x98, 0xa0, 0x28, 0xa8, 0x66, 0xa4, 0x56,
  0xb0, 0xab, 0x4d, 0x5c, 0xac, 0x67, 0xe8, 0x64, 0xdb, 0x68, 0x54, 0xa2,
  0x77, 0xc8, 0x22, 0xd0, 0xd1, 0x08, 0xea, 0xf9, 0xaa, 0x16, 0x17, 0xab,
  0xa3, 0xeb, 0xa9, 0xcc, 0x34, 0x51, 0xf9, 0x94, 0x4e, 0x32, 0x68, 0xb5,
  0x92, 0x41, 0x9b, 0x89, 0xd5, 0xea, 0xc5, 0x8d, 0x39, 0xb3, 0x12, 0xd0,
  0x59, 0x0e, 0x4b, 0x13, 0xd0, 0x1c, 0x32, 0x39, 0xa7, 0xb8, 0xd6, 0x50,
  0x4d, 0x55, 0x22, 0x33, 0x63, 0xc4, 0x6a, 0xa4, 0x74, 0xd0, 0x2a, 0x4e,
  0xc9, 0x17, 0xd6, 0x9f, 0x43, 0xad, 0x72, 0x89, 0x42, 0x6b, 0xb5, 0x54,
  0x2d, 0x8c, 0xe1, 0x0e, 0xb1, 0xcb, 0x62, 0xd7, 0xcd, 0xa8, 0x72, 0xc4,
  0x70, 0xd0, 0x49, 0xda, 0xde, 0x25, 0xa1, 0x33, 0x2f, 0x79, 0xb5, 0x41,
  0xc1, 0x7c, 0xa6, 0xe1, 0xb6, 0xb5, 0x10, 0xb1, 0x63, 0x8a, 0x3d, 0xe1,
  0xe3, 0x3e, 0x82, 0x91, 0x0f, 0x36, 0x9a, 0xf1, 0x7b, 0xde, 0xae, 0x5d,
  0x7f, 0x4e, 0xa3, 0x7d, 0x5d, 0xec, 0xf0, 0xf6, 0x29, 0xc2, 0x7c, 0xcc,
  0x76, 0xd0, 0x85, 0x43, 0x08, 0x9b, 0xc2, 0x42, 0x09, 0x5d, 0x93, 0x20,
  0x08, 0xb1, 0x38, 0x4b, 0xc5, 0xb9, 0xbf, 0x32, 0x91, 0xcf, 0x37, 0x70,
  0x8d, 0x22, 0x92, 0xe4, 0x21, 0x3f, 0x93, 0xb9, 0xfb, 0x31, 0xf5, 0xf0,
  0x38, 0xb6, 0xbe, 0x2b, 0xe9, 0xe1, 0x6e, 0x56, 0x84, 0x62, 0x13, 0xde,
  0x7c, 0x46, 0xbd, 0x49, 0x51, 0xf2, 0xe2, 0x0e, 0xb4, 0xbf, 0x5d, 0x88,
  0xf3, 0x28, 0x0e, 0xb1, 0x4f, 0x85, 0x4a, 0xbc, 0x5e, 0x74, 0x40, 0xb3,
  0x36, 0xb0, 0x09, 0xe0, 0xbb, 0xe1, 0x30, 0x67, 0x3c, 0x60, 0x51, 0x3f,
  0x0d, 0x0e, 0xf9, 0x66, 0x2d, 0xbc, 0x64, 0x2e, 0xe4, 0xac, 0xd8, 0x39,
  0x72, 0xab, 0x34, 0x55, 0xbe, 0xc7, 0x01, 0xf6, 0x63, 0x71, 0x82, 0x6e,
  0x29, 0xb9, 0xad, 0x2a, 0x79, 0xf7, 0xfe, 0x1c, 0xa8, 0xef, 0x43, 0x50,
  0x79, 0xdc, 0xa8, 0xcc, 0xf6, 0xa0, 0xbb, 0xa9, 0xee, 0xa3, 0xe5, 0xba,
  0xb4, 0x8f, 0xa6, 0x51, 0xcc, 0x22, 0x13, 0xce, 0xc3, 0xcc, 0x43, 0xb2,
  0x3e, 0xf7, 0x27, 0x63, 0x77, 0x41, 0x42, 0x08, 0x25, 0x07, 0x85, 0xc9,
  0x0a, 0x9d, 0x49, 0x84, 0x37, 0x51, 0x7a, 0xb3, 0x22, 0x5a, 0x86, 0x57,
  0xed, 0x9d, 0x8b, 0x33, 0x93, 0x14, 0x0c, 0x4c, 0x9f, 0xb6, 0xdd, 0xc3,
  0x47, 0x63, 0x56, 0xe9, 0x68, 0x7f, 0x35, 0x2e, 0x72, 0x6f, 0x0f, 0xf2,
  0xb2, 0xd1, 0x60, 0x34, 0x58, 0x34, 0x78, 0x59, 0x56, 0x83, 0x51, 0x6b,
  0xf3, 0x8e, 0x64, 0x64, 0x4f, 0xec, 0xc5, 0xe0, 0xaa, 0xce, 0x4b, 0xee,
  0xee, 0x29, 0x99, 0x29, 0x71, 0xd6, 0xc4, 0x1c, 0xa1, 0xb9, 0x15, 0xd8,
  0x43, 0xfb, 0xf2, 0x39, 0x17, 0x48, 0xb5, 0x3b, 0x43, 0xb1, 0x46, 0x59,
  0x85, 0x4c, 0x87, 0x5a, 0x0a, 0x52, 0x68, 0xf6, 0x82, 0xdd, 0xe5, 0x25,
  0x74, 0xef, 0x50, 0xac, 0xd0, 0xbc, 0x53, 0xc8, 0x95, 0x96, 0xc4, 0xdb,
  0xf7, 0x06, 0x87, 0xf5, 0x3e, 0x6b, 0xa9, 0x33, 0xf4, 0x55, 0xab, 0xc5,
  0x72, 0x7c, 0x5b, 0x59, 0xec, 0x2f, 0xac, 0x85, 0x7d, 0xb8, 0x54, 0xfe,
  0xf5, 0x71, 0xf8, 0x2b, 0xa6, 0x61, 0x96, 0xb7, 0x8d, 0xf3, 0xb2, 0xbc,
  0x28, 0x69, 0x81, 0xc5, 0xc5, 0x89, 0x71, 0xa8, 0xa7, 0x16, 0x4b, 0x0f,
  0x91, 0xc8, 0xfb, 0x17, 0x79, 0xa9, 0xe8, 0xe8, 0x9a, 0x5e, 0x0e, 0x40,
  0x94, 0x6d, 0xf5, 0x5e, 0x7d, 0x9e, 0x95, 0xbb, 0xf5, 0x43, 0x8c, 0x52,
  0x18, 0x9a, 0xe9, 0x6a, 0xf7, 0x23, 0x2b, 0xf2, 0xc5, 0x4d, 0xa4, 0x3c,
  0x54, 0x06, 0xfc, 0x22, 0x52, 0xb4, 0x71, 0xd1, 0x6c, 0xa5, 0xab, 0xe5,
  0x21, 0xb5, 0xa8, 0xc2, 0x32, 0x74, 0x8f, 0x2e, 0x44, 0xea, 0xe4, 0x28,
  0xef, 0x53, 0x9a, 0x8c, 0xdd, 0x17, 0x31, 0x93, 0x09, 0x21, 0x2e, 0xef,
  0x44, 0x71, 0x3a, 0x9a, 0x0f, 0x0f, 0xd8, 0x63, 0xa9, 0xf9, 0x0d, 0x54,
  0x15, 0xad, 0x4e, 0xb1, 0x8e, 0xf5, 0x13, 0x94, 0x82, 0x8f, 0x21, 0x16,
  0x61, 0xb3, 0xce, 0xec, 0xe1, 0x38, 0xc0, 0x4b, 0x43, 0x89, 0x20, 0xab,
  0xa7, 0xc1, 0x11, 0x5c, 0x40, 0x06, 0xe3, 0xb1, 0x51, 0xfc, 0xf4, 0x07,
  0x07, 0x10, 0x63, 0xeb, 0x4d, 0x83, 0x47, 0x1b, 0x72, 0xb1, 0x6f, 0x31,
  0x20, 0x7a, 0xc6, 0x3e, 0x05, 0x47, 0xdf, 0x89, 0x7e, 0x50, 0x0f, 0xaf,
  0xae, 0xae, 0x2e, 0x5c, 0xd5, 0xa7, 0x6c, 0x16, 0x69, 0xea, 0x6b, 0x7e,
  0x2f, 0x0a, 0x77, 0x6a, 0xf7, 0x1d, 0x7b, 0xf3, 0xbb, 0x75, 0xe4, 0xff,
  0x8f, 0x0f, 0xcd, 0xea, 0x18, 0xc7, 0xa6, 0x55, 0x7e, 0x0f, 0x5a, 0x16,
  0x8c, 0x8a, 0x0a, 0x0a, 0x2e, 0xbb, 0xfb, 0x2a, 0x8e, 0x2b, 0xfc, 0x0a,
  0xfc, 0x20, 0xb6, 0x83, 0x87, 0x1f, 0x92, 0xa4, 0x76, 0x87, 0xbe, 0x1f,
  0xd5, 0xb1, 0x5a, 0xde, 0xa8, 0x5b, 0x78, 0xad, 0xf1, 0xdf, 0x6e, 0x07,
  0x8e, 0xf5, 0x36, 0xcb, 0x2a, 0x3b, 0x75, 0xbb, 0xf3, 0xbe, 0x7f, 0xff,
  0x5e, 0x2d, 0x7b, 0xec, 0xca, 0x61, 0xc8, 0x56, 0x2d, 0x53, 0xfc, 0x24,
  0xc7, 0x81, 0x2b, 0xeb, 0x61, 0xa3, 0x5c, 0x18, 0x5e, 0x5a, 0xd5, 0x39,
  0x96, 0xf7, 0x73, 0xee, 0xa2, 0xdd, 0xeb, 0x8c, 0xc2, 0x00, 0xee, 0x9b,
  0x24, 0x29, 0x67, 0x0a, 0x5e, 0x43, 0xa7, 0xe7, 0xe2, 0xb3, 0xac, 0x69,
  0x40, 0x1e, 0x34, 0x3f, 0x44, 0x59, 0xe6, 0x35, 0x1a, 0x99, 0x8a, 0x62,
  0x5a, 0xce, 0xa6, 0x2b, 0x7b, 0xcf, 0xa7, 0x63, 0x80, 0x98, 0xce, 0xd3,
  0xd9, 0x94, 0x75, 0x03, 0xb9, 0xa2, 0xd9, 0xfe, 0xe2, 0x28, 0xcb, 0xe7,
  0x6b, 0x42, 0xbd, 0x14, 0xd3, 0x3c, 0x8d, 0x5e, 0xd9, 0x1a, 0x74, 0x0c,
  0xcc, 0xc9, 0x54, 0x41, 0x55, 0x93, 0x54, 0xa1, 0x7a, 0x79, 0xcd, 0xab,
  0x35, 0x6e, 0x57, 0xf5, 0x9a, 0x01, 0xf2, 0x60, 0x3d, 0x9b, 0xf2, 0x43,
  0xed, 0x6c, 0xca, 0x5b, 0x91, 0x46, 0x02, 0x4f, 0x38, 0x41, 0xab, 0x5d,
  0x0c, 0x80, 0xca, 0xb3, 0x7f, 0x65, 0x58, 0x93, 0xb8, 0x9b, 0xdb, 0xe9,
  0xf9, 0x7c, 0x36, 0x3d, 0x97, 0x4b, 0xcf, 0x81, 0x6d, 0xfd, 0x37, 0x93,
  0x53, 0xb2, 0x02, 0x7f, 0xee, 0x53, 0x9b, 0x33, 0xa8, 0x14, 0x57, 0x4e,
  0xd9, 0x85, 0xf2, 0x43, 0x1d, 0xe8, 0x3d, 0xe0, 0x70, 0x73, 0xab, 0x41,
  0x24, 0xa6, 0x38, 0xcb, 0x4a, 0xb9, 0xea, 0x76, 0x88, 0xcb, 0x6b, 0xc5,
  0x0c, 0x92, 0xa8, 0xf2, 0x24, 0x6f, 0x4d, 0xb9, 0x25, 0xf2, 0xec, 0xab,
  0x41, 0xdf, 0x9e, 0x5c, 0xf6, 0xed, 0xbe, 0xdd, 0xd6, 0xfe, 0xe5, 0xba,
  0xc2, 0xde, 0x45, 0x98, 0xce, 0xee, 0xf8, 0x1f, 0x6d, 0x8d, 0xb2, 0xfb,
  0xe3, 0x14, 0x16, 0xeb, 0x9e, 0x53, 0x1a, 0x32, 0xbf, 0x5f, 0xfc, 0x58,
  0xa7, 0x54, 0x7b, 0x89, 0x28, 0xde, 0xa0, 0x27, 0x88, 0x5a, 0xfe, 0x81,
  0xa3, 0x26, 0xdf, 0x8f, 0xd3, 0x5d, 0x12, 0xbf, 0xd8, 0xe3, 0x27, 0xf1,
  0x7b, 0x06, 0x79, 0x78, 0x77, 0x77, 0x73, 0x7d, 0xa4, 0xbb, 0x81, 0xbc,
  0x43, 0xdf, 0x53, 0x6b, 0x94, 0x00, 0xd1, 0x06, 0x6a, 0x20, 0xd7, 0x4c,
  0x2b, 0xde, 0x8e, 0x53, 0xb1, 0xa0, 0xee, 0x72, 0x2b, 0x4f, 0xcf, 0x92,
  0xf9, 0x09, 0x95, 0x5e, 0xc5, 0x19, 0x9d, 0x7d, 0x80, 0x5f, 0xfb, 0x75,
  0x9c, 0x28, 0x3a, 0x32, 0xf2, 0x03, 0x6e, 0x7c, 0x5e, 0xaa, 0xcd, 0xa5,
  0xf2, 0x6f, 0x15, 0xdc, 0xc2, 0xaf, 0xfd, 0x52, 0x07, 0xaa, 0x67, 0x80,
  0x72, 0xaf, 0x57, 0xa2, 0x7c, 0x3d, 0xc7, 0xa9, 0xf6, 0x80, 0xc2, 0x1c,
  0x7b, 0x97, 0x27, 0x4d, 0x12, 0x68, 0x69, 0xd0, 0xf9, 0x21, 0xce, 0x7e,
  0xe6, 0x0f, 0xda, 0xb1, 0xc1, 0x56, 0xac, 0xfb, 0xc6, 0x01, 0x87, 0x12,
  0x72, 0x8f, 0x9f, 0x66, 0x3f, 0xdd, 0xde, 0x68, 0xf0, 0xf7, 0x38, 0xd5,
  0xc4, 0x9a, 0x6f, 0xac, 0x18, 0xdf, 0xde, 0x6b, 0x0c, 0x1d, 0x55, 0x9b,
  0x63, 0xba, 0xc1, 0x38, 0xd2, 0x32, 0x1c, 0x05, 0x99, 0x76, 0x96, 0x61,
  0x58, 0x16, 0x64, 0xbd, 0x23, 0x93, 0x36, 0xc4, 0x38, 0xa1, 0x64, 0x8d,
  0x8f, 0xdb, 0xfc, 0x89, 0xd5, 0xd8, 0x7d, 0xf1, 0xc9, 0x78, 0xa1, 0x30,
  0x9c, 0x08, 0xb5, 0xf2, 0x5e, 0x80, 0x3f, 0x85, 0x30, 0x12, 0x60, 0x4d,
  0xb9, 0xa5, 0xd0, 0x99, 0x50, 0xf4, 0x80, 0x05, 0x73, 0xd1, 0x96, 0x67,
  0x77, 0x0c, 0x00, 0x2d, 0x95, 0xc2, 0x5c, 0x04, 0xdd, 0x48, 0x30, 0x9d,
  0x69, 0x2f, 0xe3, 0x0e, 0x1d, 0x8d, 0x22, 0xf0, 0xce, 0xef, 0xe2, 0xaf,
  0x26, 0x8e, 0xb2, 0x25, 0xd7, 0xe9, 0xb9, 0x68, 0xfb, 0xaa, 0x15, 0x99,
  0x9f, 0x92, 0x84, 0xce, 0x16, 0x79, 0xc4, 0x2f, 0x74, 0xb5, 0x25, 0x26,
  0xc1, 0x19, 0xee, 0x6d, 0xc5, 0xb8, 0xa0, 0x05, 0x70, 0x9c, 0x5c, 0x43,
  0xb8, 0xf5, 0x97, 0x98, 0xbe, 0x0f, 0x31, 0x7b, 0xfc, 0xfb, 0xd3, 0x0d,
  0xa3, 0xd8, 0x95, 0x4b, 0xfc, 0x15, 0x8a, 0x96, 0xf8, 0xe6, 0xf6, 0x86,
  0x79, 0xf6, 0x0e, 0xda, 0x34, 0x66, 0x0c, 0xf0, 0x0f, 0x9c, 0x95, 0x5e,
  0xf6, 0x6d, 0xbd, 0xd7, 0xe7, 0xa3, 0x4f, 0x5f, 0xce, 0x43, 0xe5, 0x17,
  0x85, 0x74, 0xe7, 0x59, 0x4a, 0x36, 0x3a, 0xe9, 0x95, 0xc4, 0x30, 0x46,
  0xc1, 0x35, 0xa2, 0xe8, 0xac, 0xb7, 0x45, 0x9f, 0xd1, 0xa3, 0x17, 0xe1,
  0x8d, 0xf6, 0xef, 0x5f, 0x7f, 0xf9, 0x40, 0x69, 0xf2, 0x3b, 0xfe, 0x6f,
  0x0e, 0xd6, 0x1b, 0x0c, 0xde, 0x07, 0x7f, 0x24, 0x30, 0x00, 0xe1, 0x8f,
  0xcc, 0xe1, 0xfa, 0xe7, 0x2c, 0x8e, 0x74, 0x81, 0x88, 0x13, 0x1c, 0x9d,
  0xe9, 0xff, 0x78, 0xff, 0x51, 0x37, 0xf4, 0x73, 0x3f, 0x4f, 0xd3, 0x3b,
  0xb1, 0x03, 0x7a, 0x4f, 0xe2, 0xa3, 0x14, 0xa3, 0xe0, 0x89, 0x69, 0x84,
  0x85, 0x79, 0x5e, 0x21, 0x1c, 0x64, 0x8e, 0x3c, 0x4f, 0xb2, 0x07, 0x1a,
  0x6e, 0xf0, 0xdb, 0xb7, 0x03, 0xcb, 0x92, 0x50, 0xb6, 0x28, 0xcf, 0xde,
  0xbe, 0x3d, 0x0b, 0x40, 0x43, 0xaf, 0xa6, 0x87, 0x21, 0x0c, 0xe5, 0x16,
  0x8a, 0xa8, 0x62, 0x34, 0x7d, 0x92, 0x48, 0x84, 0x68, 0xdb, 0x75, 0xa4,
  0x80, 0x49, 0x02, 0xd9, 0x1b, 0xeb, 0x14, 0x12, 0x58, 0xf0, 0x80, 0x46,
  0xd1, 0xe0, 0x00, 0x10, 0x89, 0x2c, 0x8a, 0x7c, 0x9d, 0xa0, 0x80, 0x4a,
  0x22, 0x56, 0x86, 0xeb, 0x04, 0x0c, 0x52, 0x70, 0x80, 0xec, 0x6b, 0xac,
  0x06, 0x88, 0x44, 0x16, 0xa5, 0xa9, 0x4e, 0x20, 0xa0, 0x37, 0xd7, 0x92,
  0x48, 0x14, 0x89, 0x3a, 0x09, 0xc0, 0xfe, 0x89, 0x4b, 0x0b, 0x8a, 0xc4,
  0x6c, 0x98, 0x51, 0x80, 0x0d, 0xdb, 0x93, 0x10, 0x1e, 0x30, 0xb5, 0x38,
  0x83, 0x25, 0x7c, 0xce, 0xc4, 0x81, 0xf7, 0xca, 0x72, 0xf6, 0x61, 0x6c,
  0xa3, 0x23, 0x66, 0xbb, 0x89, 0x7b, 0xbd, 0x9d, 0x08, 0x09, 0x56, 0x63,
  0xce, 0x7a, 0xbb, 0x06, 0x55, 0x1c, 0x81, 0x75, 0xfe, 0xbd, 0x1a, 0x1d,
  0xc7, 0xb3, 0xde, 0x49, 0x83, 0x65, 0xda, 0x76, 0xb3, 0x3b, 0x18, 0xe0,
  0xb5, 0x38, 0x2e, 0xd9, 0x9c, 0x2e, 0x86, 0xa1, 0x30, 0x7d, 0x04, 0x8f,
  0xc7, 0x39, 0x3d, 0x53, 0xd6, 0x36, 0xb5, 0x2e, 0x6e, 0x5f, 0x99, 0x63,
  0xa5, 0xa9, 0x50, 0xd5, 0x3a, 0x11, 0xd5, 0x22, 0xfe, 0xcd, 0xb2, 0x0f,
  0x1f, 0x7f, 0xfd, 0xc5, 0xd3, 0x65, 0xb9, 0xd2, 0x77, 0x86, 0x8d, 0x47,
  0x35, 0x7f, 0xb3, 0x6d, 0xee, 0x19, 0xfb, 0xe5, 0x59, 0xfb, 0xe4, 0x59,
  0x47, 0xc8, 0x83, 0x7c, 0xd7, 0xce, 0x36, 0x88, 0x50, 0x0d, 0xca, 0x22,
  0x9c, 0x0b, 0xb3, 0x9e, 0xbe, 0xab, 0xf3, 0xeb, 0xd8, 0x8e, 0xbd, 0xf2,
  0x48, 0xe2, 0x35, 0xb3, 0xdb, 0x10, 0xb9, 0xeb, 0x75, 0x25, 0xb7, 0x21,
  0xb3, 0xd6, 0xeb, 0x4c, 0x6c, 0x83, 0xe5, 0x2c, 0xff, 0x9a, 0x64, 0x3b,
  0xa9, 0x39, 0xee, 0x56, 0xa6, 0xac, 0xd7, 0x9d, 0xd7, 0x06, 0xcb, 0x58,
  0xaf, 0x9d, 0xce, 0x06, 0x4b, 0x56, 0xaf, 0x9d, 0xc9, 0x46, 0x91, 0xb9,
  0x5e, 0x77, 0x22, 0x1b, 0x22, 0x69, 0xbd, 0xae, 0x0c, 0x36, 0xca, 0xc4,
  0xf4, 0xf6, 0xe4, 0xaf, 0xd1, 0x1d, 0xff, 0x3f, 0x08, 0x80, 0x67, 0x3b,
  0xf2, 0xc1, 0x32, 0x78, 0xc9, 0xd4, 0x3f, 0x45, 0x73, 0x0c, 0xa7, 0xe9,
  0x4f, 0x11, 0xf1, 0xf4, 0x77, 0x24, 0x79, 0x07, 0x00, 0xe8, 0x81, 0xef,
  0x84, 0x03, 0xd9, 0xdb, 0x12, 0xde, 0xa4, 0xc7, 0xd8, 0x6b, 0xc0, 0x90,
  0xd2, 0x61, 0xec, 0x3d, 0x91, 0xef, 0x85, 0x93, 0x18, 0x6c, 0x05, 0x30,
  0xe6, 0x09, 0xf6, 0x1c, 0xc3, 0x33, 0xb3, 0x9e, 0x3d, 0xfb, 0xf0, 0x5c,
  0x58, 0xcb, 0xde, 0x41, 0xfc, 0x3b, 0x61, 0x1f, 0x7b, 0x7b, 0x64, 0x9c,
  0xb8, 0x72, 0xec, 0x8d, 0xb2, 0xb7, 0xc2, 0x38, 0x06, 0x80, 0x10, 0xfd,
  0x24, 0x5b, 0xca, 0x33, 0x29, 0x7a, 0xfb, 0xdb, 0x1d, 0xcf, 0x51, 0x11,
  0x38, 0x27, 0x6c, 0x32, 0xb5, 0x78, 0x54, 0x62, 0xbc, 0x36, 0x57, 0xe8,
  0x7b, 0xd3, 0x72, 0x6f, 0xb6, 0x75, 0x72, 0x64, 0x1d, 0x72, 0x67, 0x28,
  0x45, 0xa1, 0x68, 0xc4, 0x86, 0x6d, 0xc1, 0xd4, 0x25, 0x67, 0x89, 0xff,
  0x01, 0xf6, 0xde, 0x5d, 0xc9, 0x1e, 0x2c, 0x00, 0x00
};
const size_t indexLen = 2841;
void wifiServerInit();
void wifiServerListener();
void wifiServerSendIndex(FILE* stream);
void wifiServerSendSSID(FILE* stream, char linkID);
void wifiServerSaveConfig();
void wifiServerSendConfig(FILE* stream);
#endif
