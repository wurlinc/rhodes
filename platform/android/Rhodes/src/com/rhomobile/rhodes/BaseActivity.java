package com.rhomobile.rhodes;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.res.Configuration;
import android.os.Bundle;
import android.os.IBinder;
import android.util.Log;
import android.view.Display;

public class BaseActivity extends Activity implements ServiceConnection {
	
	private static final String TAG = BaseActivity.class.getName();
	
	private static final boolean DEBUG = false;
	
	public static final String INTENT_SOURCE = BaseActivity.class.getName();
	
	protected RhodesService mRhodesService;
	private boolean mBoundToService;
	private int mRuntimeOrientation;

	protected int getScreenOrientation() {
	    Display display = getWindowManager().getDefaultDisplay();
	    int orientation = display.getOrientation();

	    if (orientation == Configuration.ORIENTATION_UNDEFINED)
	    {
	        orientation = getResources().getConfiguration().orientation;

	        if (orientation == Configuration.ORIENTATION_UNDEFINED) {
	            if (display.getWidth() == display.getHeight())
	                orientation = Configuration.ORIENTATION_SQUARE;
	            else if(display.getWidth() < display.getHeight())
	                orientation = Configuration.ORIENTATION_PORTRAIT;
	            else
	                orientation = Configuration.ORIENTATION_LANDSCAPE;
	        }
	    }
	    return orientation;
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		Intent intent = new Intent(this, RhodesService.class);
		intent.putExtra(RhodesService.INTENT_SOURCE, INTENT_SOURCE);
		ComponentName serviceName = startService(intent);
		if (serviceName == null)
			throw new RuntimeException("Can not start Rhodes service");
		bindService(intent, this, Context.BIND_AUTO_CREATE);
		mBoundToService = true;
		
		mRuntimeOrientation = this.getScreenOrientation();
	}
	
	@Override
	protected void onDestroy() {
		if (mBoundToService) {
			unbindService(this);
			mBoundToService = false;
		}
		super.onDestroy();
	}
	
	@Override
	protected void onStart() {
		super.onStart();
		RhodesService.activityStarted();
	}
	
	@Override
	protected void onStop() {
		RhodesService.activityStopped();
		super.onStop();
	}
	
	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		Logger.T(TAG, "+++ onConfigurationChanged");
		if (RhoConf.getBool("disable_screen_rotation"))
		{
			super.onConfigurationChanged(newConfig);
			this.setRequestedOrientation(mRuntimeOrientation);
		}
		else
		{
			mRuntimeOrientation = this.getScreenOrientation();
			super.onConfigurationChanged(newConfig);
			RhodesService.getInstance().rereadScreenProperties();
		}
	}
	
//	public NativeApplication getRhodesApplication() {
//		Application app = super.getApplication();
//		if (DEBUG)
//			Log.d(TAG, "getApplication: " + app);
//		return (NativeApplication)app;
//	}

	@Override
	public void onServiceConnected(ComponentName name, IBinder service) {
		mRhodesService = ((RhodesService.LocalBinder)service).getService();
		if (DEBUG)
			Log.d(TAG, "Connected to service: " + mRhodesService);
	}

	@Override
	public void onServiceDisconnected(ComponentName name) {
		if (DEBUG)
			Log.d(TAG, "Disconnected from service: " + mRhodesService);
		mRhodesService = null;
	}
}
