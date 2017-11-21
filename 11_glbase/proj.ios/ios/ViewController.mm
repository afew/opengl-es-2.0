
#import "ViewController.h"

@interface ViewController ()
{
}
@property (strong, nonatomic) EAGLContext* context;
@property (assign, nonatomic) bool         initialized;
@property (assign, nonatomic) float        touch_scale;
@property (assign, nonatomic) float        mainScreenW;
@property (assign, nonatomic) float        mainScreenH;
@end

extern int app_init();
extern int app_destroy();
extern int app_update();
extern int app_draw();

@implementation ViewController

- (void)viewDidLoad
{
	[super viewDidLoad];
	self.initialized = false;
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    if(!self.context)
    {
		NSLog(@"[E]:+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
		NSLog(@"[E]: ogles failed");
		NSLog(@"[E]:+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
		return;
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    [EAGLContext setCurrentContext:self.context];
}

- (void)dealloc
{
	app_destroy();
    [EAGLContext setCurrentContext:self.context];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
    [super dealloc];
}

- (void)didReceiveMemoryWarning
{
	[super didReceiveMemoryWarning];

	if ([self isViewLoaded] && ([[self view] window] == nil))
	{
		self.view = nil;
		[EAGLContext setCurrentContext:self.context];
	
		if ([EAGLContext currentContext] == self.context)
		{
			[EAGLContext setCurrentContext:nil];
		}
		self.context = nil;
	}
}

- (void)envInit
{
	NSLog(@"[D]:=======================================================================================================");
	self.initialized = true;
	float vpt[8]={};
	glGetFloatv(GL_VIEWPORT, vpt);
	
	NSString* sys_name            = [[UIDevice currentDevice] name ];			// e.g. "My iPhone"
	NSString* sys_model           = [[UIDevice currentDevice] model];			// e.g. @"iPhone", @"iPod touch"
	NSString* sys_localizedModel  = [[UIDevice currentDevice] localizedModel];	// localized version of model
	NSString* sys_systemName      = [[UIDevice currentDevice] systemName    ];	// e.g. @"iOS"
	NSString* sys_verison         = [[UIDevice currentDevice] systemVersion ];	// e.g. @"4.0"
	
	UIViewController* root = [UIApplication sharedApplication].keyWindow.rootViewController;
	NSArray*  narr_doc     = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString* nstr_doc     = [narr_doc objectAtIndex:0];
	NSString* nstr_bundle  = [[NSBundle mainBundle] resourcePath];
	
	const char* str_doc    = [nstr_doc UTF8String ];
	const char* str_bundle = [nstr_bundle UTF8String ];
	
	UIInterfaceOrientation orientation = [UIApplication sharedApplication].statusBarOrientation;
	if( [sys_verison compare : @"8.0" options:NSNumericSearch] == NSOrderedDescending)
	{
		NSLog(@"[D]:= sys_verison compare: is 8.0");
		
		float  nativeW  = [[UIScreen mainScreen] nativeBounds].size.width;
		float  nativeH  = [[UIScreen mainScreen] nativeBounds].size.height;
		float  nativeS  = (float)[[UIScreen mainScreen] nativeScale ];
		
		self.mainScreenW = nativeW;
		self.mainScreenH = nativeH;
		self.touch_scale = nativeS;
	}
	else
	{
		NSLog(@"[D]:= sys_verison compare: is not 8.0");
		CGFloat scale = [UIScreen mainScreen].scale;
		self.mainScreenW = [[UIScreen mainScreen] bounds].size.width  * scale;
		self.mainScreenH = [[UIScreen mainScreen] bounds].size.height * scale;
		self.touch_scale = scale;
	}
	
	if(UIInterfaceOrientationIsLandscape(orientation))
	{
		NSLog(@"[D]:= Is Landscape");
		float w = self.mainScreenW;
		float h = self.mainScreenH;
		std::swap(w, h);
		self.mainScreenW = w;
		self.mainScreenH = h;
	}
	else
	{
		NSLog(@"[D]:= Is not Landscape");
	}

	NSLog(@"[D]:=");
	NSLog(@"[D]:= doc   : %s", str_doc);
	NSLog(@"[D]:= bundle: %s", str_bundle);
	NSLog(@"[D]:= name : %@", sys_name);
	NSLog(@"[D]:= model: %@", sys_model);
	NSLog(@"[D]:= localized model: %@", sys_localizedModel);
	NSLog(@"[D]:= system name: %@", sys_systemName);
	NSLog(@"[D]:= verison: %@", sys_verison);
	
	NSLog(@"[D]:= document path: %@", nstr_doc);
	NSLog(@"[D]:= bundle path: %@", nstr_bundle);
	NSLog(@"[D]:= device screen, touch_scale [W,H,S] = %.f %.f %.5f", self.mainScreenW, self.mainScreenH, self.touch_scale);
	NSLog(@"[D]:= opengl viewport : %.f  %.f  %.f  %.f", vpt[0], vpt[1], vpt[2], vpt[3]);
	NSLog(@"[D]:=======================================================================================================");
	
	//NSString* bundleIdentifier = [[NSBundle mainBundle] bundleIdentifier];
	
	[UIApplication sharedApplication].idleTimerDisabled = YES;
}

- (void)update
{
	if(![self initialized])
	{
		[self envInit];
		
		if(0>app_init())
		{
			NSLog(@"[E]:+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
			NSLog(@"[E]: app_init failed");
			NSLog(@"[E]:+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
			
		}
		return;
	}
	
	app_update();
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
	[view bindDrawable];
	if([self initialized])
	{
		app_draw();
	}
}

@end
