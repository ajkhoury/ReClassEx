#pragma once

#include <stdio.h>
#include <limits.h>     /* for CHAR_BIT */
#include <malloc.h>

#define BITMASK(b) 		(1 << ((b) % CHAR_BIT))
#define BITSLOT(b) 		((b) / CHAR_BIT)
#define BITSET(a, b) 	((a)[BITSLOT(b)] |= BITMASK(b))
#define BITCLEAR(a, b) 	((a)[BITSLOT(b)] &= ~BITMASK(b))
#define BITTEST(a, b) 	((a)[BITSLOT(b)] & BITMASK(b))
#define BITNSLOTS(nb) 	((nb + CHAR_BIT - 1) / CHAR_BIT)

#define SET_BIT(a, bit_number) ((a) |= 1 << (bit_number))
#define RESET_BIT(a, bit_number) ((a) &= ~(1 << (bit_number)))
#define FLIP_BIT(a, bit_number) ((a) ^= 1 << (bit_number))
#define GET_BIT_VALUE(a, bit_number) (((a) & (1 << (bit_number))) >> bit_number)

template<typename T>
class BitArray
{
public:
	BitArray()
	{
		value = 0xFF;
		length = sizeof(T) * CHAR_BIT;
		size = length + 1;
		bits = new char[size];
	}

	BitArray(T num)
	{
		value = num;
		length = sizeof(T) * CHAR_BIT;
		size = length + 1;
		bits = new char[size];
	}

	~BitArray()
	{
		delete[] bits;
	}

	char* GetBitsString()
	{
		for (int i = length - 1; i >= 0; i--)
		{
			unsigned char bit = GetBit(i);
			bits[(length - 1) - i] = bit + 0x30;
		}

		bits[size - 1] = '\0';

		return bits;
	}

	char* GetBitsReverseString()
	{ 
		for (int i = 0; i < length; i++)
		{
			unsigned char bit = GetBit(i);
			bits[i] = bit + '0'; // Get ascii representation of bit
		}

		bits[size - 1] = '\0';

		return bits;
	}

	//void PrintBit(int index)
	//{
	//	printf("\nbits[%d] = %d", index, GetBit(index));
	//}

	unsigned char GetBit(int index)
	{
		return (value & (1 << index)) != 0;
	}

	unsigned int RotateRight(int numOfShifts, int numOfBits)
	{
		unsigned lsbs = value & ((1 << numOfShifts) - 1);
		return (value >> numOfShifts) | (lsbs << (numOfBits - numOfShifts));
	}

	int GetValue()
	{
		return value;
	}

	void SetValue(T val)
	{
		value = val;
	}

	void SetValue(char* bits)
	{
		unsigned char result = 0;
		for (int i = 0; i < CHAR_BIT; ++i)
			result |= (bits[i] == '1') << (7 - i);
		value = (T)result;
	}

private:
	T value;
	size_t size;
	int length;
	char* bits;
};