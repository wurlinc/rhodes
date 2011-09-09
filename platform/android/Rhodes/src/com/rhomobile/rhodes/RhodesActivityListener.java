package com.rhomobile.rhodes;

import android.content.Intent;
import com.rhomobile.rhodes.mainview.MainView;


public interface RhodesActivityListener {
	
	void onCreate(RhodesActivity activity, Intent intent);
	void onPause(RhodesActivity activity);
	void onResume(RhodesActivity activity);
	void onNewIntent(RhodesActivity activity, Intent intent);
	
	void onRhodesActivityStartup(RhodesActivity activity);

	void onActivityResult(int requestCode, int resultCode, Intent data);

	void onWebViewCreated(MainView mainView, android.webkit.WebView webView, int tabIndex);

}
