#include <string>
#include <lz4.h>
#include <fstream>
#include <vector>
#include <print>
#include <optional>

static void die (std::string msg) {
    std::print ("{}\n", msg);
    exit (1);
}

static std::vector<unsigned char> readfile (const std::string& path) {
    std::ifstream file (path, std::ios::binary);
    if (!file) die ("Can't open file.\n");
    file.seekg (0, std::ios::end);
    std::streamsize size = file.tellg ();
    file.seekg (0, std::ios::beg);

    if (size < 12) die ("File is too small");
    std::vector<unsigned char> mem (size);
    if (!file.read (reinterpret_cast<char*> (mem.data ()), size)) die ("Failed to read file");
    return mem;
}
int writefile (const std::string& path, std::vector<char>& mem,  const std::optional<int> og_filesize = std::nullopt) {
    std::ofstream file (path, std::ios::binary);
    if (!file) die ("Can't open file.\n");
    if (og_filesize) {
        char header[] = "mozLz40";
        if (!file.write (header, 8)) die ("Failed to write header");
        int size = og_filesize.value();
        if (!file.write (reinterpret_cast<char*> (&size), 4)) die ("Failed to write size");
    }
    if (!file.write (reinterpret_cast<char*> (mem.data ()), mem.size ())) die ("Failed to write blob");
    return mem.size ();
}

int main (int argc, char** argv) {

    if (argc != 4) die ("Too few arguments, usage: d|e infile outfile");
    if (argv[1][0] == 'd') {
        std::vector readbuf = readfile (argv[2]);
        int expected_size = *reinterpret_cast<int*> (&readbuf[8]);
        std::vector<char> writebuf (expected_size * 4);
        int result = LZ4_decompress_safe (reinterpret_cast<const char*> (&readbuf[12]), &writebuf[0],
                                          readbuf.size () - 12, writebuf.size ());
        writebuf.resize (result);

        writefile (argv[3], writebuf);
        exit (0);
    }
    if (argv[1][0] == 'e') {
        std::vector readbuf = readfile (argv[2]);
        int inputSize = readbuf.size ();
        int maxout = LZ4_compressBound (inputSize);
        std::vector<char> writebuf (maxout);
        int actual_size
            = LZ4_compress_default (reinterpret_cast<const char*> (&readbuf[0]), &writebuf[0], inputSize, maxout);
        if (actual_size <= 0) die ("compression failed\n");
        writebuf.resize (actual_size);

        writefile (argv[3], writebuf, inputSize);
        exit (0);
    }
    die ("Unknown command, usage: d|e infile outfile");
}
