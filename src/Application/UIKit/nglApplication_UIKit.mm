#include "nui.h"

using namespace std;

#include "nglApplication_UIKit.h"

#include "../../Window/UIKit/nglWindow_UIKit.h"
#include "nglWindow.h"

#import <UIKit/UIKit.h>

/*
** nglUIApplication
*/

void objCCallOnInit(void* pUIApplication);
void objCCallOnInitWithURL(void* pUIApplication, const nglString &url);
void objCCallOnOpenURL(const nglString &url);
void objCCallOnExit(int code);
void objCCallOnWillExit();
void objCCallOnActivation();
void objCCallOnDeactivation();
void objCCallOnMemoryWarning();
/*
@implementation nglUIApplication

- (void) dealloc
{
//  NGL_OUT(_T("[nglUIApplication dealloc]\n"));
  [super dealloc];
}

- (BOOL) openURL: (NSURL*) pUrl
{
  //NGL_OUT(_T("[nglUIApplication openURL]\n"));
  return [super openURL: pUrl];
}

- (void) sendEvent: (UIEvent*) pEvent
{
//NGL_DEBUG( NGL_OUT(_T("[nglUIApplication sendEvent]\n")) );
  [super sendEvent: pEvent];
}

- (void) didReceiveMemoryWarning
{
  printf("[nglUIApplication didReceiveMemoryWarning]\n");
  [super didReceiveMemoryWarning];
}

@end///< nglUIApplication
*/

/*
** nglUIApplicationDelegate
*/
@implementation nglUIApplicationDelegate


- (void) dealloc
{
  //App->TimedPrint("nglUIApplicationDelegate dealloc");
  //NGL_OUT(_T("[nglUIApplicationDelegate dealloc]\n"));
  [super dealloc];
}

- (void) applicationDidFinishLaunching:       (UIApplication*) pUIApplication
{
  //App->TimedPrint("nglUIApplicationDelegate applicationDidFinishLaunching");
  NGL_OUT(_T("[nglUIApplicationDelegate applicationDidFinishLaunching]\n"));
  NGL_ASSERT(App);

  objCCallOnInit(pUIApplication);
}

- (BOOL)application:(UIApplication *)pUIApplication didFinishLaunchingWithOptions:(NSDictionary *)launchOptions;
{
  //App->TimedPrint("nglUIApplicationDelegate didFinishLaunchingWithOptions");
	NGL_ASSERT(App);

	NSURL *launchURL = [launchOptions objectForKey:UIApplicationLaunchOptionsURLKey];	

  	if(launchURL)
  	{
  		NSString *urlstr = [launchURL absoluteString];
  		
  		objCCallOnInitWithURL(pUIApplication, nglString ((CFStringRef)urlstr));
  	} else {
  		
  		objCCallOnInit(pUIApplication);
  	}
    

}

- (BOOL) application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation
{
    NSString *urlstr;
    
    if([url isFileURL])
        urlstr = [url path];
    else
        urlstr = [url absoluteString];
    
    objCCallOnOpenURL(nglString ((CFStringRef)urlstr));
}

- (void) applicationDidBecomeActive:          (UIApplication*) pUIApplication
{
  //App->TimedPrint("nglUIApplicationDelegate applicationDidBecomeActive");
  NGL_DEBUG( NGL_OUT(_T("[nglUIApplicationDelegate applicationDidBecomeActive]\n")); )
  NGL_ASSERT(App);

  objCCallOnActivation();
  
	NSEnumerator *e = [[pUIApplication windows] objectEnumerator];
	
	id win;
	while ((win = [e nextObject]) )
  {
		if ([win respondsToSelector: @selector(getNGLWindow)] )
		{
			nglWindow* pWindow = [win getNGLWindow];
			
			NGL_ASSERT(pWindow);
			pWindow->CallOnActivation();			
		}
	}	
    

}

- (void) applicationDidEnterBackground:       (UIApplication*) pUIApplication
{
  //App->TimedPrint("nglUIApplicationDelegate applicationDidEnterBackground");
  NGL_DEBUG( NGL_OUT(_T("[nglUIApplicationDelegate applicationDidEnterBackground]\n")); )
  NGL_ASSERT(App);

	NSEnumerator *e = [[pUIApplication windows] objectEnumerator];
	
	id win;
	while ((win = [e nextObject]))
  {
    if ([win respondsToSelector: @selector(getNGLWindow)])
		{
			nglWindow* pWindow = [win getNGLWindow];
			
			NGL_ASSERT(pWindow);
			pWindow->CallOnDesactivation();
		}
	}
	
  objCCallOnDeactivation();
}

- (void) applicationDidReceiveMemoryWarning:  (UIApplication*) pUIApplication
{
  //App->TimedPrint("nglUIApplicationDelegate applicationDidReceiveMemoryWarning");
	NGL_ASSERT(App);
  objCCallOnMemoryWarning();
}

- (void) applicationSignificantTimeChange:    (UIApplication*) pUIApplication
{
  //App->TimedPrint("nglUIApplicationDelegate applicationSignificantTimeChange");
//NGL_OUT(_T("[nglUIApplicationDelegate applicationSignificantTimeChange]\n"));
}

- (void) applicationWillTerminate:            (UIApplication*) pUIApplication
{
  //App->TimedPrint("nglUIApplicationDelegate applicationWillTerminate");
//	NGL_DEBUG( NGL_OUT(_T("[nglUIApplicationDelegate applicationWillTerminate]\n")) );

	objCCallOnWillExit();

	NSEnumerator *e = [[pUIApplication windows] objectEnumerator];
	
	id win;
	while ((win = [e nextObject]))
    {
		[win release];
	}	
 

///< advise the kernel we're quiting
  objCCallOnExit(0);
}

@end///< nglUIApplicationDelegate

/*
** nglApplication
*/

// #define NGL_APP_ENONE  0 (in nglApplicationBase.h)

const nglChar* gpApplicationErrorTable[] =
{
/*  0 */ _T("No error"),
  NULL
};

nglApplication::nglApplication()
{
  mExitPosted = false;
  mUseIdle = false;
//  mIdleTimer = NULL;

// nglApplication is a kernel's client, just as plugin instances
  IncRef();
}

nglApplication::~nglApplication()
{
  SetIdle(false);
}


/*
 * Public methods
 */

void nglApplication::Quit (int Code)
{
  mExitPosted = true;
  
  CallOnWillExit();
  CallOnExit(Code);
  exit(Code);
}


/*
 * Internals
 */

/* Startup
 */

int nglApplication::Main(int argc, const char** argv)
{
  NSAutoreleasePool *pPool = [NSAutoreleasePool new];
  {
    //App->TimedPrint("nglApplication::Main Init");

    Init(argc, argv);
  }

  //App->TimedPrint("nglApplication::Main UIApplication");
  UIApplicationMain(argc, const_cast<char**>(argv), nil, @"nglUIApplicationDelegate");

  [pPool release];

  return 0;
}


bool nglApplication::Init(int ArgCnt, const char** pArg)
{
  int i;

//Fetch application's name (App->mName) from argv[0]
  nglString arg0(pArg[0]);
  nglString Name = arg0;

//Only keep file name if it's a full path
  i = Name.Find ('/');
  if (i != -1)
    Name = Name.Extract (i + 1, Name.GetLength());
  SetName(Name);

  // Store user args in mArgs
  for (i = 1; i < ArgCnt; i++)
    AddArg( nglString(pArg[i]) );

  return true;
}


/* Event management / main loop
 */

///////////////////////////////////////////////
/*
nglApplication::nglApplication(nglApplication* pApp);
nglApplication::~nglApplication();
void nglApplication::Quit (int Code);
void nglApplication::MakeMenu();
void nglApplication::DoMenuCommand(long menuResult);
int nglApplication::Run();
void nglApplication::OnEvent(int Flags);
void nglApplication::AddEvent(nglEvent* pEvent);
void nglApplication::DelEvent(nglEvent* pEvent);
bool nglApplication::AddTimer(nglTimer* pTimer);
bool nglApplication::DelTimer(nglTimer* pTimer);
bool nglApplication::AddWindow (nglWindow* pWin);
bool nglApplication::DelWindow (nglWindow* pWin);
void nglApplication::DoEvent(EventRecord *event);
OSErr nglApplication::QuitAppleEventHandler( const AppleEvent *appleEvt, AppleEvent* reply, UInt32 refcon );
pascal void TimerAction (EventLoopTimerRef  theTimer, void* userData);
nglString nglApplication::GetClipboard();
bool nglApplication::SetClipboard(const nglString& rString);
*/
