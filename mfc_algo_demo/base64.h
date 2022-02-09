#pragma once


namespace myBase64 {

	const char myBase64Table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
	const char myBase64Map[256] =
	{
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 62, 255, 255, 255, 63,
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 255, 255,
		255, 254, 255, 255, 255, 0, 1, 2, 3, 4, 5, 6,
		7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
		19, 20, 21, 22, 23, 24, 25, 255, 255, 255, 255, 255,
		255, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36,
		37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
		49, 50, 51, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255
	};

	int EncodeBase64(const unsigned char* pSrc, unsigned char* pDst, int nSrcLen)
	{
		if (nullptr == pSrc || 0 == nSrcLen)
			return -1;

		unsigned char c1, c2, c3;
		unsigned int nDstLen = 0;
		unsigned int nDiv = nSrcLen / 3;
		unsigned int nMod = nSrcLen % 3;

		for (int i = 0; i < nDiv; i++)
		{
			c1 = *pSrc++;
			c2 = *pSrc++;
			c3 = *pSrc++;

			*pDst++ = myBase64Table[c1 >> 2];
			*pDst++ = myBase64Table[((c1 << 4) | c2 >> 4) & 0x3f];
			*pDst++ = myBase64Table[((c2 << 2) | c3 >> 6) & 0x3f];
			*pDst++ = myBase64Table[c3 & 0x3f];
			nDstLen += 4;
		}

		if (1 == nMod)
		{
			*pDst++ = myBase64Table[(*pSrc) >> 2];
			*pDst++ = myBase64Table[((*pSrc) << 4) & 0x3f];
			*pDst++ = '=';
			*pDst++ = '=';
			nDstLen += 4;
		}
		else if (2 == nMod)
		{
			c1 = *pSrc++;
			c2 = *pSrc++;

			*pDst++ = myBase64Table[c1 >> 2];
			*pDst++ = myBase64Table[((c1 << 4) | c2 >> 4) & 0x3f];
			*pDst++ = myBase64Table[c2 >> 2];
			*pDst++ = '=';
			nDstLen += 4;
		}

		*pDst = '\0';
		return nDstLen;
	}

	int DecodeBase64(const unsigned char* pSrc, unsigned char* pDst, int nSrcLen)
	{
		if (nullptr == pSrc || 0 == nSrcLen)
			return -1;

		unsigned int nDstLen = 0;
		unsigned char pc; // now char ptr
		unsigned long ncode = 0;
		unsigned int dc = 0; //'=' count
		unsigned int nc = 0; // get char count
		for (int i = 0; i < nSrcLen; i++)
		{
			pc = myBase64Map[*pSrc++];

			if (pc == 0xff)continue;
			if (pc == 0xfe) { pc = 0; dc++; }

			ncode = (ncode << 6) | pc;

			if (++nc == 4)
			{
				*pDst++ = (unsigned char)(ncode >> 16 & 0xff);
				nDstLen++;
				if (dc < 2)
				{
					*pDst++ = (unsigned char)((ncode >> 8) & 0xff);
					nDstLen++;
				}
				if (dc < 1)
				{
					*pDst++ = (unsigned char)(ncode & 0xff);
					nDstLen++;
				}
				nc = 0;
				ncode = 0;
			}
		}
		return nDstLen;
	}


}
