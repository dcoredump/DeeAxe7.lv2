#include <stdio.h>
#include <string.h>

char epiano[] = {
  95, 29, 20, 50, 99, 95, 0, 0, 41, 0, 19, 0, 115, 24, 79, 2, 0,
  95, 20, 20, 50, 99, 95, 0, 0, 0, 0, 0, 0, 3, 0, 99, 2, 0,
  95, 29, 20, 50, 99, 95, 0, 0, 0, 0, 0, 0, 59, 24, 89, 2, 0,
  95, 20, 20, 50, 99, 95, 0, 0, 0, 0, 0, 0, 59, 8, 99, 2, 0,
  95, 50, 35, 78, 99, 75, 0, 0, 0, 0, 0, 0, 59, 28, 58, 28, 0,
  96, 25, 25, 67, 99, 75, 0, 0, 0, 0, 0, 0, 83, 8, 99, 2, 0,
  
  94, 67, 95, 60, 50, 50, 50, 50, 4, 6, 34, 33, 0, 0, 56, 24,
  69, 46, 80, 73, 65, 78, 79, 32, 49, 32
};

void UnpackPatch(const char bulk[128], char patch[156]) {
        for (int op = 0; op < 6; op++) {
                // eg rate and level, brk pt, depth, scaling
                memcpy(patch + op * 21, bulk + op * 17, 11);
                char leftrightcurves = bulk[op * 17 + 11];
                patch[op * 21 + 11] = leftrightcurves & 3;
                patch[op * 21 + 12] = (leftrightcurves >> 2) & 3;
                char detune_rs = bulk[op * 17 + 12];
                patch[op * 21 + 13] = detune_rs & 7;
                patch[op * 21 + 20] = detune_rs >> 3;
                char kvs_ams = bulk[op * 17 + 13];
                patch[op * 21 + 14] = kvs_ams & 3;
                patch[op * 21 + 15] = kvs_ams >> 2;
                patch[op * 21 + 16] = bulk[op * 17 + 14];  // output level
                char fcoarse_mode = bulk[op * 17 + 15];
                patch[op * 21 + 17] = fcoarse_mode & 1;
                patch[op * 21 + 18] = fcoarse_mode >> 1;
                patch[op * 21 + 19] = bulk[op * 17 + 16];  // fine freq
        }
        memcpy(patch + 126, bulk + 102, 9);  // pitch env, algo
        char oks_fb = bulk[111];
        patch[135] = oks_fb & 7;
        patch[136] = oks_fb >> 3;
        memcpy(patch + 137, bulk + 112, 4);  // lfo
        char lpms_lfw_lks = bulk[116];
        patch[141] = lpms_lfw_lks & 1;
        patch[142] = (lpms_lfw_lks >> 1) & 7;
        patch[143] = lpms_lfw_lks >> 4;
        memcpy(patch + 144, bulk + 117, 11);  // transpose, name
        patch[155] = 0x3f;  // operator on/off
}

void print_patch_data(const char patch[156])
{
  printf("\n-------------------------------------------------------------------------------\n");
  // OP rates and levels
  printf("                                                            A\n");
  printf("  O                              R                        V M\n");
  printf("  P  R1 L1  R2 L2  R3 L3  R4 L4  S  M FC FF  D Freq.  OL  S S\n");

  for (int op = 5; op >=0; --op)
  {
    int off = op * 21;

    printf("  %1d",6-op);
    for (int i = 0; i < 4; i++)
    {
      printf("  %02d %02d",(int)patch[off + i],(int)patch[off + 4 + i]);
    }
    printf("  %1d  %1d %2d %2d %+1d        %1d  %1d %1d\n",(int)patch[off + 13],(int)patch[off + 17],(int)patch[off + 18],(int)patch[off + 19], (int)patch[off +20]-7, (int)patch[off +16], (int)patch[off + 15], (int)patch[off + 14]);
  }

  printf("  P");
  for (int i = 0; i < 4; i++)
  {
    printf("  %02d %02d",(int)patch[126 + i],(int)patch[130 + i]);
  }

  printf("\n\n                                          Alg.  %2d\n",(int)patch[134]);
  printf("      Left   BkPt  Right     Speed %2d     Fdbk   %1d\n",(int)patch[137],(int)patch[135]);
  for(int op=5;op>=0;--op)
  {
    int off = op * 21;
    switch(6-op)
    {
      case 1:
        printf("  %1d   %1d %02d   %02d     %1d %02d     Delay %2d     C3=   %2d\n",5-op ,(int)patch[off + 11],(int)patch[off + 9],(int)patch[off + 8],(int)patch[off + 12],(int)patch[off + 10],(int)patch[138],(int)patch[144]);
        break;
      case 2:
        printf("  %1d   %1d %02d   %02d     %1d %02d     PMD   %2d     OKS    %1d\n",5-op ,(int)patch[off + 11],(int)patch[off + 9],(int)patch[off + 8],(int)patch[off + 12],(int)patch[off + 10],(int)patch[139],(int)patch[136]);
        break;
      case 3:
        printf("  %1d   %1d %02d   %02d     %1d %02d     AMD   %2d\n",5-op, (int)patch[off + 11],(int)patch[off + 9],(int)patch[off + 8],(int)patch[off + 12],(int)patch[off + 10],(int)patch[140]);
        break;
      case 4:
        printf("  %1d   %1d %02d   %02d     %1d %02d     Sync   %1d\n",5-op ,(int)patch[off + 11],(int)patch[off + 9],(int)patch[off + 8],(int)patch[off + 12],(int)patch[off + 10],(int)patch[141]);
        break;
      case 5:
        printf("  %1d   %1d %02d   %02d     %1d %02d     Wave   %1d\n",5-op ,(int)patch[off + 11],(int)patch[off + 9],(int)patch[off + 8],(int)patch[off + 12],(int)patch[off + 10],(int)patch[142]);
        break;
      case 6:
        printf("  %1d   %1d %02d   %02d     %1d %02d     PMS    %1d\n",5-op, (int)patch[off + 11],(int)patch[off + 9],(int)patch[off + 8],(int)patch[off + 12],(int)patch[off + 10],(int)patch[143]);
        break;
    }
  }
  printf("Ready\n");
}

int main(int argc, char **argv)
{
	char patch[56];

	UnpackPatch(epiano,patch);
	print_patch_data(patch);

	printf(".\n");
	return(0);
}

