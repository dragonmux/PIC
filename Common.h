#include <string.h>
#include <inttypes.h>
#include <map>

class RegisterException
{
private:
	const char *Error;

public:
	RegisterException(const char *errorString) : Error(errorString)
	{
	}
};

template<typename T = uint8_t> class Register
{
private:
	T *contents;
	uint8_t bitsUsed;
	bool mayDelete;

public:
	Register(uint8_t bits = sizeof(T)) : bitsUsed(bits), mayDelete(true)
	{
		contents = new T();
	}

	Register(T *overlay) : contents(overlay), bitsUsed(sizeof(T)), mayDelete(false)
	{
	}

	~Register()
	{
		if (mayDelete)
			delete contents;
	}

	operator T() const
	{
		return *contents;
	}

	operator T*() const
	{
		return contents;
	}

	Register<T> &operator =(T b)
	{
		*contents = b;
		return *this;
	}

	Register<T> &operator +=(T b)
	{
		*contents += b;
		return *this;
	}

	Register<T> &operator |=(T b)
	{
		*contents |= b;
		return *this;
	}

	Register<T> &operator &=(T b)
	{
		*contents &= b;
		return *this;
	}

	Register<T> operator ^=(T b) const
	{
		*contents ^= b;
		return *this;
	}

	uint8_t GetHi()
	{
		throw new RegisterException("Cannot get high byte of 8-bit register");
	}

	uint8_t GetLo()
	{
		return *contents;
	}
};

template<> uint8_t Register<uint16_t>::GetHi()
{
	return ((*contents) >> 8) & 0xFF;
}

template<> uint8_t Register<uint16_t>::GetLo()
{
	return (*contents) & 0xFF;
}

class MemoryException
{
private:
	const char *Error;

public:
	MemoryException(const char *errorString) : Error(errorString)
	{
	}
};

class Memory
{
protected:
	Memory();

public:
	template<class T> static Memory *Generate();
	virtual uint8_t Dereference(uint8_t bank, uint8_t partialAddress) = 0;
	virtual uint8_t *Reference(uint8_t bank, uint8_t partialAddress) = 0;
	virtual uint8_t *operator [](uint32_t ref) const = 0;
};

class ROMException
{
public:
	ROMException(const char *errorMessage);
};

class ROM
{
private:
	char *Program;
	uint32_t maxAddress;

public:
	ROM(const char *program, uint32_t programLength);
	~ROM();
	uint16_t operator [](uint32_t ref) const;
};

template<typename T, uint8_t depth> class Stack
{
private:
	uint8_t top;
	T theStack[depth];

public:
	Stack()
	{
		memset(theStack, 0, depth * sizeof(T));
		top = 0;
	}

	void Push(T value)
	{
		theStack[top] = value;
		top++;
		top %= depth;
	}

	T Pop()
	{
		T value;
		if (top == 0)
			top = depth - 1;
		else
			top--;
		value = theStack[top];
		theStack[top] = 0;
		return value;
	}
};

// TODO: implement this properly
class CPUException
{
public:
	CPUException()
	{
	}
};

#define TRAP() \
{ \
	trapped = true; \
	throw new CPUException(); \
}
