/*
 *  This file is part of Signus: The Artefact Wars
 *  https://github.com/signus-game
 *
 *  Copyright (C) 2020-2021 Martin Doucha
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef MEMMAN_H_
#define MEMMAN_H_

#include <cstdio>
#include <inttypes.h>

class MemoryReadStream;

class ReadStream {
public:
	virtual int8_t readSint8(void);
	virtual uint8_t readUint8(void);

	virtual int16_t readSint16LE(void);
	virtual uint16_t readUint16LE(void);
	virtual int32_t readSint32LE(void);
	virtual uint32_t readUint32LE(void);
	virtual int64_t readSint64LE(void);
	virtual uint64_t readUint64LE(void);

	virtual int16_t readSint16BE(void);
	virtual uint16_t readUint16BE(void);
	virtual int32_t readSint32BE(void);
	virtual uint32_t readUint32BE(void);
	virtual int64_t readSint64BE(void);
	virtual uint64_t readUint64BE(void);

	virtual float readFloatX86(void);
	virtual double readDoubleX86(void);

	virtual size_t read(void *buf, size_t size) = 0;
	virtual char *readLine(char *buf, size_t size) = 0;

	virtual bool eos() const { return false; }

	virtual ~ReadStream() { }

	MemoryReadStream *readStream(size_t size);
};

class SeekableReadStream : public ReadStream {
public:
	virtual void seek(long offset, int whence) = 0;
	virtual long pos(void) const = 0;
	virtual long size(void) const = 0;

	~SeekableReadStream() { }
};

class MemoryReadStream : public SeekableReadStream {
private:
	unsigned char *_data;	// always add extra null byte at the end
	size_t _length, _pos;

public:
	MemoryReadStream(const void *ptr, size_t len);
	MemoryReadStream(const MemoryReadStream &src);
	~MemoryReadStream(void);

	const MemoryReadStream &operator=(const MemoryReadStream &src);

	size_t read(void *buf, size_t size);
	char *readLine(char *buf, size_t size);
	const char *readCString(void);
	bool eos(void) const;

	void seek(long offset, int whence);
	long pos(void) const { return _pos; }
	long size(void) const { return _length; }
	const void *dataPtr(void) const { return _data; }
};

class WriteStream {
public:
	virtual void writeSint8(int8_t data);
	virtual void writeUint8(uint8_t data);

	virtual void writeSint16LE(int16_t data);
	virtual void writeUint16LE(uint16_t data);
	virtual void writeSint32LE(int32_t data);
	virtual void writeUint32LE(uint32_t data);
	virtual void writeSint64LE(int64_t data);
	virtual void writeUint64LE(uint64_t data);

	virtual void writeSint16BE(int16_t data);
	virtual void writeUint16BE(uint16_t data);
	virtual void writeSint32BE(int32_t data);
	virtual void writeUint32BE(uint32_t data);
	virtual void writeSint64BE(int64_t data);
	virtual void writeUint64BE(uint64_t data);

	virtual void writeFloatX86(float data);
	virtual void writeDoubleX86(double data);

	virtual size_t write(const void *buf, size_t size) = 0;
	virtual size_t copy(ReadStream &stream, size_t size);
	virtual size_t copy(MemoryReadStream &stream, size_t size);

	virtual ~WriteStream() { }
};

class File : public SeekableReadStream, public WriteStream {
public:
	typedef enum {
		READ = 1,
		WRITE = 2,
		TRUNCATE = 4
	} OpenMode;

private:
	FILE *_file;
	char *_name;
	unsigned _mode;

	// Do not implement
	File(const File &src);
	const File &operator=(const File &src);
public:
	File(void);
	explicit File(const char *filename, unsigned mode = READ);
	~File(void);

	// inherited methods
	void seek(long offset, int whence);
	long pos(void) const;
	long size(void) const;
	size_t read(void *buf, size_t size);
	size_t write(const void *buf, size_t size);
	char *readLine(char *buf, size_t size);

	// new methods
	int open(const char *filename, unsigned mode = READ);
	void close(void);
	inline const char *getName(void) const { return _name; }
	inline bool isOpen(void) const { return _file; }
	bool eos() const;
};

class MemoryWriteStream : public WriteStream {
private:
	unsigned char *_data;
	size_t _size, _pos;

	// Do not implement
	MemoryWriteStream(const MemoryWriteStream &src);
	const MemoryWriteStream &operator=(const MemoryWriteStream &src);
public:
	MemoryWriteStream(size_t alloc = 32);
	~MemoryWriteStream(void);

	size_t write(const void *buf, size_t size);
	void *dataPtr(void) const { return _data; }
	size_t size(void) const { return _pos; }
};

class BitStream {
private:
	ReadStream &_stream;
	uint8_t _lastByte;
	size_t _bitsLeft;

public:
	BitStream(ReadStream &stream);
	~BitStream(void);

	unsigned readBitsLE(size_t bits);
	bool eos(void) const { return _stream.eos(); }
};

#endif
