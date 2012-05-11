#ifndef __MEMORY_H__
#define __MEMORY_H__

class MemoryLocation
{
protected:
	MemoryLocation();

public:
	virtual operator uint8_t *() const = 0;
};

class MemoryLocationUnmapped : public MemoryLocation
{
private:
	uint8_t *valuePtr;

public:
	MemoryLocationUnmapped(uint8_t *address) : MemoryLocation()
	{
		valuePtr = address;
	}

	operator uint8_t *() const
	{
		return valuePtr;
	}
};

class MemoryLocationMapped : public MemoryLocation
{
private:
	Register<> *valuePtr;

public:
	MemoryLocationMapped(Register<> *address) : MemoryLocation()
	{
		valuePtr = address;
	}

	operator uint8_t *() const
	{
		return *valuePtr;
	}
};

#endif /*__MEMORY_H__*/
