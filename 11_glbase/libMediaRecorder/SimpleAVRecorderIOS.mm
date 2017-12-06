#if defined(__APPLE__)

#include <string>
#include <stdio.h>
#include <sys/stat.h>

#import <AVFoundation/AVFoundation.h>
#import <OpenGLES/ES2/gl.h>
#import <AVKit/AVKit.h>
#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#import <ReplayKit/ReplayKit.h>
#import <QuartzCore/QuartzCore.h>

#include "SimpleAVRecorderIOS.h"

@interface RPKDelegate : NSObject <RPScreenRecorderDelegate, RPPreviewViewControllerDelegate>
@property (assign, nonatomic) cocos2d::SimpleAVRecorderIOS* recorder;
@end
@implementation RPKDelegate
+(RPKDelegate*) sharedInstance {
	static id sharedInstance = nil;
	@synchronized(self) {
		if (!sharedInstance)
			sharedInstance = [[RPKDelegate alloc] init];
		return sharedInstance;
	}
}
#pragma mark - RPScreenRecorderDelegate

- (void)screenRecorder:(RPScreenRecorder *)screenRecorder didStopRecordingWithError:(NSError *)error previewViewController:(nullable RPPreviewViewController *)previewViewController {
	// handle error which caused unexpected stop of recording
	NSLog(@"$$$$$$$$$$$$$$$$$$$$ RPKDelegate::didStopRecordingWithError");
}

- (void)screenRecorder:(RPScreenRecorder *)screenRecorder didStopRecordingWithPreviewViewController:(nullable RPPreviewViewController *)previewViewController error:(nullable NSError *)error {
	// handle error which caused unexpected stop of recording
	NSLog(@"#################### RPKDelegate::didStopRecordingWithPreviewViewController");
}

- (void)screenRecorderDidChangeAvailability:(RPScreenRecorder *)screenRecorder {
	// handle screen recorder availability changes
	NSLog(@"//////////////////// RPKDelegate::screenRecorderDidChangeAvailability");
}

- (void)previewControllerDidFinish:(RPPreviewViewController *)previewController {
	NSLog(@"++++++++++++++++++++ RPKDelegate::previewControllerDidFinish");
	[previewController removeFromParentViewController];
	[previewController.view removeFromSuperview ];
	if(self.recorder)
		self.recorder->Complete();
}

@end

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

namespace cocos2d {

SimpleAVRecorderIOS::SimpleAVRecorderIOS()
 : m_recording(false)
 , m_event_cb(NULL)
{
}

int SimpleAVRecorderIOS::Init(const EVENT_FUNC& event_cb)
{
	m_event_cb   = event_cb;
	[RPKDelegate sharedInstance].recorder = this;
	[RPScreenRecorder sharedRecorder].delegate = [RPKDelegate sharedInstance];
	[[RPScreenRecorder sharedRecorder] setCameraEnabled:NO];
	[[RPScreenRecorder sharedRecorder] setMicrophoneEnabled:NO];
	[[RPScreenRecorder sharedRecorder] startRecordingWithHandler:^(NSError* error)
	{
		if(error)
		{
			NSLog(@"GLVideoRecorderIOS::Init:: startRecordingWithHandler :: %@", error);
			return;
		}
		this->m_recording = true;
		if(m_event_cb)
			m_event_cb(this, AVRECORDER_EVENT_START);
	}];
	return 0;
}

void SimpleAVRecorderIOS::Complete()
{
	NSLog(@"SimpleAVRecorderIOS::Complete::finishWritingWithCompletionHandler");
	if(m_event_cb)
		m_event_cb(this, AVRECORDER_EVENT_COMPLETE);
}

int SimpleAVRecorderIOS::Stop()
{
	[[RPScreenRecorder sharedRecorder] stopRecordingWithHandler:^(RPPreviewViewController* previewViewController, NSError* error)
	{
		if(error)
		{
			NSLog(@"GLVideoRecorderIOS::Init:: startRecordingWithHandler :: %@", error);
			return;
		}
		m_recording = false;
		if(m_event_cb)
			m_event_cb(this, AVRECORDER_EVENT_STOP);

		if(!previewViewController)
			return;

		previewViewController.previewControllerDelegate = [RPKDelegate sharedInstance];
		UIViewController * rootView = [[UIApplication sharedApplication] windows][0].rootViewController;
		previewViewController.view.frame = CGRectMake(	rootView.view.bounds.size.width*1/5, rootView.view.bounds.size.height*1/5
													  , rootView.view.bounds.size.width*3/5, rootView.view.bounds.size.height*3/5);
		[previewViewController willMoveToParentViewController:rootView];
		[rootView addChildViewController:previewViewController];
		[rootView.view addSubview:previewViewController.view];
	}];
	return 0;
}

}// namespace cocos2d

#endif // defined(__APPLE__)

