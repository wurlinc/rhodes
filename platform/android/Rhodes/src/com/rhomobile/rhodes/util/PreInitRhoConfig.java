package com.rhomobile.rhodes.util;

import android.content.res.AssetManager;
import android.util.Log;
import com.rhomobile.rhodes.RhodesActivity;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.HashMap;

/**
 * Wurl created. Same license as rhodes.
 *
 * A rho config that simply reads rhoconfig.txt; no initialization
 * needed as we do it so we can support the interactive splash screen.
 * No support to write back. Reads into plain properties file.
 */
public class PreInitRhoConfig {

    public static final String TAG = PreInitRhoConfig.class.getSimpleName();

    public static String RHO_CONFIG_LOCATION = "apps/rhoconfig.txt";

    public static HashMap<String, String> readRhoConfig() {
        HashMap<String, String> result = new HashMap<String, String>();
        AssetManager am = RhodesActivity.getContext().getResources().getAssets();
        try {
            BufferedReader br = new BufferedReader(new InputStreamReader(am.open(RHO_CONFIG_LOCATION)));
            String line = null;
            while ( (line = br.readLine()) != null) {
                if (line.indexOf("=")>1) {
                    String[] nvPair = line.split("=");
                    String key = nvPair[0];
                    String value = nvPair[1];
                    result.put(key, value);
                }
            }
        } catch (IOException e) {
            Log.d(TAG, "Cannot read "+RHO_CONFIG_LOCATION+": "+e.getLocalizedMessage());
        }
        return result;
    }

}
