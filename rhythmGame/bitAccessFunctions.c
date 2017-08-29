#ifndef BITACCESSFUNCTIONS_C
#define BITACCESSFUNCTIONS_C

// Bit-access function (CODE TAKEN FROM CS120B LAB MANUAL)
unsigned char SetBit(unsigned char tmp, unsigned char bitPlace, unsigned char setToThisBitValue) {
	return (setToThisBitValue ? tmp | (0x01 << bitPlace) : tmp & ~(0x01 << bitPlace));
}
unsigned char GetBit(unsigned char tmp, unsigned char bitPlace) {
	return ((tmp & (0x01 << bitPlace)) != 0);
}

#endif