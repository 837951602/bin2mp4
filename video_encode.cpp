#include "ezpwd/rs"
#include <stdio.h>
const int density = 181;
char zeros[1048576];
int main(int argc, char** argv) {
    FILE* fin = fopen("input.zip", "rb");
    FILE* ftmp = fopen("tmp.bin", "wb");
    ezpwd::RS<255,density> coder;
    std::vector<uint8_t> data(density);
    int rb;
    do {
        rb = fread(data.data(),1,density,fin);
        if (rb < density) {
            data[rb] = -1;
            for (int i=rb; ++i<density; ) data[i]=0;
        }
        coder.encode(data);
        fwrite (data.data(), 255, 1, ftmp);
        data.resize (density);
    } while (rb==density);
    fwrite (zeros, 320*240/8-1, 1, ftmp);
    fclose (fin);
    fclose (ftmp);
    system ("ffmpeg\\ffmpeg.exe -y -f rawvideo -s 320x240 -r 10 -pix_fmt monow -i tmp.bin -bufsize 15M -maxrate 5M -b:v 5M video.mp4");
}
