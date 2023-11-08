static unsigned char find_png[] = {
0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d, 
0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x0e, 
0x08, 0x06, 0x00, 0x00, 0x00, 0x1f, 0x48, 0x2d, 0xd1, 0x00, 0x00, 0x00, 
0x04, 0x67, 0x41, 0x4d, 0x41, 0x00, 0x00, 0xd6, 0xd8, 0xd4, 0x4f, 0x58, 
0x32, 0x00, 0x00, 0x00, 0x19, 0x74, 0x45, 0x58, 0x74, 0x53, 0x6f, 0x66, 
0x74, 0x77, 0x61, 0x72, 0x65, 0x00, 0x41, 0x64, 0x6f, 0x62, 0x65, 0x20, 
0x49, 0x6d, 0x61, 0x67, 0x65, 0x52, 0x65, 0x61, 0x64, 0x79, 0x71, 0xc9, 
0x65, 0x3c, 0x00, 0x00, 0x02, 0xad, 0x49, 0x44, 0x41, 0x54, 0x78, 0xda, 
0x62, 0xfc, 0xff, 0xff, 0x3f, 0x23, 0x2f, 0x2f, 0xaf, 0x23, 0x90, 0x8e, 
0x62, 0x66, 0x66, 0x76, 0x65, 0x62, 0x62, 0x62, 0x64, 0x00, 0x82, 0x7f, 
0xff, 0xfe, 0xfd, 0xff, 0xfb, 0xf7, 0xef, 0x6e, 0x46, 0x46, 0xc6, 0x65, 
0x9f, 0x3f, 0x7f, 0xde, 0x0f, 0x14, 0xfa, 0xcf, 0x80, 0x04, 0x00, 0x02, 
0x88, 0x81, 0x87, 0x87, 0xc7, 0x49, 0x40, 0x40, 0xe0, 0x7c, 0x48, 0x58, 
0xe8, 0xff, 0xa5, 0x2b, 0x97, 0xff, 0xbf, 0xfb, 0xe8, 0xc1, 0xff, 0x5b, 
0x0f, 0xef, 0xfd, 0x5f, 0xb0, 0x7c, 0xc9, 0xff, 0x80, 0x90, 0xe0, 0xff, 
0x40, 0xb9, 0x0b, 0xdc, 0xdc, 0xdc, 0x2e, 0x40, 0xb5, 0x20, 0x03, 0x19, 
0x81, 0x16, 0x30, 0x80, 0x30, 0x40, 0x00, 0x31, 0x00, 0x05, 0xe7, 0x04, 
0x87, 0x86, 0xfe, 0xbf, 0x79, 0xef, 0xce, 0xb7, 0x6f, 0xbf, 0x7f, 0xfe, 
0xff, 0xfa, 0xeb, 0xc7, 0xff, 0x8f, 0x3f, 0xbf, 0xfd, 0x7f, 0xf7, 0xfd, 
0xcb, 0xff, 0x73, 0x37, 0xaf, 0x7c, 0xf3, 0x0b, 0x0c, 0xf8, 0x0f, 0x54, 
0xb3, 0x40, 0x41, 0x41, 0x81, 0x03, 0xa8, 0x91, 0x05, 0x66, 0x00, 0x40, 
0x00, 0x31, 0x81, 0x9c, 0x17, 0x10, 0x1c, 0xc0, 0x20, 0x2d, 0x2d, 0xc3, 
0xf9, 0x0f, 0xe8, 0x98, 0x3f, 0xff, 0xff, 0x31, 0xfc, 0x03, 0x99, 0x0a, 
0x94, 0x15, 0x16, 0x16, 0xe1, 0xb4, 0x77, 0x75, 0x66, 0x00, 0x3a, 0xdf, 
0xe5, 0xfd, 0xfb, 0xf7, 0x9c, 0x40, 0x21, 0x56, 0x20, 0x66, 0x06, 0xb9, 
0x14, 0x20, 0x80, 0x58, 0x40, 0x7e, 0x32, 0xb1, 0x30, 0x07, 0x6a, 0xf8, 
0xcb, 0x00, 0x72, 0xc1, 0xef, 0x7f, 0x7f, 0x18, 0xfe, 0x02, 0x19, 0xbf, 
0x7f, 0xfe, 0x64, 0xf8, 0xf2, 0xf9, 0x33, 0x83, 0xa2, 0x92, 0x22, 0x03, 
0xd0, 0x70, 0xa6, 0x9f, 0x3f, 0x7f, 0xf2, 0x22, 0x7b, 0x11, 0x20, 0x80, 
0xc0, 0x56, 0xff, 0xfe, 0xfb, 0x97, 0xe1, 0xcf, 0x3f, 0xa0, 0x46, 0xa0, 
0x23, 0x7e, 0xfd, 0xf9, 0xc3, 0xf0, 0xfb, 0xd7, 0x2f, 0x86, 0x1f, 0x5f, 
0xbf, 0x31, 0x7c, 0xfa, 0xf8, 0x89, 0xe1, 0xf3, 0xa7, 0xcf, 0x20, 0x3f, 
0x31, 0x03, 0x03, 0x4a, 0x08, 0x14, 0x66, 0x30, 0x0c, 0x10, 0x40, 0x4c, 
0xa0, 0xd0, 0x3b, 0x76, 0xf8, 0x28, 0xc3, 0x97, 0xaf, 0x5f, 0x19, 0xbe, 
0x7c, 0xfa, 0xc2, 0xf0, 0xe5, 0xc3, 0x27, 0x86, 0x4f, 0xef, 0x3f, 0x32, 
0x7c, 0x02, 0xd2, 0x20, 0x1b, 0xaf, 0x5e, 0xbe, 0x0c, 0x0a, 0x61, 0x66, 
0x20, 0x16, 0x04, 0x6a, 0xe0, 0x01, 0x62, 0x76, 0x90, 0x73, 0x01, 0x02, 
0x88, 0x99, 0x85, 0x85, 0xc5, 0xe0, 0xe7, 0xaf, 0x9f, 0xfa, 0x0a, 0x8a, 
0x8a, 0x3f, 0x81, 0x2e, 0x65, 0xf9, 0xfc, 0x09, 0xa8, 0xf1, 0xe3, 0x47, 
0x86, 0x9f, 0x3f, 0x7f, 0x30, 0x3c, 0x7a, 0xf4, 0xe8, 0xcf, 0xb6, 0x8d, 
0x9b, 0x98, 0xee, 0xdf, 0xbb, 0xff, 0x1c, 0xa8, 0xf1, 0x05, 0xd0, 0xe6, 
0x27, 0x40, 0x4d, 0x3f, 0x80, 0xf8, 0x17, 0x40, 0x00, 0x31, 0xb3, 0xb2, 
0xb2, 0xfe, 0x79, 0xf6, 0xe4, 0xa9, 0xfe, 0xe3, 0x87, 0x8f, 0x24, 0xbf, 
0x7c, 0xfd, 0xc2, 0xf0, 0x07, 0xe8, 0xec, 0x0f, 0x1f, 0x3e, 0x30, 0x9c, 
0x3e, 0x75, 0x8a, 0x01, 0xa4, 0xe9, 0xc9, 0xad, 0xeb, 0xef, 0xf2, 0xa3, 
0xbd, 0xbf, 0xef, 0x3b, 0x75, 0xd9, 0x1b, 0xa8, 0xe1, 0x22, 0x10, 0x3f, 
0x00, 0x69, 0x06, 0x08, 0x20, 0x46, 0x41, 0x41, 0x41, 0xfe, 0x1f, 0x3f, 
0x7e, 0x78, 0x03, 0x23, 0x3a, 0x1c, 0x18, 0x08, 0xd6, 0xc0, 0xc0, 0x02, 
0x85, 0x1a, 0xc8, 0x69, 0x4c, 0x40, 0xf1, 0x8f, 0x3e, 0xc6, 0x8a, 0xff, 
0x53, 0x3d, 0x8c, 0xa5, 0x39, 0x54, 0x6d, 0x1e, 0x3a, 0x45, 0x67, 0x0b, 
0x00, 0xe5, 0x12, 0x81, 0x78, 0x2f, 0x40, 0x00, 0x81, 0xe2, 0x04, 0x14, 
0x5a, 0xfc, 0x40, 0x9b, 0x25, 0x80, 0x8a, 0x65, 0x80, 0xce, 0x91, 0x05, 
0xf2, 0x45, 0x41, 0xe2, 0x40, 0xb6, 0x21, 0x0b, 0x13, 0xa3, 0x8e, 0x83, 
0xae, 0x1c, 0x5b, 0x49, 0x88, 0x3d, 0xef, 0x5f, 0x19, 0x83, 0x53, 0x5e, 
0x09, 0x85, 0xaa, 0x40, 0xb9, 0x10, 0x80, 0x00, 0x62, 0x86, 0x86, 0x2c, 
0xc8, 0x06, 0x66, 0xa0, 0x42, 0x16, 0x20, 0x66, 0xfa, 0x0f, 0x01, 0xbf, 
0x80, 0xe2, 0x77, 0x80, 0x71, 0xfa, 0xfb, 0xde, 0x8b, 0x0f, 0x22, 0x4f, 
0x5f, 0xbf, 0xff, 0x6a, 0x23, 0xcd, 0xaa, 0x11, 0x13, 0x15, 0x71, 0x7a, 
0xe9, 0xe6, 0x7d, 0x39, 0x00, 0x01, 0x04, 0xd2, 0xc8, 0x04, 0x4b, 0x0d, 
0xd0, 0x28, 0xfa, 0x0b, 0xc4, 0x3f, 0x81, 0xf8, 0x2b, 0x10, 0x7f, 0x04, 
0xe2, 0xb3, 0xa0, 0xc0, 0x78, 0xf0, 0xea, 0xa3, 0xe2, 0x97, 0x6f, 0x3f, 
0x3e, 0xc8, 0xb3, 0x7f, 0xe7, 0x5d, 0x7f, 0xf2, 0xd6, 0x1e, 0x80, 0x00, 
0x62, 0x84, 0xda, 0x08, 0x4a, 0x11, 0xa0, 0x24, 0xc5, 0x0d, 0x0d, 0x72, 
0x6e, 0xa8, 0x18, 0x08, 0xfc, 0x86, 0x1a, 0x12, 0x0e, 0xc4, 0x91, 0x20, 
0x57, 0x80, 0xfc, 0x08, 0x10, 0x60, 0x00, 0xae, 0x0a, 0x42, 0x70, 0xb1, 
0xfe, 0x60, 0xac, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae, 
0x42, 0x60, 0x82, 
};