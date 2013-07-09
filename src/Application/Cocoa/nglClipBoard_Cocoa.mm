/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nglKernel.h"
#include "nglLog.h"
#include "nglClipBoard.h"

#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>

void nglClipBoard::Clear()
{
    NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
    [pasteboard clearContents];
}

void nglClipBoard::SetData(const nglMimeSource &src)
{
  Clear();
    
    if(src.GetMimeType() == 0)
    {
        NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
        NSString *str = [NSString stringWithCString: (const char *)src.GetData() encoding:NSUTF8StringEncoding];
        NSArray *copiedObjects = [NSArray arrayWithObject: str];
        [pasteboard writeObjects:copiedObjects];
    }
    

}

bool nglClipBoard::IsAvailable(const nglMimeSource::nglMimeDataType &mimeType)
{
    if(mimeType == 0)
    {
        
        NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
        NSArray *classArray = [NSArray arrayWithObject:[NSString class]];
        NSDictionary *options = [NSDictionary dictionary];
        
        BOOL ok = [pasteboard canReadObjectForClasses:classArray options:options];
        if (ok) {
            return true;
        }
    }
    
    
  return false;
}

void nglClipBoard::GetData(nglMimeSource &dst)
{
    if(dst.GetMimeType() == 0)
    {
        NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
        NSArray *classArray = [NSArray arrayWithObject:[NSString class]];
        NSDictionary *options = [NSDictionary dictionary];
        
        BOOL ok = [pasteboard canReadObjectForClasses:classArray options:options];
        if (ok) {
            NSArray *objectsToPaste = [pasteboard readObjectsForClasses:classArray options:options];
            NSString *str = [objectsToPaste objectAtIndex:0];
            
            const char *cstr = [str UTF8String];
            
            dst.SetData((uint8 *)cstr, strlen(cstr));
        }
    }
}

