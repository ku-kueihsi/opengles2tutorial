/*
 * assets_file_io.h
 *
 *  Created on: Oct 30, 2013
 *      Author: jesse
 */

#ifndef ASSETS_FILE_IO_H_
#define ASSETS_FILE_IO_H_


#include <stdio.h>
#include <android/asset_manager.h>

#ifdef __cplusplus
extern "C" {
#endif

/* hijack fopen and route it through the android asset system so that
we can pull things out of our packagesk APK */

void android_fopen_set_asset_manager(AAssetManager* manager);
FILE* android_fopen(const char* fname, const char* mode);

//#define fopen(name, mode) android_fopen(name, mode)

#ifdef __cplusplus
}
#endif


#endif /* ASSETS_FILE_IO_H_ */
