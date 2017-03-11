#include <stdio.h>
#include <sys/stat.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define CHR "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0b\x0c\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2a\x2b\x2c\x2d\x2e\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3b\x3c\x3d\x3e\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e\x4f\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5a\x5b\x5c\x5d\x5e\x5f\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6a\x6b\x6c\x6d\x6e\x6f\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7a\x7b\x7c\x7d\x7e\x7f"

int main(int argc, char* argv[])
{
	int bufsize = sizeof(CHR);
	printf("[+] Length of allowed Chars: %d bytes\n",bufsize);
	printf("[+]Encoding for this character set:\n");
	printf("01-02-03-04-05-06-07-08-09-0b-0c-0e-0f-10-11-12-13-14-15-16-17-18-19-1a-1b-1c-1d-1e-1f-20-21-22-23-24-25-26-27-28-29-2a-2b-2c-2d-2e-30-31-32-33-34-35-36-37-38-39-3b-3c-3d-3e-41-42-43-44-45-46-47-48-49-4a-4b-4c-4d-4e-4f-50-51-52-53-54-55-56-57-58-59-5a-5b-5c-5d-5e-5f-60-61-62-63-64-65-66-67-68-69-6a-6b-6c-6d-6e-6f-70-71-72-73-74-75-76-77-78-79-7a-7b-7c-7d-7e-7f\n\n");
	printf("[+] Allocating Memory...");
	unsigned int targ, last, t[4], l[4];
	unsigned int shot, single, carry = 0;
	int len, a, i, j, k, m, z, flag = 0;
	char word[3][4];
	unsigned char mem[bufsize + 3];
	if (argc < 2) {
		printf("[!] Usage: %s <EAX starting value> <EAX end value>\n", argv[0]);
		exit(1);
	}
	srand(time(NULL));
	memset(mem,0,bufsize+3);
	strcpy(mem, CHR);
	len = strlen(mem);
	strfry(mem); // Randomize
	last = strtoul(argv[1], NULL, 0);
	targ = strtoul(argv[2], NULL, 0);
	printf("[+] Calculating printable values to subtract from EAX..\n\n");
	t[3] = (targ & 0xff000000) >> 24; // Splitting by bytes
	t[2] = (targ & 0x00ff0000) >> 16;
	t[1] = (targ & 0x0000ff00) >> 8;
	t[0] = (targ & 0x000000ff);
	l[3] = (last & 0xff000000) >> 24;
	l[2] = (last & 0x00ff0000) >> 16;
	l[1] = (last & 0x0000ff00) >> 8;
	l[0] = (last & 0x000000ff);
	for (a = 1; a < 5; a++)
	{ // Value count
		carry = flag = 0;
		for (z = 0; z < 4; z++)
		{ // Byte count
			for (i = 0; i < len; i++)
			{
				for (j = 0; j < len; j++)
				{
					for (k = 0; k < len; k++)
					{
						for (m = 0; m < len; m++)
						{
							if (a < 2) j = len + 1;
							if (a < 3) k = len + 1;
							if (a < 4) m = len + 1;
							shot = t[z] + carry + mem[i] + mem[j] + mem[k] + mem[m];
							single = (shot & 0x000000ff);
							if (single == l[z])
							{
								carry = (shot & 0x0000ff00) >> 8;
								if (i < len) word[0][z] = mem[i];
								if (j < len) word[1][z] = mem[j];
								if (k < len) word[2][z] = mem[k];
								if (m < len) word[3][z] = mem[m];
								i = j = k = m = len + 2;
								flag++;
							}
						}
					}
				}
			}
		}
		if (flag == 4) { // If all 4 bytes found
			printf("[*] Encoded!\nStart: 0x%08x\n\n", last);
			for (i = 0; i < a; i++)
				printf("     - 0x%08x\n", *((unsigned int *)word[i]));
			printf("-------------------\n");
			printf("End:   0x%08x\n", targ);
			exit(0);
		}
	}
}
