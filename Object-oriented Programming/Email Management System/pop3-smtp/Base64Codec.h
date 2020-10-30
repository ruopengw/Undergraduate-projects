#if !defined(L_BASE64_CODEC_h)
#define L_BASE64_CODEC_h

inline unsigned int Base64EncLen(unsigned int L, unsigned char blocks_in_line=80/4-1)
{
	unsigned short line_size = (blocks_in_line+1)*4;

	unsigned int EncCharCount = (L/3+(L%3?1:0))*4;
	EncCharCount+=2*(EncCharCount/line_size+(EncCharCount%line_size?1:0)-1);
	return EncCharCount;
}

inline unsigned int Base64DecLen(unsigned int L, unsigned char blocks_in_line=80/4-1)
{
	unsigned short line_size = (blocks_in_line+1)*4;
	L-=2*(L/line_size + (L%line_size?1:0)-1);
	unsigned int EncCharCount = (L/4+(L%4?1:0))*3;
	return EncCharCount;
}

unsigned int Base64Enc(const void* in, int in_len, char* out, unsigned char blocks_in_line=80/4-1);

unsigned int Base64InplaceEnc(void* inout, int in_len, unsigned char blocks_in_line=80/4-1);

unsigned int Base64InplaceDec(char* inout, unsigned int in_len);


#endif //L_BASE64_CODEC_h
