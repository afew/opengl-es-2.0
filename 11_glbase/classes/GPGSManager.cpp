#if defined(ANDROID) || defined(__ANDROID__)
#if defined(_GPGS_)

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <unistd.h>
#include <android/bitmap.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include "app_util.h"
#include "GPGSManager.h"


#define DEBUG_TAG "CGPGS"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, DEBUG_TAG, __VA_ARGS__))

bool GPGSManager::isSignedIn = false;
std::unique_ptr<gpg::GameServices> GPGSManager::gameServices;
gpg::TurnBasedMatch GPGSManager::current_match_;

/*
 * Callback: Authentication started
 */
void OnAuthActionStarted(gpg::AuthOperation op) {
	LOGI("GPGS :: OnAuthActionStarted.");
	switch ( op ) {
		case gpg::AuthOperation::SIGN_IN:
			LOGI("GPGS :: Signing in to GPG.");
			break;
		case gpg::AuthOperation::SIGN_OUT:
			LOGI("GPGS :: Signing out from GPG.");
			break;
	}
}

/*
 * Callback: Authentication finishes
 */
void OnAuthActionFinished(gpg::AuthOperation op, gpg::AuthStatus status) {
	LOGI("GPGS :: OnAuthActionFinished.");
	if (status == gpg::AuthStatus::VALID){
		LOGI("GPGS :: Signing In.");
	}
	else{
		LOGI("GPGS :: Signing Out.");
	}
}

/*
 * Get GameServices
 */
gpg::GameServices *GPGSManager::GetGameServices() {
	return gameServices.get();
}

/*
 * Start Authorization
 */
void GPGSManager::BeginUserInitiatedSignIn() {
	if (!gameServices->IsAuthorized()) {
		LOGI("GPGS :: StartAuthorizationUI.");
		gameServices->StartAuthorizationUI();
	}
}

/*
 * Sign Out
 */
void GPGSManager::SignOut() {
	if (gameServices && gameServices->IsAuthorized()) {
		LOGI("GPGS :: SignOut.");
		gameServices->SignOut();
	}
}

/*
 * Unlock specified achievement
 */
void GPGSManager::UnlockAchievement(const char *achievementId) {
	if (gameServices && gameServices->IsAuthorized()) {
		LOGI("GPGS :: Achievement unlocked.");
		gameServices->Achievements().Unlock(achievementId);
	}
}

/*
 * Increase specified achievemet
 */
void GPGSManager::IncrementAchievement(const char *achievementId, uint32_t steps/* = 1*/)
{
	if (gameServices && gameServices->IsAuthorized()) {
		LOGI("GPGS :: Achievement Increase");
		gameServices->Achievements().Increment(achievementId, steps);
	}
}

/*
 * Submit high score
 */
void GPGSManager::SubmitHighScore(const char *leaderboardId, uint64_t score) {
	if (gameServices && gameServices->IsAuthorized()) {
		LOGI("GPGS :: High score submitted");
		gameServices->Leaderboards().SubmitScore(leaderboardId, score);
	}
}

/*
 * Show achievemets
 */
void GPGSManager::ShowAchievements()
{
	if (gameServices && gameServices->IsAuthorized()) {
		LOGI("GPGS :: Show achievement");
		gameServices->Achievements().ShowAllUI();
	}
}

/*
 * Show Leaderboard
 */
void GPGSManager::ShowLeaderboard(const char *leaderboardId)
{
	if (gameServices && gameServices->IsAuthorized()) {
		LOGI("GPGS :: Show achievement");
		gameServices->Leaderboards().ShowUI(leaderboardId);
	}
}


/*
 * Show capture overlay
 */
void GPGSManager::ShowCaptureOverlay()
{
	LOGI("GPGS :: ShowCaptureOverlay 0");
	if(!gameServices)
	{
		LOGI("GPGS :: ShowCaptureOverlay 0 1");
		return;
	}

	if(!gameServices->IsAuthorized())
	{
		LOGI("GPGS :: ShowCaptureOverlay 0 2");
		return;
	}

	LOGI("GPGS :: ShowCaptureOverlay 1");
	//gameServices->Video().ShowCaptureOverlay();
	LOGI("GPGS :: ShowCaptureOverlay 2");
}

/*
 * Initialize GooglePlayGameServices
 */
void GPGSManager::InitServices(gpg::PlatformConfiguration &pc)
{
	LOGI("GPGS :: Initializing Services");
	if (!gameServices) {
		// Game Services have not been initialized, create a new Game Services.

		gameServices = gpg::GameServices::Builder()
		.SetOnAuthActionFinished([](gpg::AuthOperation op, gpg::AuthStatus status)
		{
			LOGI("GPGS :: Sign in finished with a result of %d", status);
			if( status == gpg::AuthStatus::VALID )
				isSignedIn = true;
			else
				isSignedIn = false;
		})
		.SetDefaultOnLog(gpg::LogLevel::VERBOSE)
		.Create(pc);
		/*
		gameServices = gpg::GameServices::Builder()
		.SetLogging(gpg::DEFAULT_ON_LOG, gpg::LogLevel::VERBOSE)
		.SetOnAuthActionStarted([](gpg::AuthOperation op){
			OnAuthActionStarted(op);
		})
		.SetOnAuthActionFinished([](gpg::AuthOperation op, gpg::AuthStatus status){
			LOGI("GPGS :: Sign in finished with a result of %d", status);
			if( status == gpg::AuthStatus::VALID )
				isSignedIn = true;
			else
				isSignedIn = false;
			OnAuthActionFinished( op, status);
		})
		.SetOnTurnBasedMatchEvent([](gpg::TurnBasedMultiplayerEvent event, std::string match_id,
									 gpg::TurnBasedMatch match) {
			LOGI("GPGS :: TurnBasedMultiplayerEvent callback");
			//Show default inbox
			ShowMatchInbox();
		})
		.SetOnMultiplayerInvitationEvent(
			[](gpg::TurnBasedMultiplayerEvent event, std::string match_id, gpg::MultiplayerInvitation invitation)
			{
			LOGI("GPGS :: MultiplayerInvitationEvent callback");
			//Show default inbox
			ShowMatchInbox();
		}).Create(pc);
		*/
	}
	LOGI("GPGS :: Created %p  ++++++++++++++++++++++++++++++++++++++++++", GPGSManager::GetGameServices());
}

/*
 * Quick match
 * - Create a match with minimal setting and play the game
 */
void GPGSManager::QuickMatch()
{
	gpg::TurnBasedMultiplayerManager& manager = gameServices->TurnBasedMultiplayer();
	gpg::TurnBasedMatchConfig config = gpg::TurnBasedMatchConfig::Builder()
	.SetMinimumAutomatchingPlayers(MIN_PLAYERS)
	.SetMaximumAutomatchingPlayers(MAX_PLAYERS).Create();
	
	// Create new match with the config
	manager.CreateTurnBasedMatch(config,[](gpg::TurnBasedMultiplayerManager::TurnBasedMatchResponse const &matchResponse)
								 {
									 if (matchResponse.status == gpg::MultiplayerStatus::VALID) {
										 LOGI("GPGS :: QuickMatch game success.");
										 PlayGame(matchResponse.match);
									 }
									 else{
										 LOGI("GPGS :: QuickMatchs game failed with a result of %d.", matchResponse.status);
									 }
								 });
}

/*
 * Invite friends
 * - Show Player Select UI via ShowPlayerSelectUI,
 * - When the UI is finished, create match and show game UI
 */
void GPGSManager::InviteFriend()
{
	gameServices->TurnBasedMultiplayer().ShowPlayerSelectUI(MIN_PLAYERS, MAX_PLAYERS, true,
	[](gpg::TurnBasedMultiplayerManager::PlayerSelectUIResponse const & response)
	{
		LOGI("GPGS :: selected match %d.", response.status);
		
		if (response.status == gpg::UIStatus::VALID) {
			
			// Create new match with the config
			gpg::TurnBasedMatchConfig config = gpg::TurnBasedMatchConfig::Builder()
			.SetMinimumAutomatchingPlayers(response.minimum_automatching_players)
			.SetMaximumAutomatchingPlayers(response.maximum_automatching_players)
			.AddAllPlayersToInvite(response.player_ids).Create();
			
			gameServices->TurnBasedMultiplayer().CreateTurnBasedMatch(config,
			[](gpg::TurnBasedMultiplayerManager::TurnBasedMatchResponse const & matchResponse)
			{
				if (matchResponse.status == gpg::MultiplayerStatus::VALID)
				{
					LOGI("GPGS :: InviteFriend game success.");
					PlayGame(matchResponse.match);
				}
				else
				{
					LOGI("GPGS :: InviteFriend game failed with a result of %d.",matchResponse.status);
				}
			});
		}
	});
}

/*
 * Show match inbox
 */
void GPGSManager::ShowMatchInbox()
{
	gameServices->TurnBasedMultiplayer().ShowMatchInboxUI(
	[](gpg::TurnBasedMultiplayerManager::MatchInboxUIResponse const & response)
	{
		if (response.status == gpg::UIStatus::VALID) {
			
			//Show game based on the user's selection
			switch (response.match.Status()) {
				case gpg::MatchStatus::THEIR_TURN:
				{
					//Manage match with dismiss, leave and cancel options
					LOGI("GPGS :: Their turn.");
				}
					break;
				case gpg::MatchStatus::MY_TURN:
				{
					//Play selected game
					LOGI("GPGS :: My turn.");
					PlayGame(response.match);
				}
					break;
				case gpg::MatchStatus::COMPLETED:
				{
					//Manage match with dismiss, rematch options
					LOGI("GPGS :: Completed.");
				}
					break;
				case gpg::MatchStatus::EXPIRED:
				default:
				{
					//Manage match with dismiss option
					LOGI("GPGS :: Expired & default.");
				}
					break;
			}
		} else {
			LOGI("GPGS :: Invalid response status");
		}
	});
}

/*
 * Leave current match
 * Invoking different APIs based on match state
 */
void GPGSManager::LeaveMatch()
{
	/*
	gpg::TurnBasedMultiplayerManager& manager = gameServices->TurnBasedMultiplayer();
	if (current_match_.Status() == gpg::MatchStatus::MY_TURN) {
		//Leave a game
		std::vector<gpg::MultiplayerParticipant> participants = current_match_.Participants();
		int32_t nextPlayerIndex = GetNextParticipant();
		if (nextPlayerIndex == NEXT_PARTICIPANT_AUTOMATCH) {
			manager.LeaveMatchDuringMyTurnAndAutomatch(current_match_,
														 [](gpg::MultiplayerStatus status) {
															 LOGI("GPGS :: Left the game...NEXT_PARTICIPANT_AUTOMATCH.");
														 });
			return;
		} else {
			manager.LeaveMatchDuringMyTurn(current_match_, participants[nextPlayerIndex],
											 [](gpg::MultiplayerStatus status) {
												 LOGI("GPGS :: Left the game...NOT_NEXT_PARTICIPANT_AUTOMATCH.");
											 });
		}
	} else {
		manager.LeaveMatchDuringTheirTurn(current_match_, [](gpg::MultiplayerStatus status) {
			LOGI("GPGS :: Left the game...NOT_MY_TURN.");
		});
	}
	*/
}

/*
 * Cancel current match
 */
void GPGSManager::CancelMatch()
{
	gameServices->TurnBasedMultiplayer().CancelMatch(current_match_,
	[](gpg::MultiplayerStatus status)
	{
		LOGI("GPGS :: Canceled the game.");
	});
}

/*
 * Dismiss current match
 */
void GPGSManager::DismissMatch()
{
	gameServices->TurnBasedMultiplayer().DismissMatch(current_match_);
}

/*
 * Rematch selected match
 */
void GPGSManager::Rematch()
{
	gameServices->TurnBasedMultiplayer().Rematch(current_match_,
	[](gpg::TurnBasedMultiplayerManager::TurnBasedMatchResponse matchResponse)
	{
		LOGI("GPGS :: Rematching the game.");
		if (matchResponse.status == gpg::MultiplayerStatus::VALID)
		{
			 PlayGame(matchResponse.match);
		}
	});
}

/*
 * Comfirm pending completion
 */
void GPGSManager::ConfirmPendingCompletion()
{
	gameServices->TurnBasedMultiplayer().ConfirmPendingCompletion(current_match_,
	[](gpg::TurnBasedMultiplayerManager::TurnBasedMatchResponse matchResponse)
	{
		if (matchResponse.status == gpg::MultiplayerStatus::VALID) {
			LOGI("GPGS :: ConfirmPendingCompletion.");
		}
	});
}

/*
 * Retrieve next participant index in the match
 * returns NEXT_PARTICIPANT_AUTOMATCH if automatching
 * returns NEXT_PARTICIPANT_NONE if there is no next participant candidate
 */
int32_t GPGSManager::GetNextParticipant()
{
	gpg::PlayerManager::FetchSelfResponse localPlayer =
	gameServices->Players().FetchSelfBlocking();
	
	//Retrieve next participant
	std::vector<gpg::MultiplayerParticipant> participants =
	current_match_.Participants();
	int32_t localPlayerIndex = -1;
	int32_t nextPlayerIndex = -1;
	int32_t size = participants.size();
	
	LOGI("GPGS :: # of participants: %d", size);
	for (int32_t i = 0; i < size; ++i)
	{
		if (participants[i].Player().Id().compare(localPlayer.data.Id()) == 0)
		{
			localPlayerIndex = i;
		}
		LOGI("GPGS :: participant: %s", participants[i].Player().Id().c_str());
	}
	if (localPlayerIndex == -1)
	{
		LOGI("GPGS :: Local player not found in a match?");
		return -1;
	}
	
	for (int32_t i = 1; i < size; ++i)
	{
		int32_t index = (localPlayerIndex + i) % size;
		if (participants[index].Status() == gpg::ParticipantStatus::INVITED ||
			participants[index].Status() == gpg::ParticipantStatus::JOINED ||
			participants[index].Status() == gpg::ParticipantStatus::NOT_INVITED_YET) {
			LOGI("GPGS :: Found next participant");
			nextPlayerIndex = index;
		}
	}
	if (nextPlayerIndex == -1)
	{
		// No next participant found
		// Do we have Auto-match player?
		if (current_match_.AutomatchingSlotsAvailable())
		{
			LOGI("GPGS :: Auto matching for next participant");
			return NEXT_PARTICIPANT_AUTOMATCH;
		} else
			return NEXT_PARTICIPANT_NONE;
	}
	return nextPlayerIndex;
}

/*
 * Play games UI that is in your turn
 */
void GPGSManager::PlayGame(gpg::TurnBasedMatch const& match)
{
	current_match_ = match;
	ParseMatchData();
	
	//your game control flow, you can call any functions about games flow here.
	//eg. LeaveMatch(),CancelMatch(),Taketurn(bool,bool)
	
}

/*
 * Take turn based on playGame UI parameters
 */
void GPGSManager::TakeTurn(const bool winning, const bool losing)
{
	gpg::TurnBasedMultiplayerManager& manager = gameServices->TurnBasedMultiplayer();
	gpg::PlayerManager::FetchSelfResponse localPlayer = gameServices->Players().FetchSelfBlocking();
	
	//Find the participant for the local player.
	gpg::MultiplayerParticipant localParticipant;
	for (auto &participant : current_match_.Participants())
	{
		if (participant.Player().Valid() && participant.Player().Id() == localPlayer.data.Id())
		{
			localParticipant = participant;
		}
	}
	
	LOGI("GPGS :: Taking my turn. local participant id:%s", localParticipant.Id().c_str());
	
	std::vector<gpg::MultiplayerParticipant> participants = current_match_.Participants();
	int32_t nextPlayerIndex = GetNextParticipant();
	
	//Get Match Data
	std::vector<uint8_t> match_data = SetupMatchData();
	
	//By default, passing through existing participatntResults
	gpg::ParticipantResults results = current_match_.ParticipantResults();
	
	if(winning || losing)
	{
		if (winning) {
			//Create winning participants result
			results = current_match_.ParticipantResults()
			.WithResult(localParticipant.Id(),  // local player ID
						0,						// placing
						gpg::MatchResult::WIN   // status
						);
		} else if (losing) {
			//Create losing participants result
			results = current_match_.ParticipantResults()
			.WithResult(localParticipant.Id(),  // local player ID
						0,						// placing
						gpg::MatchResult::LOSS  // status
						);
		}
		
		manager.FinishMatchDuringMyTurn(current_match_, match_data, results,
		[](
			 gpg::TurnBasedMultiplayerManager::TurnBasedMatchResponse const &
			 response) {
			LOGI("GPGS :: Finish match during my turn.");
		});
		return ;
	}
	
	//Take normal turn
	switch (nextPlayerIndex) {
		default:
			manager.TakeMyTurn(
								 current_match_, match_data, results, participants[nextPlayerIndex],
								 [](
									gpg::TurnBasedMultiplayerManager::TurnBasedMatchResponse const &
									response) {
									 LOGI("GPGS :: Took turn");
								 });
			break;
		case NEXT_PARTICIPANT_AUTOMATCH:
			/*
			manager.TakeMyTurnAndAutomatch(
											 current_match_, match_data, results,
											 [](
												gpg::TurnBasedMultiplayerManager::TurnBasedMatchResponse const &
												response) {
												 LOGI("GPGS :: Took turn");
											 });
			*/
			break;
		case NEXT_PARTICIPANT_NONE:
			//Error case
			manager.DismissMatch(current_match_);
			break;
	}
}

/*
 * Parse match data, you can choose JSON or other format.
 */
void GPGSManager::ParseMatchData()
{
	LOGI("GPGS :: Parse match data.");
}

/*
 * Create data to store
 */
std::vector<uint8_t> GPGSManager::SetupMatchData() {
	LOGI("GPGS :: Setup match data.");
	std::vector<uint8_t> v;
	return v;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//For GPGS
extern "C"
{
JNIEXPORT void
Java_org_gpgs_GooglePlayGameServiceC_nativeInitGPGS(JNIEnv*  env, jobject thiz, jobject activity)
{
	gpg::AndroidPlatformConfiguration platform_configuration;
	platform_configuration.SetActivity(activity);
	GPGSManager::InitServices(platform_configuration);
}

JNIEXPORT void
Java_org_gpgs_GooglePlayGameServiceC_nativeOnActivityResult(
	JNIEnv* env, jobject thiz, jobject activity, jint requestCode,
	jint resultCode, jobject data) {
  gpg::AndroidSupport::OnActivityResult(env, activity, requestCode, resultCode,
										data);
}

JNIEXPORT void
Java_org_gpgs_GooglePlayGameServiceC_nativeOnActivityCreated(
	JNIEnv* env, jobject thiz, jobject activity, jobject saved_instance_state) {
  gpg::AndroidSupport::OnActivityCreated(env, activity, saved_instance_state);
}

JNIEXPORT void
Java_org_gpgs_GooglePlayGameServiceC_nativeOnActivityDestroyed(
	JNIEnv* env, jobject thiz, jobject activity) {
  gpg::AndroidSupport::OnActivityDestroyed(env, activity);
}

JNIEXPORT void
Java_org_gpgs_GooglePlayGameServiceC_nativeOnActivityPaused(
	JNIEnv* env, jobject thiz, jobject activity) {
  gpg::AndroidSupport::OnActivityPaused(env, activity);
}

JNIEXPORT void
Java_org_gpgs_GooglePlayGameServiceC_nativeOnActivityResumed(
	JNIEnv* env, jobject thiz, jobject activity) {
  gpg::AndroidSupport::OnActivityResumed(env, activity);
}

JNIEXPORT void
Java_org_gpgs_GooglePlayGameServiceC_nativeOnActivitySaveInstanceState(
	JNIEnv* env, jobject thiz, jobject activity, jobject out_state) {
  gpg::AndroidSupport::OnActivitySaveInstanceState(env, activity, out_state);
}

JNIEXPORT void
Java_org_gpgs_GooglePlayGameServiceC_nativeOnActivityStarted(
	JNIEnv* env, jobject thiz, jobject activity) {
  gpg::AndroidSupport::OnActivityStarted(env, activity);
}

JNIEXPORT void
Java_org_gpgs_GooglePlayGameServiceC_nativeOnActivityStopped(
	JNIEnv* env, jobject thiz, jobject activity) {
  gpg::AndroidSupport::OnActivityStopped(env, activity);
}

} // extern "C"

#endif // defined(_GPGS_)
#endif // defined(ANDROID) || defined(__ANDROID__)
