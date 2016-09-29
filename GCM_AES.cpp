#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <Windows.h>
BYTE R1[128] = { 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
void printArr(BYTE *X, int size){
	int i;
	for (i = 0; i < size; i++){
		printf("%02X ", X[i]);
	}
	printf("\n");
	printf("\n");
}
static BYTE SBox[256] = {

	0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5,
	0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
	0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0,
	0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
	0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC,
	0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
	0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A,
	0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
	0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0,
	0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
	0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B,
	0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
	0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85,
	0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
	0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5,
	0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
	0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17,
	0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
	0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88,
	0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
	0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C,
	0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
	0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9,
	0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
	0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6,
	0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
	0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E,
	0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
	0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94,
	0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
	0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68,
	0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16

};

BYTE GF(BYTE a, BYTE b){
	BYTE p = 0, i, c;

	for (i = 0; i < 8; i++) {
		if (b & 1)
			p ^= a;
		c = a & 0x80;
		a <<= 1;
		if (c)
			a ^= 0x1B;
		b >>= 1;
	}

	return (BYTE)p;
}

void Add(BYTE a[], BYTE b[], BYTE d[]){

	d[0] = a[0] ^ b[0];
	d[1] = a[1] ^ b[1];
	d[2] = a[2] ^ b[2];
	d[3] = a[3] ^ b[3];

}

void Multi(BYTE *a, BYTE *b, BYTE *d){

	d[0] = GF(a[0], b[0]) ^ GF(a[3], b[1]) ^ GF(a[2], b[2]) ^ GF(a[1], b[3]);
	d[1] = GF(a[1], b[0]) ^ GF(a[0], b[1]) ^ GF(a[3], b[2]) ^ GF(a[2], b[3]);
	d[2] = GF(a[2], b[0]) ^ GF(a[1], b[1]) ^ GF(a[0], b[2]) ^ GF(a[3], b[3]);
	d[3] = GF(a[3], b[0]) ^ GF(a[2], b[1]) ^ GF(a[1], b[2]) ^ GF(a[0], b[3]);

}

void SubBytes(BYTE *state) {

	BYTE i;
	for (i = 0; i < 16; i++) {
		state[i] = SBox[state[i]];

	}
}

void ShiftRows(BYTE *state) {

	int s = 0;
	BYTE tmp;

	for (int i = 1; i < 4; i++) {
		s = 0;
		while (s < i) {
			tmp = state[4 * i + 0];

			for (int k = 1; k < 4; k++) {
				state[4 * i + k - 1] = state[4 * i + k];
			}

			state[4 * i + 4 - 1] = tmp;
			s++;
		}
	}
}

void MixColums(BYTE *state) {

	BYTE a[] = { 0x02, 0x01, 0x01, 0x03 };
	BYTE col[4], res[4];
	int i = 0, j = 0;

	for (j = 0; j < 4; j++) {
		for (i = 0; i < 4; i++) {
			col[i] = state[4 * i + j];
		}

		Multi(a, col, res);

		for (i = 0; i < 4; i++) {
			state[4 * i + j] = res[i];
		}
	}
}

void AddRoundKey(BYTE *state, BYTE *w, int r) {

	for (int c = 0; c < 4; c++) {
		state[4 * 0 + c] = state[4 * 0 + c] ^ w[16 * r + 4 * c + 0];
		state[4 * 1 + c] = state[4 * 1 + c] ^ w[16 * r + 4 * c + 1];
		state[4 * 2 + c] = state[4 * 2 + c] ^ w[16 * r + 4 * c + 2];
		state[4 * 3 + c] = state[4 * 3 + c] ^ w[16 * r + 4 * c + 3];
	}
}


void Sub(BYTE *w) {

	for (int i = 0; i < 4; i++) {
		w[i] = SBox[16 * ((w[i] & 0xf0) >> 4) + (w[i] & 0x0f)];
	}
}

void Rot(BYTE *w) {

	BYTE tmp;

	tmp = w[0];
	for (int i = 0; i < 3; i++) {
		w[i] = w[i + 1];
	}
	w[3] = tmp;
}


BYTE R[] = { 0x02, 0x00, 0x00, 0x00 };

BYTE * Rcon(int i) {

	if (i == 1) {
		R[0] = 0x01;
	}
	else if (i > 1) {
		R[0] = 0x02;
		i--;
		while (i - 1 > 0) {
			R[0] = GF(R[0], 0x02);
			i--;
		}
	}
	return R;
}

void KeyExpansion(BYTE *key, BYTE *w) {

	BYTE tmp[4];
	int i;
	int len = 4 * (10 + 1);

	for (i = 0; i < 4; i++) {
		w[4 * i + 0] = key[4 * i + 0];
		w[4 * i + 1] = key[4 * i + 1];
		w[4 * i + 2] = key[4 * i + 2];
		w[4 * i + 3] = key[4 * i + 3];
	}

	for (i = 4; i < len; i++) {
		tmp[0] = w[4 * (i - 1) + 0];
		tmp[1] = w[4 * (i - 1) + 1];
		tmp[2] = w[4 * (i - 1) + 2];
		tmp[3] = w[4 * (i - 1) + 3];

		if (i % 4 == 0) {
			Rot(tmp);
			Sub(tmp);
			Add(tmp, Rcon(i / 4), tmp);

		}

		w[(4 * i) + 0] = w[4 * (i - 4) + 0] ^ tmp[0];
		w[(4 * i) + 1] = w[4 * (i - 4) + 1] ^ tmp[1];
		w[(4 * i) + 2] = w[4 * (i - 4) + 2] ^ tmp[2];
		w[(4 * i) + 3] = w[4 * (i - 4) + 3] ^ tmp[3];
	}
}

void Encrypt(BYTE *PlainText, BYTE *CipherText, BYTE *w) {

	BYTE state[4 * 4];
	int r, i, j;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			state[4 * i + j] = PlainText[i + 4 * j];
		}
	}

	AddRoundKey(state, w, 0);

	for (r = 1; r < 10; r++) {
		SubBytes(state);
		ShiftRows(state);
		MixColums(state);
		AddRoundKey(state, w, r);
	}
	SubBytes(state);
	ShiftRows(state);
	AddRoundKey(state, w, 10);

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			CipherText[i + 4 * j] = state[4 * i + j];
		}
	}
}
void cpystr(BYTE * X, BYTE *Y, int len){
	int i;
	for (i = 0; i < len; i++){
		X[i] = Y[i];
	}
}
void str2hex(char *str, BYTE *hex, int keylength){
	char tmp[2];
	int val[2];
	for (int i = 0; i < keylength; i++) {
		tmp[0] = str[i * 2];
		tmp[1] = str[i * 2 + 1];
		for (int j = 0; j < 2; j++) {
			if (tmp[j] >= '0' && tmp[j] <= '9') {
				val[j] = tmp[j] - '0';
			}
			else if (tmp[j] >= 'a' && tmp[j] <= 'f') {
				val[j] = tmp[j] - 'a' + 10;
			}
			else {
				val[j] = tmp[j] - 'A' + 10;
			}
		}
		hex[i] = (BYTE)(val[0] * 16 + val[1]);
	}
}
void Multiplication(BYTE *X, BYTE *Y){
	// X , Y 
	// Output = X
	int val = 0, pow = 128;
	BYTE x1[128], Z[128] = { 0x00 }, V[128];
	BYTE prv_Z[128] = { 0x00 }, prv_V[128];
	int i, j, k;
	for (i = 0; i < 16; i++){		// X denote
		for (j = 0; j < 8; j++){
			x1[i * 8 + (7 - j)] = X[i] % 2;
			X[i] = X[i] / 2;
		}
	}
	for (i = 0; i < 16; i++){		// Y denote
		for (j = 0; j < 8; j++){
			V[i * 8 + (7 - j)] = Y[i] % 2;
			Y[i] = Y[i] / 2;
		}
	}
	for (k = 0; k < 128; k++){
		prv_V[k] = V[k];
	}
	//최초의 prv_V , prv_Z는 Z0 , V0임
	//======================================== Complete↓
	for (i = 0; i < 128; i++){
		//calc Zi+1 
		if (x1[i] == 0){
			//zi+1 = zi
		}
		else{
			for (j = 0; j < 128; j++){
				Z[j] = prv_Z[j] ^ prv_V[j];
			}
		}
		//calc Vi+1 
		if (prv_V[127] == 0){
			V[0] = 0;
			for (j = 1; j < 128; j++){
				V[j] = prv_V[j - 1];
			}
		}
		else{
			V[0] = 0;
			for (j = 1; j < 128; j++){
				V[j] = prv_V[j - 1];
			}
			for (j = 0; j < 128; j++){
				V[j] = V[j] ^ R1[j];
			}
		}
		//옮김
		for (j = 0; j < 128; j++){
			prv_V[j] = V[j];
			prv_Z[j] = Z[j];
		}
	}
	//=======================================================
	//Z128 -> X로
	for (i = 0; i < 16; i++){
		for (j = 0; j < 8; j++){
			val += Z[i * 8 + j] * pow;
			pow /= 2;
		}
		X[i] = val;
		val = 0;
		pow = 128;
	}
}
void inc32(BYTE *X){  //complete
	int i;
	int xlen = 96;
	BYTE temp[12];
	BYTE tamp[4];
	unsigned int d = 0;
	for (i = 0; i < 12; i++){
		temp[i] = X[i];
	}

	for (i = 12; i < 16; i++){
		tamp[i - 12] = X[i];
	}

	for (i = 0; i < 4; i++){
		d += tamp[i] * pow((double)256, (3 - i));
	}
	d++;
	d = d % 0x100000000;

	for (i = 0; i < 4; i++){
		tamp[3 - i] = d % 256;
		d /= 256;
	}
	for (i = 0; i < 12; i++){
		X[i] = temp[i];
	}
	for (i = 12; i < 16; i++){
		X[i] = tamp[i - 12];
	}
}
void GCTR(int length, int n, BYTE *m, BYTE *Y, BYTE *ICB, BYTE *ekey,int fin){
	// hexlen , n , m ,Y (output) , ICB , ekey(AES key expansion) , fin : firstCTR 0 , lastCTR 1
	int i, j, k, nowlen;
	BYTE x1[16] = { 0, }, y1[16] = { 0, };
	BYTE ICB_temp[16] = { 0, }, CIPH[16] = { 0, };
	nowlen = length;
	for (i = 0; i < n; i++){
		if (nowlen >= 16){
			for (k = 0; k < 16; k++){
				x1[k] = 0;
				y1[k] = 0;
			}
			for (j = 0; j < 16; j++){
				x1[j] = m[i * 16 + j];
			}
			if (fin==0)
				inc32(ICB);  // ICB_temp -> ICB
			Encrypt(ICB, CIPH, ekey);
			for (k = 0; k < 16; k++){
				y1[k] = x1[k] ^ CIPH[k];
			}
			//printf("\n");
			for (k = 0; k < 16; k++){
				Y[i * 16 + k] = y1[k];
			}
			nowlen -= 16;
		}
		else if (nowlen >0 && nowlen < 16){
			for (k = 0; k < 16; k++){
				x1[k] = 0;
				y1[k] = 0;
			}
			for (j = 0; j < nowlen; j++){
				x1[j] = m[i * 16 + j];
			}
			printf("\n");
			inc32(ICB);  // ICB_temp -> ICB
			Encrypt(ICB, CIPH, ekey);
			for (k = 0; k < nowlen; k++){
				y1[k] = x1[k] ^ CIPH[k];
			}
			for (k = 0; k < nowlen; k++){
				Y[i * 16 + k] = y1[k];
			}
			printf("\n");
		}
	}
}
void GHASH(int length, BYTE *X1, BYTE *Y1,BYTE *ekey){
	int i, j, n;
	BYTE zero[16] = { 0, };
	BYTE tmpY[16] = { 0, };
	BYTE tamp[16];
	BYTE H[16] = { 0x66, 0xe9, 0x4b, 0xd4, 0xef, 0x8a, 0x2c, 0x3b, 0x88, 0x4c, 0xfa, 0x59, 0xca, 0x34, 0x2b, 0x2e };
	n = length / 16;
	if (length * 8 < 128 || (length * 8) % 128 != 0){
		printf("Wrong Input!!!\n");
		exit(1); // Failing Exit status
	}
	for (i = 0; i < n; i++){
		/* Step 1*/
		/* Step 2 ; 초기화 Y0 = 0^128임 */
		for (j = 0; j < 16; j++){
			tamp[j] = X1[i * 16 + j];
		}
		/* Step 3*/
		for (j = 0; j < 16; j++){
			Y1[j] = tmpY[j] ^ tamp[j];
		}
		Encrypt(zero,H,ekey);
		Multiplication(Y1, H);
		for (j = 0; j < 16; j++){
			tmpY[j] = Y1[j];
		}
	}
}
int makeN(int length){
	int n;
	if (length * 4 % 128 == 0)
		n = length * 4 / 128;
	else
		n = length * 4 / 128 + 1;

	return n;
}
void gerJ0(BYTE *iv, BYTE *j,BYTE *ekey, int n){
//IV j IVhexlen;
	int i = 0,s,IVlen,k=0;
	BYTE *ivtmp, lenIV[8];

	//printf("%d\n", strlen(iv));
	if (n == 12){
		for (i = 0; i < 12; i++)
			j[i] = iv[i];
		for (i = 12; i < 15; i++)
			j[i] = 0;
		j[15] = 1;
	}
	else{
		IVlen = n * 8; // IV 의 bitlen
		for (i = 0; i < 8; i++){
			lenIV[7 - i] = IVlen % 256;
			IVlen /= 256;
		}
		IVlen = n * 8;
		s = 128 * makeN(n*2) - IVlen;
		// n + 8*makeN(n*2) + 16;
		ivtmp = (BYTE*)malloc(sizeof(BYTE)*((IVlen + s + 64 + 64) / 8)+1);
		memset(ivtmp, 0, (IVlen + s + 64 + 64) / 8 + 1);
		for (i = 0; i < n; i++){
			ivtmp[i] = iv[i];
		}
		for (i = n; i < n + (s + 64)/8; i++){
			ivtmp[i] = 0x00;
		}
		for (i = n + (s + 64) / 8; i < n + (s / 8) + 8 + 8; i++){
			ivtmp[i] = lenIV[k];
			k++;
		}
		GHASH((IVlen + s + 64 + 64) / 8, ivtmp, j, ekey);
	}
}
int padding(BYTE *Y,BYTE *A, int C_size,int A_size){ //
	//size is hex size , need *8
	BYTE lenC[8]; BYTE lenA[8];
	BYTE *tamp;
	int len;
	int cBlen=C_size*8, aBlen=A_size*8;
	int max = 16, max2 = 0, i = 0, j = 0, k = 0,val;
	if (A == NULL)
		max2 = 0;
	else
		max2 = 16;

	while (C_size > max){
		max += 16;
	}
	while ((A_size > max2) && (A_size != 0)){
		max2 += 16;
	}
	tamp = (BYTE*)malloc(sizeof(BYTE)*(max + 3));
	//=======================================
	for (i = 0; i < 8; i++){
		lenC[7 - i] = cBlen % 256;
		cBlen /= 256;
	}								// [len(A)]64 || [len(C)]64
	for (i = 0; i < 8; i++){
		lenA[7 - i] = aBlen % 256;
		aBlen /= 256;
	}
	//=======================================C padding
	for (i = C_size; i < max; i++){
		Y[i] = 0x00;
	}
	for (i = 0; i < max; i++){
		tamp[i] = Y[i];
	}
	if (A == NULL){
		//======================================= [len(A)]64 || [len(C)]64
		for (i=max; i < max + 8; i++){
			Y[i] = lenA[j];
			j++;
		}
		for (i=max+8; i < max + 16; i++){
			Y[i] = lenC[k];
			k++;
		}
		val = max + 16;
	}
	else{
		//=================================== A padding
		for (i = 0; i < A_size; i++){
			Y[i] = A[i];
		}
		for (i=A_size; i < max2; i++){
			Y[i] = 0x00;
		}
		//==================================== C padding
		for (i=max2; i < max2 + max; i++){
			Y[i] = tamp[k];
			k++;
		}
		k = 0; j = 0;
		//===================================== lenC lenA
		for (i=max2+max; i < max2 + max + 8;i++){
			Y[i] = lenA[j];
			j++;
		}
		for (i=max2+max+8; i < max2 + max + 16; i++){
			Y[i] = lenC[k];
			k++;
		}
		val = max2 + max + 16;
	}
	return val;
}


int main(int argc, char *argv[]) {
	// case3 ; d9 31 32 25 f8 84 06 e5 a5 59 09 c5 af f5 26 9a 86 a7 a9 53 15 34 f7 da 2e 4c 30 3d 8a 31 8a 72 1c 3c 0c 95 95 68 09 53 2f cf 0e 24 49 a6 b5 25 b1 6a ed f5 aa 0d e6 57 ba 63 7b 39 1a af d2 55
	//case 4 : d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a721c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b39
	//0xfe , 0xed, 0xfa ,0xce ,0xde ,0xad ,0xbe ,0xef ,0xfe ,0xed ,0xfa ,0xce ,0xde ,0xad ,0xbe ,0xef ,0xab ,0xad ,0xda ,0xd2
	//feedfacedeadbeeffeedfacedeadbeefabaddad2
	BYTE * X; BYTE * Y; BYTE *m = 0; 
	BYTE *A=0;
	char *tamp = 0, *tamp1 = 0, *tamp2 = 0,ch;
	int n, length = 0, hexlen, nowlen = 0, A_len = 0, A_hexlen = 0, iv_len = 0, iv_hexlen = 0;;
	int len;
	BYTE J0[16] = { 0, }, ICB[16] = { 0. };
	//======================================>>>>>>>>>>>>>IV<<<<<<<<<<<<<==========================================================
	//BYTE IV[12] = { 0, }; // 1
	//BYTE IV[12] = { 0xca, 0xfe, 0xba, 0xbe, 0xfa, 0xce, 0xdb, 0xad, 0xde, 0xca, 0xf8, 0x88 }; // 2
	//cafebabefacedbaddecaf888
	//cafebabefacedbad
	BYTE *IV = 0;
	//======================================>>>>>>>>>>>>>KEY<<<<<<<<<<<<==========================================================
	//BYTE key[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };//1
	BYTE key[16] = { 0xfe, 0xff, 0xe9, 0x92, 0x86, 0x65, 0x73, 0x1c, 0x6d, 0x6a, 0x8f, 0x94, 0x67, 0x30, 0x83, 0x08 };//2
	BYTE ekey[4 * 44] = { 0, };
	//=================================================
	printf("Insert Plaintext\n");
	while ((ch = getchar()) != '\n')
	{
		if ((ch == ' ') || (ch == '\t') || (ch == '\n'))
			continue;
		tamp = (char *)realloc(tamp, sizeof(char)*(length + 1));
		tamp[length] = ch;
		if ((ch == ' ') || (ch == '\t') || (ch == '\n'))
			continue;
		tamp = (char *)realloc(tamp, sizeof(char)*(length + 1));
		tamp[length] = ch;
		length++;
	}
	m = (BYTE *)realloc(tamp, sizeof(BYTE)*(length + 1));
	str2hex(tamp, m, length);
	hexlen = length / 2;
	n = makeN(length);
	fflush(stdin);
	printf("\n");
	//================================================
	printf("Insert Authenticated data\n");
	while ((ch = getchar()) != '\n')
	{
		if ((ch == ' ') || (ch == '\t') || (ch == '\n'))
			continue;
		tamp1 = (char *)realloc(tamp1, sizeof(char)*(A_len + 1));
		tamp1[A_len] = ch;
		A_len++;
	}
	A = (BYTE *)realloc(tamp1, sizeof(BYTE)*(A_len + 1));
	str2hex(tamp1, A, A_len);
	A_hexlen = A_len / 2;
	printf("\n");
	//=================================================
	printf("Insert IV\n");
	while ((ch = getchar()) != '\n')
	{
		if ((ch == ' ') || (ch == '\t') || (ch == '\n'))
			continue;
		tamp2 = (char *)realloc(tamp2, sizeof(char)*(iv_len + 1));
		tamp2[iv_len] = ch;
		iv_len++;
	}
	IV = (BYTE *)realloc(tamp2, sizeof(BYTE)*(iv_len + 1));
	str2hex(tamp2, IV, iv_len);
	iv_hexlen = iv_len / 2;
	printf("\n");
	//=================================================

	Y = (BYTE *)realloc(tamp, sizeof(BYTE)*length + A_len+1);
	X = (BYTE *)realloc(tamp1, sizeof(BYTE)*length + A_len + 1);
	
	KeyExpansion(key, ekey);
	//========================================================//
	gerJ0(IV, J0, ekey, iv_hexlen);   //IV -> J0
	cpystr(ICB, J0, 16);  // ICB = J0 카피함
	GCTR(hexlen, n, m, Y, ICB, ekey,0); // GCTR돌림 output Y
	printf("Output C : \n");
	printArr(Y, hexlen);
	len = padding(Y, A,hexlen,A_hexlen);   // Test Vector 2에 고정됨
	GHASH(len, Y, X,ekey);  //32 80
	GCTR(16, 1, X, Y, J0, ekey,1); // GCTR돌림 output Y
	printf("Output T : \n");
	printArr(Y, 16);
	return 0;
}
/*
	160929
	IV 가 96bit가 아닐때 오답출력
	m에 IV가 들어가는 이상한 현상;;
	*/