enum PIC16F88Instruction
{
	ADDWF,
	ANDWF,
	CLRF,
	CLRW,
	COMF,
	DECF,
	DECFSZ,
	INCF,
	INCFSZ,
	IORWF,
	MOVF,
	MOVWF,
	NOP,
	RLF,
	RRF,
	SUBWF,
	SWAPF,
	XORWF,
	BCF,
	BSF,
	BTFSC,
	BTFSS,
	ADDLW,
	ANDLW,
	CALL,
	CLRWDT,
	GOTO,
	IORLW,
	MOVLW,
	RETFIE,
	RETLW,
	RETURN,
	SLEEP,
	SUBLW,
	XORLW
};

class PIC16F88
{
public:
	PIC16F88(ROM *ProgramMemory);
	void Step();

private:
	uint8_t q;
	bool nextIsNop, trapped;
	Memory *memory;
	ROM *program;
	Stack<uint16_t, 8> *CallStack;
	Register<uint16_t> *PC;
	Register<> *WREG, *PCL, *STATUS, *PCLATCH;
	PIC16F88Instruction inst;
	uint16_t instrWord;

private:
	void DecodeInstruction();
	void ProcessInstruction();

	uint8_t GetBank();
	uint8_t GetMemoryContents(uint8_t partialAddress);
	void SetMemoryContents(uint8_t partialAddress, uint8_t newVal);
	void CheckZero(uint8_t value);
	void StoreValue(uint8_t value, bool updateZero);
	uint8_t SetCarry(bool val);
	uint16_t GetPCHFinalBits();
};
