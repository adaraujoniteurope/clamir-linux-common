
#ifndef PF_FILE_H
#define PF_FILE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/* Filenames used by p-net stack. A filename may not be longer than
 * PNET_MAX_FILENAME_SIZE (termination included).
 */
#define PF_FILENAME_IP          "pnet_data_ip.bin"
#define PF_FILENAME_IM          "pnet_data_im.bin"
#define PF_FILENAME_DIAGNOSTICS "pnet_data_diagnostics.bin"
#define PF_FILENAME_PDPORT_1    "pnet_data_pdport_1.bin"
#define PF_FILENAME_PDPORT_2    "pnet_data_pdport_2.bin"
#define PF_FILENAME_PDPORT_3    "pnet_data_pdport_3.bin"
#define PF_FILENAME_PDPORT_4    "pnet_data_pdport_4.bin"

/**
 * Load a binary file, and verify the file version.
 *
 * @param directory        In:    Directory for files. Terminated string. NULL
 *                                or empty string is interpreted as current
 *                                directory.
 * @param filename         In:    File name. Terminated string.
 * @param p_object         Out:   Struct to load
 * @param size             In:    Size of struct to load
 * @return  0  if the operation succeeded.
 *          -1 if not found or an error occurred (for example wrong version).
 */
int pf_file_load (
   const char * directory,
   const char * filename,
   void * p_object,
   size_t size);

/**
 * Save a binary file, and include version information.
 *
 * @param directory        In:    Directory for files. Terminated string. NULL
 *                                or empty string is interpreted as current
 *                                directory.
 * @param filename         In:    File name. Terminated string.
 * @param p_object         In:    Struct to save
 * @param size             In:    Size of struct to save
 * @return  0  if the operation succeeded.
 *          -1 if an error occurred.
 */
int pf_file_save (
   const char * directory,
   const char * filename,
   const void * p_object,
   size_t size);

/**
 * Save a binary file if modified, and include version information.
 *
 * No saving is done if the content would be the same. This reduces the flash
 * memory wear.
 *
 * @param directory        In:    Directory for files. Terminated string. NULL
 *                                or empty string is interpreted as current
 *                                directory.
 * @param filename         In:    File name. Terminated string.
 * @param p_object         In:    Struct to save
 * @param p_tempobject     Temp:  Temporary buffer (of same size as object) for
 *                                loading existing file.
 * @param size             In:    Size of struct to save
 * @return  2 First saving of file (no previous file with correct version found)
 *          1 Updated file
 *          0  No storing required (no changes)
 *          -1 if an error occurred.
 */
int pf_file_save_if_modified (
   const char * directory,
   const char * filename,
   const void * p_object,
   void * p_tempobject,
   size_t size);

/**
 * Clear a binary file.
 *
 * @param directory        In:    Directory for files. Terminated string. NULL
 *                                or empty string is interpreted as current
 *                                directory.
 * @param filename         In:    File name. Terminated string.
 */
void pf_file_clear (const char * directory, const char * filename);

/************ Internal functions, made available for unit testing ************/

int pf_file_join_directory_filename (
   const char * directory,
   const char * filename,
   char * fullpath,
   size_t size);

#ifdef __cplusplus
}
#endif

#endif /* PF_FILE_H */