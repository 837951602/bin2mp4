#include "ezpwd/rs"
#include <stdio.h>
const int density = 181;

int main(int argc, char** argv) {
    system ("ffmpeg\\ffmpeg.exe -y -i video.mp4 -f rawvideo -pix_fmt rgb24 tmp2.bin");
    FILE* ftmp = fopen("tmp2.bin", "rb");
    FILE* fout = fopen("pi_data2.zip", "wb+");
    FILE* fdbg = fopen("decode.log", "w");
    ezpwd::RS<255,density> coder;
    std::vector<uint8_t> data(255);
    int rb;
    while (1) {
        unsigned char rgb[255*24];
        rb = fread(rgb, 1, 255*24, ftmp);
        if (rb<255) break;
        for (int i=0; i<255; ++i) {
            int res = 0;
            for (int j=0; j<8; ++j)
                res |= (rgb[i*24+j*3]<128) << (7-j);
            data[i] = res;
        }
        int errs = coder.decode(data);
        fprintf(fdbg, "%d\n", errs);
        fwrite (data.data(), density, 1, fout);
        data.resize (255);
    }
    // TODO: shrink file
    fclose (fout);
    fclose (fdbg);
    fclose (ftmp);
}
