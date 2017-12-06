package pwork.service;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.games.Games;
import com.google.android.gms.games.Player;

public class GoogleSignIn
		extends Object
		implements GoogleApiClient.ConnectionCallbacks, GoogleApiClient.OnConnectionFailedListener
{

	protected static final String TAG = GoogleSignIn.class.getSimpleName();

	public static final int   GPGS_RESOLUTION = 10000;

	private static GoogleApiClient gpgs_googleClient = null;
	private static boolean         gpgs_resolving = false;		// for resolving connection
	private Activity               main_act;

	public GoogleSignIn(Activity activity)
	{
		main_act = activity;

		Log.i(TAG, "onCreate() ------------------------------------------------------------------");

		// Create the Google API Client with access to Games
		gpgs_googleClient = new GoogleApiClient.Builder(activity)
				.addConnectionCallbacks(this)
				.addOnConnectionFailedListener(this)
				.addApi(Games.API)
				.addScope(Games.SCOPE_GAMES)
				//.addApi(Plus.API)
				//.addScope(Plus.SCOPE_PLUS_LOGIN)
				//.addScope(Plus.SCOPE_PLUS_PROFILE)
				.build();
		signIn();
	}

	@Override public void onConnected(Bundle bundle)
	{
		Log.i(TAG, "onConnected(): connected to Google APIs");

		final Player p = Games.Players.getCurrentPlayer(gpgs_googleClient);
		if(p == null)
		{
			main_act.runOnUiThread(new Runnable()
			{
				@Override public void run()
				{
					Toast.makeText(main_act, "onConnected(): player : NULL", Toast.LENGTH_SHORT).show();
				}
			});
		}
		else
		{
			main_act.runOnUiThread(new Runnable()
			{
				@Override public void run()
				{
					Toast.makeText(main_act, "onConnected(): player : " + p.getDisplayName(), Toast.LENGTH_SHORT).show();
				}
			});
		}
	}

	@Override public void onConnectionSuspended(int i)
	{
		Log.i(TAG, "onConnectionSuspended(): attempting to connect");
		gpgs_googleClient.connect();
	}

	@Override public void onConnectionFailed(ConnectionResult result)
	{
		Log.i(TAG, "onConnectionFailed(): " + result.toString());
		if (gpgs_resolving)
		{
			Log.w(TAG, "onConnectionFailed(): already resolving");
			return;
		}

		gpgs_resolving = true;			// startResolutionForResult 재시도 방지
		if (result.hasResolution())
		{
			try
			{
				result.startResolutionForResult(main_act, GPGS_RESOLUTION);
				Log.i(TAG, "onConnectionFailed(): startResolutionForResult");
			}
			catch (Exception e)
			{
				Log.w(TAG, "onConnectionFailed(): startResolutionForResult:: failed");
			}
		}
	}

	public static boolean isSignedIn()
	{
		return (gpgs_googleClient != null && gpgs_googleClient.isConnected());
	}

	public static void signIn()
	{
		if(isSignedIn())
			return;
		gpgs_resolving = false;
		gpgs_googleClient.connect();
		Log.i(TAG, "start signIn -------------------------------------");
	}

	public static void signOut()
	{
		if(!isSignedIn())
			return;
		Games.signOut(gpgs_googleClient);
		if (gpgs_googleClient.isConnected())
		{
			gpgs_googleClient.disconnect();
		}
		Log.i(TAG, "sign Out    -------------------------------------");
	}

	public static void connect()
	{
		gpgs_googleClient.connect();
	}

	public static GoogleApiClient getGoolgeClient()
	{
		return gpgs_googleClient;
	}
}
