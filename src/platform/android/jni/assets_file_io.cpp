#include <stdio.h>

#include "assets_file_io.h"
#include <errno.h>
#include <android/asset_manager.h>
#include <android/log.h>

//avoid warning about unused parameter
#define DUMMY(a) (void)(a)

static int android_read(void* cookie, char* buf, int size) {
	return AAsset_read((AAsset*)cookie, buf, size);
}

static int android_write(void* cookie, const char* buf, int size) {
	DUMMY(cookie);
	DUMMY(buf);
	DUMMY(size);
	return EACCES; // can't provide write access to the apk
}

static fpos_t android_seek(void* cookie, fpos_t offset, int whence) {
	return AAsset_seek((AAsset*)cookie, offset, whence);
}

static int android_close(void* cookie) {
	AAsset_close((AAsset*)cookie);
	return 0;
}

// must be established by someone else...
AAssetManager* android_asset_manager;
void android_fopen_set_asset_manager(AAssetManager* manager) {
	android_asset_manager = manager;
}

FILE* android_fopen(const char* fname, const char* mode) {
	if(mode[0] == 'w') return NULL;
	//  __android_log_print(ANDROID_LOG_VERBOSE, "test", "opening\n");
	AAsset* asset = AAssetManager_open(android_asset_manager, fname, 0);
	//  __android_log_print(ANDROID_LOG_VERBOSE, "test", "%d\n", asset);
	if(!asset) return NULL;
	return funopen(asset, android_read, android_write, android_seek, android_close);
}
