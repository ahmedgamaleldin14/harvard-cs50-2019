# Questions

## What's `stdint.h`?

It is a header file in the C standard library which provides data types that specify different widths for each type.

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

Preserve the correct numerical values of the data types.
uint8_t is unsighned int of size 8.

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

BYTE = 1, DWROD = 4, LONG = 4, WORD = 2 

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

The first two bytes specify that the file is a bitmap or "BM". In Hex: 0x4d42

## What's the difference between `bfSize` and `biSize`?

bfSize is the size for the entire file in bytes.
biSize is the size for the BITMAPINFOHEADER in bytes.

## What does it mean if `biHeight` is negative?

The bitmap is top-bottom and its origin in the top-left corner.

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

biBitCount

## Why might `fopen` return `NULL` in `copy.c`?

OS might failt to open the file for many reasons.
Maybe it doesn't fine the file or there is not enough memory.

## Why is the third argument to `fread` always `1` in our code?

Because we read one byte at a time. (Not sure)

## What value does `copy.c` assign to `padding` if `bi.biWidth` is `3`?

int padding = (4 - (3 * 3 % 4) % 4;
padding is equal to 3.

## What does `fseek` do?

fseek moves the file position indicator.

## What is `SEEK_CUR`?

This moves the file indicator to a given position.
