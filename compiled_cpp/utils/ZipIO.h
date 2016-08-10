#ifndef ZIP_IO_H
#define ZIP_IO_H

#include <minizip/zip.h>
#include <minizip/unzip.h>
#include <cstdint>
#include <cstdlib>
#include <string>

unzFile open_unzip(const std::string filename);
unsigned get_nfiles_in_unzip(unzFile ar);
unsigned get_current_file_size(unzFile ar);
void read_current_file(unzFile ar, void* buffer, unsigned bytes);


// When you know the file contains one array and it has an exact size
template<typename Array>
void unzip_to_sarray(std::string filename, Array &buf) {
  unzFile ar = open_unzip(filename);
  unsigned fsize = get_current_file_size(ar);
  assert(fsize == sizeof(buf.data));

  read_current_file(ar, (void*)buf.ptr(), fsize);
}

//------------------------------------------------------------------------
// Writes a buffer to a new file with name [fname] inside the 
// zip archive [zf].
//------------------------------------------------------------------------
void write_buffer_to_zip(
    zipFile zf,         // zip archive handle
    std::string fname,  // name of file instead archive to write
    void* buf,          // buffer of data
    unsigned len        // how many bytes to write
    );

#endif
