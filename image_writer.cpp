/*
    Intention Repeater Image Writer v1.0
    Created 3/25/2022 by Anthro Teacher (AnthroHeart) aka Thomas Sweet.
*/

#include <iostream>
#include <bitset>
using namespace std;

#define WIDTH 1024
#define HEIGHT 768

void drawbmp();

int redcount[WIDTH][HEIGHT];

string TextToBinaryString(string words)
{
    string binaryString = "";
    for (char &_char : words)
    {
        binaryString += bitset<8>(_char).to_string();
    }
    return binaryString;
}
int main()
{
    string testText = "", bitString = "", BinaryString = "", intention = "";
    int h, w;
    long long c = 0;

    cout << "Intention Repeater Image Writer v1.0" << endl;
    cout << "by Anthro Teacher (AnthroHeart) aka Thomas Sweet." << endl
         << endl;

    cout << "Intention: ";
    std::getline(std::cin, intention);

    bitString = TextToBinaryString(intention);

    do
    {
        BinaryString += bitString;
    } while (BinaryString.length() < 1024 * 768);

    for (h = 0; h < 768; h++)
    {
        for (w = 0; w < 1024; w++)
        {
            c++;
            if (BinaryString.substr(c - 1, 1) == "0")
            {
                redcount[w][h] = 0;
            }
            else
            {
                redcount[w][h] = 255;
            }
        }
    }

    drawbmp();

    cout << "Output.bmp Written." << endl;
    return 0;
}

void drawbmp()
{

    unsigned int headers[13];
    FILE *outfile;
    int extrabytes;
    int paddedsize;
    int x;
    int y;
    int n;
    int red, green, blue;

    extrabytes = 4 - ((WIDTH * 3) % 4); // How many bytes of padding to add to each
    // horizontal line - the size of which must
    // be a multiple of 4 bytes.
    if (extrabytes == 4)
        extrabytes = 0;

    paddedsize = ((WIDTH * 3) + extrabytes) * HEIGHT;

    // Headers...
    // Note that the "BM" identifier in bytes 0 and 1 is NOT included in these "headers".

    headers[0] = paddedsize + 54; // bfSize (whole file size)
    headers[1] = 0;               // bfReserved (both)
    headers[2] = 54;              // bfOffbits
    headers[3] = 40;              // biSize
    headers[4] = WIDTH;           // biWidth
    headers[5] = HEIGHT;          // biHeight

    // Would have biPlanes and biBitCount in position 6, but they're shorts.
    // It's easier to write them out separately (see below) than pretend
    // they're a single int, especially with endian issues...

    headers[7] = 0;          // biCompression
    headers[8] = paddedsize; // biSizeImage
    headers[9] = 0;          // biXPelsPerMeter
    headers[10] = 0;         // biYPelsPerMeter
    headers[11] = 0;         // biClrUsed
    headers[12] = 0;         // biClrImportant

    outfile = fopen("Output.bmp", "wb");

    //
    // Headers begin...
    // When printing ints and shorts, we write out 1 character at a time to avoid endian issues.
    //

    fprintf(outfile, "BM");

    for (n = 0; n <= 5; n++)
    {
        fprintf(outfile, "%c", headers[n] & 0x000000FF);
        fprintf(outfile, "%c", (headers[n] & 0x0000FF00) >> 8);
        fprintf(outfile, "%c", (headers[n] & 0x00FF0000) >> 16);
        fprintf(outfile, "%c", (headers[n] & (unsigned int)0xFF000000) >> 24);
    }

    // These next 4 characters are for the biPlanes and biBitCount fields.

    fprintf(outfile, "%c", 1);
    fprintf(outfile, "%c", 0);
    fprintf(outfile, "%c", 24);
    fprintf(outfile, "%c", 0);

    for (n = 7; n <= 12; n++)
    {
        fprintf(outfile, "%c", headers[n] & 0x000000FF);
        fprintf(outfile, "%c", (headers[n] & 0x0000FF00) >> 8);
        fprintf(outfile, "%c", (headers[n] & 0x00FF0000) >> 16);
        fprintf(outfile, "%c", (headers[n] & (unsigned int)0xFF000000) >> 24);
    }

    //
    // Headers done, now write the data...
    //

    for (y = HEIGHT - 1; y >= 0; y--) // BMP image format is written from bottom to top...
    {
        for (x = 0; x <= WIDTH - 1; x++)
        {
            red = redcount[x][y];
            green = redcount[x][y];
            blue = redcount[x][y];

            red = red;
            green = green;
            blue = blue;

            // Also, it's written in (b,g,r) format...

            fprintf(outfile, "%c", blue);
            fprintf(outfile, "%c", green);
            fprintf(outfile, "%c", red);
        }
    }

    fclose(outfile);
    return;
}