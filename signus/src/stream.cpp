#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cfloat>
#include <cassert>

#include "stream.h"

#if FLT_RADIX != 2
#error "FPU implementation not supported. FLT_RADIX must be equal to 2."
#endif

using std::isnan;

uint32_t float_to_x86(float num) {
	int ex;
	uint32_t mantissa, sign = 0;

	if (isnan(num)) {
		return (num < 0) ? 0xffc00000 : 0x7fc00000;
	}

	ex = ilogbf(num);

	if (num < 0) {
		sign = 1UL << 31;
		num = -num;
	}

	if (ex == FP_ILOGB0) {
		// num == 0
		return sign;
	} else if (ex > 0xfe) {
		// INFINITY (ex == INT_MAX) or x86 range overflow
		return sign | 0x7f800000;
	}

	mantissa = (uint32_t)scalbnf(num, 23 - ex);
	ex += 0x7f;

	if (ex <= 0) {
		// correct subnormal mantissa
		mantissa >>= 1 - ex;
		ex = 0;
	}

	mantissa &= 0x7fffff;
	return sign | (((unsigned)ex) << 23) | mantissa;
}

uint64_t double_to_x86(double num) {
	int ex;
	uint64_t mantissa, sign = 0;

	if (isnan(num)) {
		if (num < 0) {
			return uint64_t(0x1fff) << 51;
		} else {
			return uint64_t(0xfff) << 51;
		}
	}

	ex = ilogb(num);

	if (num < 0) {
		sign = uint64_t(1) << 63;
		num = -num;
	}

	if (ex == FP_ILOGB0) {
		// num == 0
		return sign;
	} else if (ex > 0x7fe) {
		// INFINITY (ex == INT_MAX) or x86 range overflow
		return sign | uint64_t(0x7ff) << 52;
	}

	mantissa = (uint64_t)scalbn(num, 52 - ex);
	ex += 0x3ff;

	if (ex <= 0) {
		// correct subnormal mantissa
		mantissa >>= 1 - ex;
		ex = 0;
	}

	mantissa &= ~(uint64_t(0xfff) << 52);
	return sign | (uint64_t(ex) << 52) | mantissa;
}

float float_from_x86(uint32_t bin) {
	uint32_t sign = bin >> 31, ex = (bin >> 23) & 0xff;
	uint32_t mantissa = bin & 0x7fffffUL;
	float ret;

	if (ex == 0xff) {
		if (mantissa) {
			return NAN;
		}

		return sign ? -INFINITY : INFINITY;
	} else if (ex) {
		// add the omitted highest bit to normal mantissa
		mantissa |= 1UL << 23;
	} else if (!mantissa) {
		return 0.0f;
	} else {
		// correct subnormal mantissa
		mantissa <<= 1;
	}

	ex -= 0x7f;
	ret = scalbnf(mantissa, ex - 23);
	return sign ? -ret : ret;
}

double double_from_x86(uint64_t bin) {
	uint64_t sign = bin >> 63, mantissa = bin & ~(uint64_t(0xfff) << 52);
	int ex = (bin >> 52) & 0x7ff;
	double ret;

	if (ex == 0x7ff) {
		if (mantissa) {
			return NAN;
		}

		return sign ? -INFINITY : INFINITY;
	} else if (ex) {
		// add the omitted highest bit to normal mantissa
		mantissa |= uint64_t(1) << 52;
	} else if (!mantissa) {
		return 0.0f;
	} else {
		// correct subnormal mantissa
		mantissa <<= 1;
	}

	ex -= 0x3ff;
	ret = scalbn(mantissa, ex - 52);
	return sign ? -ret : ret;
}

int8_t ReadStream::readSint8(void) {
	int8_t ret = 0;

	read(&ret, 1);
	return ret;
}

uint8_t ReadStream::readUint8(void) {
	uint8_t ret = 0;

	read(&ret, 1);
	return ret;
}

uint16_t ReadStream::readUint16LE(void) {
	uint16_t a = readUint8();
	uint16_t b = readUint8();

	return (b << 8) | a;
}

uint32_t ReadStream::readUint32LE(void) {
	uint32_t a = readUint16LE();
	uint32_t b = readUint16LE();

	return (b << 16) | a;
}

uint64_t ReadStream::readUint64LE(void) {
	uint64_t a = readUint32LE();
	uint64_t b = readUint32LE();

	return (b << 32) | a;
}

int16_t ReadStream::readSint16LE(void) {
	return (int16_t)readUint16LE();
}

int32_t ReadStream::readSint32LE(void) {
	return (int32_t)readUint32LE();
}

int64_t ReadStream::readSint64LE(void) {
	return (int64_t)readUint64LE();
}

uint16_t ReadStream::readUint16BE(void) {
	uint16_t a = readUint8();
	uint16_t b = readUint8();

	return (a << 8) | b;
}

uint32_t ReadStream::readUint32BE(void) {
	uint32_t a = readUint16BE();
	uint32_t b = readUint16BE();

	return (a << 16) | b;
}

uint64_t ReadStream::readUint64BE(void) {
	uint64_t a = readUint32BE();
	uint64_t b = readUint32BE();

	return (a << 32) | b;
}

int16_t ReadStream::readSint16BE(void) {
	return (int16_t)readUint16BE();
}

int32_t ReadStream::readSint32BE(void) {
	return (int32_t)readUint32BE();
}

int64_t ReadStream::readSint64BE(void) {
	return (int64_t)readUint64BE();
}

float ReadStream::readFloatX86(void) {
	return float_from_x86(readUint32LE());
}

double ReadStream::readDoubleX86(void) {
	return double_from_x86(readUint64LE());
}

MemoryReadStream *ReadStream::readStream(size_t size) {
	MemoryReadStream *ret;
	unsigned char *ptr = new unsigned char[size];

	size = read(ptr, size);
	ret = new MemoryReadStream(ptr, size);
	delete[] ptr;
	return ret;
}

MemoryReadStream::MemoryReadStream(const void *ptr, size_t len) : _data(NULL),
	_length(0), _pos(0) {

	_length = len;
	_data = new unsigned char[_length + 1];
	memcpy(_data, ptr, _length);
	_data[_length] = 0;
}

MemoryReadStream::MemoryReadStream(const MemoryReadStream &src) : _data(NULL),
	_length(src._length), _pos(src._pos) {

	_data = new unsigned char[_length + 1];
	memcpy(_data, src._data, _length);
	_data[_length] = 0;
}

MemoryReadStream::~MemoryReadStream(void) {
	delete[] _data;
}

const MemoryReadStream &MemoryReadStream::operator=(const MemoryReadStream &src) {
	MemoryReadStream tmp(src);
	size_t tmp1;
	unsigned char *ptr;

	ptr = _data;
	_data = tmp._data;
	tmp._data = ptr;

	tmp1 = _length;
	_length = tmp._length;
	tmp._length = tmp1;

	tmp1 = _pos;
	_pos = tmp._pos;
	tmp._pos = tmp1;

	return *this;
}

size_t MemoryReadStream::read(void *buf, size_t size) {
	size_t len;

	if (_pos >= _length) {
		_pos = _length + 1;
		return 0;
	}

	len = size < _length - _pos ? size : _length - _pos;
	memcpy(buf, _data + _pos, len);
	_pos = len < size ? _length + 1 : _pos + len;

	return len;
}

char *MemoryReadStream::readLine(char *buf, size_t size) {
	size_t i;

	if (_pos >= _length) {
		_pos = _length + 1;
		return NULL;
	}

	for (i = 0; i < size - 1 && i + _pos < _length; i++) {
		if (_data[_pos + i] == '\n') {
			i++;
			break;
		}
	}

	memcpy(buf, _data + _pos, i);
	_pos = _pos + i >= _length && i < size - 1 && buf[i-1] != '\n' ? _length + 1 : _pos + i;

	buf[i] = '\0';
	if (buf[i-1] == '\n' && buf[i-2] == '\r') {
		buf[i-2] = '\n';
		buf[i-1] = '\0';
	} else if (buf[i-1] == '\r' && _pos < _length && _data[_pos] == '\n') {
		buf[i-1] = '\n';
		_pos++;
	}

	return buf;
}

const char *MemoryReadStream::readCString(void) {
	const char *ptr = (char*)(_data + _pos);
	int len;

	if (_pos >= _length) {
		_pos = _length + 1;
		return NULL;
	}

	len = strlen(ptr) + 1;
	_pos += len;

	if (_pos > _length) {
		_pos = _length;
	}

	return ptr;
}

bool MemoryReadStream::eos(void) const {
	return _pos > _length;
}

void MemoryReadStream::seek(long offset, int whence) {
	switch (whence) {
	case SEEK_SET:
		if (offset >= 0 && size_t(offset) <= _length) {
			_pos = offset;
		}

		break;

	case SEEK_CUR:
		if (offset < 0 && _pos < (size_t)-offset) {
			break;
		}

		if (_pos + offset <= _length) {
			_pos += offset;
		}

		break;

	case SEEK_END:
		if (offset <= 0 && _length >= (size_t)-offset) {
			_pos = _length + offset;
		}
	}
}

void WriteStream::writeSint8(int8_t data) {
	write(&data, 1);
}

void WriteStream::writeUint8(uint8_t data) {
	write(&data, 1);
}

void WriteStream::writeUint16LE(uint16_t data) {
	writeUint8(data & 0xff);
	writeUint8(data >> 8);
}

void WriteStream::writeUint32LE(uint32_t data) {
	writeUint16LE(data & 0xffff);
	writeUint16LE(data >> 16);
}

void WriteStream::writeUint64LE(uint64_t data) {
	writeUint32LE(data & 0xffffffff);
	writeUint32LE(data >> 32);
}

void WriteStream::writeSint16LE(int16_t data) {
	writeUint16LE((uint16_t)data);
}

void WriteStream::writeSint32LE(int32_t data) {
	writeUint32LE((uint32_t)data);
}

void WriteStream::writeSint64LE(int64_t data) {
	writeUint64LE((uint64_t)data);
}

void WriteStream::writeUint16BE(uint16_t data) {
	writeUint8(data >> 8);
	writeUint8(data & 0xff);
}

void WriteStream::writeUint32BE(uint32_t data) {
	writeUint16BE(data >> 16);
	writeUint16BE(data & 0xffff);
}

void WriteStream::writeUint64BE(uint64_t data) {
	writeUint32BE(data >> 32);
	writeUint32BE(data & 0xffffffff);
}

void WriteStream::writeSint16BE(int16_t data) {
	writeUint16BE((uint16_t)data);
}

void WriteStream::writeSint32BE(int32_t data) {
	writeUint32BE((uint32_t)data);
}

void WriteStream::writeSint64BE(int64_t data) {
	writeUint64BE((uint64_t)data);
}

void WriteStream::writeFloatX86(float data) {
	return writeUint32LE(float_to_x86(data));
}

void WriteStream::writeDoubleX86(double data) {
	return writeUint64LE(double_to_x86(data));
}

size_t WriteStream::copy(ReadStream &stream, size_t size) {
	char *buf = NULL;
	size_t tmp, bufsize = 65536, block, written, total = 0;

	bufsize = size < bufsize ? size : bufsize;
	buf = new char[bufsize];

	while (total < size) {
		written = 0;
		block = size - total > bufsize ? bufsize : size - total;
		block = stream.read(buf, block);

		if (!block) {
			break;
		}

		while (written < block) {
			tmp = write(buf + written, block - written);

			if (!tmp) {
				// Remaining data in buffer will be lost
				// FIXME: Try seeking back if possible
				delete[] buf;
				return total + written;
			}

			total += tmp;
			written += tmp;
		}
	}

	delete[] buf;
	return total;
}

size_t WriteStream::copy(MemoryReadStream &stream, size_t size) {
	const char *buf = (const char*)stream.dataPtr();
	size_t tmp, pos, total, written = 0;

	pos = stream.pos();
	total = stream.size() - pos;
	buf += pos;

	if (size > total) {
		size = total;
	}

	while (written < size) {
		tmp = write(buf + written, size - written);

		if (!tmp) {
			break;
		}

		written += tmp;
	}

	return written;
}

File::File(void) : _file(NULL), _name(NULL), _mode(0) { }

File::File(const char *filename, unsigned mode) : _file(NULL), _name(NULL),
	_mode(0) {
	open(filename, mode);
}

File::~File(void) {
	close();
}

int File::open(const char *filename, unsigned mode) {
	static const char *modelist[] = {NULL, "r", "a", "r+", NULL, "w", "w+"};
	const char *modestr = NULL;

	close();

	if (mode >= 0 && mode < 8) {
		modestr = modelist[mode];
	}

	_file = fopen(filename, modestr);

	if (_file) {
		_name = new char[strlen(filename) + 1];
		strcpy(_name, filename);
		_mode = mode;

		if (mode == WRITE) {
			seek(0, SEEK_SET);
		}

		return 1;
	}

	return 0;
}

void File::close(void) {
	if (!_file) {
		return;
	}

	fclose(_file);
	delete[] _name;

	_file = NULL;
	_name = NULL;
	_mode = 0;
}

void File::seek(long offset, int whence) {
	assert(_file);
	fseek(_file, offset, whence);
}

long File::pos(void) const {
	assert(_file);
	return ftell(_file);
}

long File::size(void) const {
	long tmp, ret;
	assert(_file);

	tmp = ftell(_file);
	fseek(_file, 0, SEEK_END);
	ret = ftell(_file);
	fseek(_file, tmp, SEEK_SET);
	return ret;
}

size_t File::read(void *buf, size_t size) {
	assert(_file && (_mode & READ));
	return fread(buf, 1, size, _file);
}

size_t File::write(const void *buf, size_t size) {
	assert(_file && (_mode & WRITE));
	return fwrite(buf, 1, size, _file);
}

char *File::readLine(char *buf, size_t size) {
	int idx;
	assert(_file && (_mode & READ));

	if (!fgets(buf, size, _file)) {
		return NULL;
	}

	idx = strlen(buf);

	if (buf[idx-1] == '\n' && buf[idx-2] == '\r') {
		buf[idx-2] = '\n';
		buf[idx-1] = '\0';
	} else if (buf[idx-1] == '\r') {
		unsigned tmp = readUint8();

		if (eos()) {
			return buf;
		} else if (tmp == '\n') {
			buf[idx-1] = '\n';
		} else {
			seek(-1, SEEK_CUR);
		}
	}

	return buf;
}

bool File::eos() const {
	assert(_file);
	return feof(_file);
}

MemoryWriteStream::MemoryWriteStream(size_t alloc) :
	_data(new unsigned char[alloc + 1]), _size(alloc + 1), _pos(0) {

	memset(_data, 0, _size * sizeof(unsigned char));
}

MemoryWriteStream::~MemoryWriteStream(void) {
	delete[] _data;
}

size_t MemoryWriteStream::write(const void *buf, size_t size) {
	if (_pos + size >= _size) {
		unsigned char *tmp;

		_size = _pos + size > 2 * _size ? _pos + size : 2 * _size;
		tmp = new unsigned char[_size];
		memcpy(tmp, _data, _pos);
		memset(tmp + _pos + size, 0, _size - _pos - size);
		delete[] _data;
		_data = tmp;
	}

	memcpy(_data + _pos, buf, size);
	_pos += size;
	return size;
}

BitStream::BitStream(ReadStream &stream) : _stream(stream), _lastByte(0),
	_bitsLeft(0) {

}

BitStream::~BitStream(void) {

}

unsigned BitStream::readBitsLE(size_t bits) {
	unsigned ret = 0, shift = 0;
	static const unsigned mask[] = {0, 0x1, 0x3, 0x7, 0xf, 0x1f, 0x3f, 0x7f, 0xff};
	assert(bits <= 8 * sizeof(unsigned));

	if (_bitsLeft >= bits) {
		ret = _lastByte & mask[bits];
		_lastByte >>= bits;
		_bitsLeft -= bits;
		return ret;
	}

	ret = _lastByte;
	shift = _bitsLeft;
	bits -= shift;
	_lastByte = 0;
	_bitsLeft = 0;

	for (; bits >= 8; bits -= 8, shift += 8) {
		ret |= _stream.readUint8() << shift;
	}

	if (bits) {
		_lastByte = _stream.readUint8();
		_bitsLeft = 8 - bits;
		ret |= (_lastByte & mask[bits]) << shift;
		_lastByte >>= bits;
	}

	return ret;
}
