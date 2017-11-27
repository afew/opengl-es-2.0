#ifdef __APPLE__
#import <Foundation/Foundation.h>

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "app_util.h"

const char* AppResourcePath(const char* fileName)
{
	NSString *nstrFileName = [NSString stringWithUTF8String:fileName];
	NSString *rs_path = [[[NSBundle mainBundle] resourcePath] stringByAppendingString:@"/"];
	NSString* fullPath = [rs_path stringByAppendingString:nstrFileName];
	return [fullPath UTF8String];
}

const char *GetBundleFileName( const char *fileName )
{
	NSString* fileNameNS = [NSString stringWithUTF8String:fileName];
	NSString* baseName = [fileNameNS stringByDeletingPathExtension];
	NSString* extension = [fileNameNS pathExtension];
	NSString *path = [[NSBundle mainBundle] pathForResource: baseName ofType: extension ];
	fileName = [path cStringUsingEncoding:1];
	
	return fileName;
}
#endif


