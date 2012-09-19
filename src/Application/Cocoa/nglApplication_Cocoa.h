#ifndef __nglNSApplication_h__
#define __nglNSApplication_h__

#include "ngl.h"
#import <Cocoa/Cocoa.h>

@class nglUIWindow;

@interface nglNSApplication : NSApplication
{
}
- (void) dealloc;
- (BOOL) openURL: (NSURL*) pUrl;
- (void) sendEvent: (NSEvent*) pEvent;


@end//nglNSApplication

@interface nglNSApplicationDelegate : NSObject<NSApplicationDelegate>
{
}

- (void) applicationDidFinishLaunching:       (NSNotification *) aNotification;
- (void) applicationDidBecomeActive:          (NSNotification *) aNotification;
//- (void) applicationDidEnterBackground:         (NSApplication*) pUIApp;
//- (void) applicationDidReceiveMemoryWarning:  (NSApplication*) pUIApp;
//- (void) applicationSignificantTimeChange:    (NSApplication*) pUIApp;
- (void) applicationWillTerminate:            (NSNotification *) aNotification;

@end//nglNSApplicationDelegate


#endif//__nglNSApplication_h__
