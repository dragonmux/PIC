/*
 * This file is part of PIC
 * Copyright © 2012 Rachel Mant (dx-mon@users.sourceforge.net)
 *
 * PIC is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PIC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Common.h"
#include "Memory16F88.h"
#include "16F88.h"

PIC16F88::PIC16F88(ROM *ProgramMemory)
{
	q = 0;
	nextIsNop = false;
	trapped = false;
	PC = new Register<uint16_t>(13);
	WREG = new Register<>();
	program = ProgramMemory;
	memory = Memory::Generate<Memory16F88>();
	PCL = new Register<>((*memory)[2]);
	STATUS = new Register<>((*memory)[3]);
	PCLATCH = new Register<>((*memory)[10]);
	CallStack = new Stack<uint16_t, 8>();
}

void PIC16F88::DecodeInstruction()
{
	uint16_t instrWord = (*program)[*PC] & 0x3FFF;
	switch ((instrWord & 0xFF80) >> 7)
	{
		case 0:
		{
			uint8_t subInstr = instrWord & 0x7F;
			if ((subInstr & 0x1F) == 0)
				inst = NOP;
			else if (subInstr == 0x64)
				inst = CLRWDT;
			else if (subInstr == 0x09)
				inst = RETFIE;
			else if (subInstr == 0x08)
				inst = RETURN;
			else if (subInstr == 0x63)
				inst = SLEEP;
			else
				TRAP();
			break;
		}
		case 1:
			inst = MOVWF;
			break;
		case 2:
			inst = CLRW;
			break;
		case 3:
			inst = CLRF;
			break;
		case 4:
		case 5:
			inst = SUBWF;
			break;
		case 6:
		case 7:
			inst = DECF;
			break;
		case 8:
		case 9:
			inst = IORWF;
			break;
		case 10:
		case 11:
			inst = ANDWF;
		case 12:
		case 13:
			inst = XORWF;
			break;
		case 14:
		case 15:
			inst = ADDWF;
			break;
		case 16:
		case 17:
			inst = MOVF;
			break;
		case 18:
		case 19:
			inst = COMF;
			break;
		case 20:
		case 21:
			inst = INCF;
			break;
		case 22:
		case 23:
			inst = DECFSZ;
			break;
		case 24:
		case 25:
			inst = RRF;
			break;
		case 26:
		case 27:
			inst = RLF;
			break;
		case 28:
		case 29:
			inst = SWAPF;
			break;
		case 30:
		case 31:
			inst = INCFSZ;
			break;
		// 32 through 63 are handled in the default case as 3 bits have to be treated as "don't care" bits.
		// 64 through 95 are handled in the default case as only the high 3 bits have meaningful values.
		// 96 through 111 are handled in the default case as only the high 4 bits have meaningful values.
		case 112:
		case 113:
			inst = IORLW;
			break;
		case 114:
		case 115:
			inst = ANDLW;
			break;
		case 116:
		case 117:
			inst = XORLW;
			break;
		// 118 and 119 don't exist and should fall through to TRAP()
		// 120 thrhough 127 are handled in the default case as only the top 5 bits have meaningful values
		default:
		{
			uint8_t which = (instrWord & 0xF000) >> 12;
			uint8_t subInstr = (instrWord & 0x0F80) >> 5;
			if (which == 1)
			{
				subInstr = (subInstr & 0x18) >> 3;
				if (subInstr == 0)
					inst = BCF;
				else if (subInstr == 1)
					inst = BSF;
				else if (subInstr == 2)
					inst = BTFSC;
				else if (subInstr == 3)
					inst = BTFSS;
				else
					// This should never be run as the shifting above makes this an impossible case!
					TRAP();
				break;
			}
			else if (which == 2)
			{
				subInstr = (subInstr & 0x10) >> 4;
				if (subInstr == 0)
					inst = CALL;
				else
					inst = GOTO;
				break;
			}
			which = (instrWord & 0xFC00) >> 10;
			if (which == 12)
				inst = MOVLW;
			else if (which == 13)
				inst = RETLW;
			else if (which == 15)
			{
				subInstr = (instrWord & 0x0200) >> 9;
				if (subInstr == 0)
					inst = SUBLW;
				else
					inst = ADDLW;
			}
			else
				TRAP();
		}
	}
}

uint8_t PIC16F88::GetBank()
{
	return (*STATUS & 0x60) >> 5;
}

uint8_t PIC16F88::GetMemoryContents(uint8_t partialAddress)
{
	// STATUS->value() & 0x60 gives us the bank selector bits
	// Dereference takes a bank as the first param and the address
	// in that bank as the second
	return memory->Dereference(GetBank(), partialAddress);
}

void PIC16F88::SetMemoryContents(uint8_t partialAddress, uint8_t newVal)
{
	uint8_t *location = memory->Reference(GetBank(), partialAddress);
	*location = newVal;
}

void PIC16F88::CheckZero(uint8_t value)
{
	*STATUS = *STATUS & (value == 0 ? 0x04 : ~0x04);
}

void PIC16F88::StoreValue(uint8_t value, bool updateZero)
{
	// Update the zero flag if needed
	if (updateZero)
		CheckZero(value);

	if ((instrWord & 0x80) == 0)
		*WREG = value;
	else
		SetMemoryContents(instrWord & 0x7F, value);
}

uint8_t PIC16F88::SetCarry(bool val)
{
	uint8_t ret = *STATUS & 0x01;
	*STATUS = *STATUS & (val ? 0x01 : ~0x01);
	return ret;
}

uint16_t PIC16F88::GetPCHFinalBits()
{
	return (*PCLATCH & 0x18) >> 3;
}

void PIC16F88::ProcessInstruction()
{
	uint8_t memAddr = instrWord & 0x7F;
	uint8_t literal = instrWord & 0xFF;
	uint8_t bit = (instrWord & 0x0380) >> 7;
	switch (inst)
	{
		case ADDWF:
		{
			uint8_t w = *WREG, f = GetMemoryContents(memAddr);
			uint8_t val = w + f;
			StoreValue(val, true);
			SetCarry(val < (w | f));
			break;
		}
		case ANDWF:
			StoreValue(*WREG & GetMemoryContents(memAddr), true);
			break;
		case CLRF:
			StoreValue(0, true);
			break;
		case CLRW:
			WREG = 0;
			CheckZero(0);
			break;
		case COMF:
			StoreValue(~GetMemoryContents(memAddr), true);
			break;
		case DECF:
			StoreValue(GetMemoryContents(memAddr) - 1, true);
			break;
		case DECFSZ:
		{
			uint8_t val = GetMemoryContents(memAddr) - 1;
			StoreValue(val, false);
			if (val == 0)
			{
				*PC += 4;
				nextIsNop = true;
			}
			break;
		}
		case INCF:
			StoreValue(GetMemoryContents(memAddr) + 1, true);
			break;
		case INCFSZ:
		{
			uint8_t val = GetMemoryContents(memAddr) + 1;
			StoreValue(val, false);
			if (val == 0)
			{
				*PC += 4;
				nextIsNop = true;
			}
			break;
		}
		case IORWF:
			StoreValue(*WREG | GetMemoryContents(memAddr), true);
			break;
		case MOVF:
			StoreValue(GetMemoryContents(memAddr), true);
			break;
		case MOVWF:
			*WREG = GetMemoryContents(memAddr);
			CheckZero(*WREG);
			break;
		case NOP:
			return;
		case RLF:
		{
			uint8_t val = GetMemoryContents(memAddr), carry;
			carry = SetCarry((val & 0x80) == 0x80);
			val <<= 1;
			val |= carry;
			break;
		}
		case RRF:
		{
			uint8_t val = GetMemoryContents(memAddr), carry;
			carry = SetCarry((val & 0x01) == 1);
			val >>= 1;
			val |= carry << 7;
			break;
		}
		case SUBWF:
		{
			uint8_t f = GetMemoryContents(memAddr), w = *WREG;
			StoreValue(f - w, true);
			SetCarry(w < f);
			break;
		}
		case SWAPF:
		{
			uint8_t val = GetMemoryContents(memAddr);
			StoreValue(((val & 0x0F) << 4) | ((val & 0xF0) >> 4), false);
			break;
		}
		case XORWF:
			StoreValue(*WREG ^ GetMemoryContents(memAddr), true);
			break;
		case BCF:
			SetMemoryContents(memAddr, GetMemoryContents(memAddr) & ~(1 << bit));
			break;
		case BSF:
			SetMemoryContents(memAddr, GetMemoryContents(memAddr) | (1 << bit));
			break;
		case BTFSC:
			if ((GetMemoryContents(memAddr) & (1 << bit)) == 0)
			{
				*PC += 4;
				nextIsNop = true;
			}
			break;
		case BTFSS:
			if ((GetMemoryContents(memAddr) & (1 << bit)) != 0)
			{
				*PC += 4;
				nextIsNop = true;
			}
			break;
		case ADDLW:
		{
			uint8_t w, chk = *WREG | literal;
			*WREG += literal;
			w = *WREG;
			CheckZero(w);
			SetCarry(w < chk);
			break;
		}
		case ANDLW:
			*WREG &= literal;
			CheckZero(*WREG);
			break;
		case CALL:
			CallStack->Push(*PC);
			*PC = (GetPCHFinalBits() << 11) | (instrWord & 0x3FF);
			nextIsNop = true;
			break;
		case CLRWDT:
			// TODO: IMPLEMENT!!
			return;
		case GOTO:
			*PC = (GetPCHFinalBits() << 11) | (instrWord & 0x3FF);
			nextIsNop = true;
			break;
		case IORLW:
			*WREG |= literal;
			CheckZero(*WREG);
			break;
		case MOVLW:
			*WREG = literal;
			break;
		case RETFIE:
			*PC = CallStack->Pop();
			// TODO: Enable GIE
			nextIsNop = true;
			break;
		case RETLW:
			*WREG = literal;
		case RETURN:
			*PC = CallStack->Pop();
			nextIsNop = true;
			break;
		case SLEEP:
			// TODO: IMPLEMENT!!
			return;
		case SUBLW:
		{
			uint8_t w = *WREG;
			*WREG = literal - w;
			CheckZero(w);
			SetCarry(w < literal);
			break;
		}
		case XORLW:
			*WREG ^= literal;
			CheckZero(*WREG);
			break;
		default:
			TRAP();
	}
}

void PIC16F88::Step()
{
	if (q == 0)
	{
		if (nextIsNop == true)
			inst = NOP;
		else
			DecodeInstruction();
	}
	else if (q == 2)
		ProcessInstruction();
	else if (q == 3)
	{
		if (nextIsNop == false)
			PC += 2;
		else
			nextIsNop = false;
	}

	q++;
	q %= 4;
}
