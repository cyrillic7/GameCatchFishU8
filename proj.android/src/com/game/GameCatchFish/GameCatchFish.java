/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package com.game.GameCatchFish;



import java.io.File;
import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.util.Date;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Random;
import java.util.UUID;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.json.JSONException;
import org.json.JSONObject;
import org.json.JSONTokener;

import com.alipay.sdk.app.PayTask;
import com.game.GameCatchFish.R;
import com.tx.wx.wxapi.SendToWXActivity;

import android.app.ProgressDialog;
import android.app.Service;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.media.AudioManager;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.telephony.TelephonyManager;
import android.text.TextUtils;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.WindowManager;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.Toast;

import android.graphics.Bitmap;



public class GameCatchFish extends Cocos2dxActivity {
	public static GameCatchFish app;
	public String updateUrl;
	WebView m_webView;//WebView锟截硷拷
    FrameLayout m_webLayout;//FrameLayout锟斤拷锟斤拷
    LinearLayout m_topLayout;//LinearLayout锟斤拷锟斤拷
    Button m_backButton;//锟截闭帮拷钮
    int    m_webType;
    
    private static native void JniQQLogin(int value,final String account,final String pwd);
	private static native void JniCallPay();
	
	protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);	
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON); 
		app=this;
		//锟斤拷始锟斤拷一锟斤拷锟秸诧拷锟斤拷
		DisplayMetrics metric = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(metric);
        int width = metric.widthPixels;  // 锟斤拷幕锟斤拷龋锟斤拷锟斤拷兀锟�
        int height = metric.heightPixels;  // 锟斤拷幕锟竭度ｏ拷锟斤拷锟截ｏ拷
        
	    m_webLayout = new FrameLayout(this);
        FrameLayout.LayoutParams lytp = new FrameLayout.LayoutParams(width,height);
	    lytp .gravity = Gravity.CENTER;
	    addContentView(m_webLayout, lytp);

	}

	public static void callJni(final String data,int type) {
      
		app.openWebview(data,type);
    }
	
	public static void callPay(final String data)
	{
		System.out.println("callPay");
		app.pay(data);
	}
	
	
	public static String getUUid() 
	{
		System.out.println("getUUid");
		return app.getDeviceId();
    }
	
	public static String getCurNetWorkType()
	{
		return app.getNetWorkType();
	}
	
	public String getNetWorkType()
	{
		String type = ""; 
		ConnectivityManager cm = (ConnectivityManager) app
					.getSystemService(Service.CONNECTIVITY_SERVICE);
		
	    NetworkInfo info = cm.getActiveNetworkInfo(); 
	    if (info == null) { 
	        type = "null"; 
	    } else if (info.getType() == ConnectivityManager.TYPE_WIFI) { 
	        type = "wifi"; 
	    } else if (info.getType() == ConnectivityManager.TYPE_MOBILE) { 
	        int subType = info.getSubtype(); 
	        if (subType == TelephonyManager.NETWORK_TYPE_CDMA || subType == TelephonyManager.NETWORK_TYPE_GPRS 
	                || subType == TelephonyManager.NETWORK_TYPE_EDGE) { 
	            type = "2g"; 
	        } else if (subType == TelephonyManager.NETWORK_TYPE_UMTS || subType == TelephonyManager.NETWORK_TYPE_HSDPA 
	                || subType == TelephonyManager.NETWORK_TYPE_EVDO_A || subType == TelephonyManager.NETWORK_TYPE_EVDO_0 
	                || subType == TelephonyManager.NETWORK_TYPE_EVDO_B) { 
	            type = "3g"; 
	        } else if (subType == TelephonyManager.NETWORK_TYPE_LTE) {// LTE锟斤拷3g锟斤拷4g锟侥癸拷桑锟斤拷锟�3.9G锟斤拷全锟斤拷锟阶� 
	            type = "4g"; 
	        } 
	    } 
	    return type; 
	}
	
	public static String getCurVersion() 
	{
		System.out.println("getCurVersion");
		return app.getVersion();
    }
	
	public static void callWx(int flag)
	{
		System.out.println("callWx");
		app.showWxShare(flag);
	}
	
	public void showWxShare(int flag)
	{
		SendToWXActivity pSendActivity=new SendToWXActivity();
		pSendActivity.sendShareTest("锟劫诧拷锟斤拷锟斤拷锟斤拷锟角撅拷锟斤拷锟剿ｏ拷          锟斤拷锟轿拷藕锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷妗� 锟斤拷锟剿诧拷锟姐”锟斤拷锟斤拷锟斤拷前锟斤拷锟斤拷锟斤拷锟斤拷戏支援锟斤拷锟窖ｏ拷~",flag);

	}
	/*
	 * 锟斤拷取锟斤拷锟斤拷锟斤拷息
	 * 
	 */
	public static String getCurTradeNo()
	{
		return app.getOutTradeNo();
	}
	
	public static void callMakeDownDir(final String data,int type)
	{
		System.out.println("callMakeDownDir");
		app.makStorageDir(data);
	}
	
	public static void setDownloadUrl(final String data)
	{
		System.out.println("setDownloadUrl");
		app.updateGame(data);
	}
	
	public void updateGame(final String url)
	{
		updateUrl = url;
		stopService(new Intent(app, UpdateManager.class));
		startService(new Intent(app, UpdateManager.class));
	}
	
	public void makStorageDir(final String path)
	{
		try { 
			System.out.println(path);
			
			File dirFile = new File(path);
			boolean bFile = dirFile.exists(); 
			if( bFile == true ) 
			{ 
				System.out.println("The folder exists.");
			} 
			else
			{ 
				System.out.println("The folder do not exist,now trying to create a one..."); 
				bFile = dirFile.mkdir(); 
				if( bFile == true )
				{ 
					System.out.println("Create successfully!");
				}
				else
				{ 
					System.out.println("Disable to make the folder,please check the disk is full or not."); 
					System.exit(1);
				}
			} 
		} 
		catch(Exception err) 
		{ 
			System.err.println("ELS - Chart : 锟侥硷拷锟叫达拷锟斤拷锟斤拷锟斤拷锟届常"); 
			err.printStackTrace();
		} 
	}
	public String getDeviceId()
	{
		TelephonyManager tm = (TelephonyManager) this
				.getSystemService(TELEPHONY_SERVICE);
		return tm.getDeviceId();
	}
	
	private ProgressDialog gressDialog;
	
	public void openWebview(final String url,final int webType) {
    	this.runOnUiThread(new Runnable() {//锟斤拷锟斤拷锟竭筹拷锟斤拷锟斤拷颖锟侥控硷拷
            public void run() {   
            	  LayoutInflater flater = LayoutInflater.from(app);
        	      View view = flater.inflate(R.layout.webview_layout, null);
        	      m_webLayout.addView(view);
        	      //锟斤拷锟轿帮拷锟斤拷锟斤拷漏
        	      view.setOnClickListener(new OnClickListener() {
  					@Override
  					public void onClick(View v) {
  					}
  				});
        	     
        	    m_webType  = webType;
                //锟斤拷始锟斤拷webView
                //m_webView = new WebView(game);
        	    m_webView=(WebView)view.findViewById(R.id.webViewQQLogin);
        	    m_webView.setHorizontalScrollBarEnabled(false);//水平锟斤拷锟斤拷示
				m_webView.setVerticalScrollBarEnabled(false); //锟斤拷直锟斤拷锟斤拷示 
                //锟斤拷锟斤拷webView锟杰癸拷执锟斤拷javascript锟脚憋拷
                m_webView.getSettings().setJavaScriptEnabled(true);            
                //锟斤拷锟矫匡拷锟斤拷支锟斤拷锟斤拷锟斤拷
                m_webView.getSettings().setSupportZoom(true);//锟斤拷锟矫筹拷锟斤拷锟斤拷锟脚癸拷锟斤拷
                m_webView.getSettings().setBuiltInZoomControls(true);
                //锟斤拷锟斤拷URL
                m_webView.loadUrl(url);
                //使页锟斤拷锟矫斤拷锟斤拷
                m_webView.requestFocus();
                //锟斤拷锟揭筹拷锟斤拷锟斤拷锟斤拷樱锟斤拷锟斤拷希锟斤拷锟斤拷锟斤拷锟接硷拷锟斤拷锟节碉拷前browser锟斤拷锟斤拷应
                m_webView.setWebViewClient(new WebViewClient(){       
                    public boolean shouldOverrideUrlLoading(WebView view, String url) {   
                        if(url.indexOf("tel:")<0){
                        	Log.v("url", url);
                        	 view.loadUrl(url); 
                        	//url锟斤拷锟斤拷锟街碉拷锟�
                            String strRequestKeyAndValues="";        
                            Map<String, String> mapRequest =CRequest.URLRequest(url);
                            for(String strRequestKey: mapRequest.keySet()) {
                                String strRequestValue=mapRequest.get(strRequestKey);
                                strRequestKeyAndValues+="key:"+strRequestKey+",Value:"+strRequestValue+";"; 
                            } 
                        	Log.v("url", strRequestKeyAndValues);
                        	if(mapRequest.get("Id")!=null){
                        		System.out.println (mapRequest.get("Id"));
                        		System.out.println (mapRequest.get("pwd"));
                        		if(m_webType==200){
                        			System.out.println("qq login"); 
                        			JniQQLogin(1,mapRequest.get("Id"), mapRequest.get("pwd"));
                        		}
                        		removeWebView();
                        	}
                        }
                        return true;       
                    } 
                    
                    
                    @Override
					public void onPageStarted(WebView view, String url,
							Bitmap favicon) {
						// TODO Auto-generated method stub
						super.onPageStarted(view, url, favicon);
			            gressDialog = ProgressDialog.show(m_webView.getContext(), "正在加载中...", "", true);
			            gressDialog.setCanceledOnTouchOutside(true);
					}


                    @Override
					public void onPageFinished(WebView view, String url) {
						// TODO Auto-generated method stub
						
						super.onPageFinished(view, url);
						if (null != gressDialog) {
			                gressDialog.dismiss();
			            }
}
                });
                /*
                //锟斤拷锟斤拷图
                m_imageView = new ImageView(game);
                m_imageView.setImageResource(R.drawable.icon);
                m_imageView.setScaleType(ImageView.ScaleType.FIT_XY);
                //锟斤拷始锟斤拷锟斤拷锟皆诧拷锟斤拷 锟斤拷锟斤拷影锟脚ワ拷锟絯ebView
                m_topLayout = new LinearLayout(game);      
                m_topLayout.setOrientation(LinearLayout.VERTICAL);*/
                m_topLayout=(LinearLayout)view.findViewById(R.id.LinearLayoutLogin);
                //锟斤拷始锟斤拷锟斤拷锟截帮拷钮
                //m_backButton = new Button(game);
                m_backButton=(Button)view.findViewById(R.id.buttonBack);
               // m_backButton.setBackgroundResource(R.drawable.icon);
                //LinearLayout.LayoutParams lypt=new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);
                //lypt.gravity=Gravity.LEFT;
                //m_backButton.setLayoutParams(lypt);            
                m_backButton.setOnClickListener(new OnClickListener() {                    
                    public void onClick(View v) {
                    	
                    	if(m_webType==200){
                    		removeWebView();	
                    	}
                    	else
                    	{
	                		if(m_webView.canGoBack())
	                		{
	                    		m_webView.goBack();
	                		}
	                    	else
	                    	{
	                    		removeWebView();
	                    	}
                    	}
                    	
                    }
                });
                /*
                //锟斤拷image锟接碉拷锟斤拷锟斤拷锟斤拷锟斤拷 
                m_webLayout.addView(m_imageView);
                //锟斤拷webView锟斤拷锟诫到锟斤拷锟皆诧拷锟斤拷
                m_topLayout.addView(m_backButton);
                m_topLayout.addView(m_webView);                
                //锟劫帮拷锟斤拷锟皆诧拷锟街硷拷锟诫到锟斤拷锟斤拷锟斤拷
                m_webLayout.addView(m_topLayout);*/
            }
        });
    }

 public void removeWebView() {    
    	
    	//m_webLayout.removeView(m_imageView);
    	m_webLayout.removeAllViews();
        
       m_webLayout.removeView(m_topLayout);
       m_topLayout.destroyDrawingCache();
                
        m_topLayout.removeView(m_webView);
        m_webView.destroy();
                
        m_topLayout.removeView(m_backButton);
        m_backButton.destroyDrawingCache();
    }
 
 	//锟教伙拷PID
	public static final String PARTNER = "2088111930234071";
	// 锟教伙拷锟秸匡拷锟剿猴拷
	public static final String SELLER = "hzxiangwan@163.com";
	// 锟教伙拷私钥锟斤拷pkcs8锟斤拷式
	public static final String RSA_PRIVATE = "MIICdwIBADANBgkqhkiG9w0BAQEFAASCAmEwggJdAgEAAoGBAN64/lUSkVgd/cVh"
			+ "opVPrTquh+K0MvOt9k1boX3yBKmeB95ZV7y/GfiSAs9swuxZn7rkZN9WELqoB1P5"
			+ "lk/djECqWRmLBFOIetGqVZ3aqrVjVfHE3uYW3PgxCBXukMVzdX/7uzWHtyvsScFf"
			+ "o8LvKvchRhA4pVnlT11YMXMGuMdJAgMBAAECgYEAytXFYmrPjWV66Nh7PenbRJcQ"
			+ "T+l2gbALoPXzRAU2M9clKV1KZy3PFC69enR9rMwOiSFZsH/sKmG+kr66IrAM+mB4"
			+ "QY1BtOzQXpWvzlXx8QMSbc6a1r/jHITpPKbd4gpU6f6OZVjkY1ViXN0q/mzt735h"
			+ "pP1tNTMbknpcrVAbqRECQQDwrvZhz0T/jr5vuvXKbpaVntN8h3aw3xHyARvWb8c+"
			+ "LPChwYlfHeLLRAJs8Zbl5EH7+YK5Idsz51DM9aTAzbbVAkEA7OVtOXMEfvUGYfBF"
			+ "15c21xFwwllTPbeJsa64YizRlSE32Z6oCiwkYD32kntKEt+0joVOPatmFsCVdql6"
			+ "MXIwpQJBAMRQAViwJfhIpt4dYcPWxe6OGLQXDeGgYVPCQcF7dMkrAK7/XBwAFW90"
			+ "LoJL36ftsUBkOJLmoZPJVeeHaPV2FW0CQGiVfRwAyDpYaEHytpLQYliejX5vuw0j"
			+ "KznzXdmR2dERwjtkDOG4zTJkCKhwn6oncun8ticJtV9UFGg3uAd+VzUCQAk/iafj"
			+ "GsJS5drAHvvBYeFQR5ooG/CqMz2K4MHnHGh/pAWCLnNQgO6FgNl7yjkjuMgbRBsk"
			+ "yMwdN/3atqT0RsQ=";
	// 支锟斤拷锟斤拷锟斤拷钥
	public static final String RSA_PUBLIC =
	// "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCnxj/9qwVfgoUh/y2W89L6BkRAFljhNhgPdyPuBV64bfQNN1PjbCzkIM6qRdKBoLPXmKKMiFYnkd6rAoprih3/PrQEB/VsW8OoM8fxn67UDYuyBTqA23MML9q1+ilIZwBC2AQ2UBVOrFXfFl75p6/B5KsiNG9zpgmLCUYuLkxpLQIDAQAB"

	"MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDeuP5VEpFYHf3FYaKVT606rofi"
			+ "tDLzrfZNW6F98gSpngfeWVe8vxn4kgLPbMLsWZ+65GTfVhC6qAdT+ZZP3YxAqlkZ"
			+ "iwRTiHrRqlWd2qq1Y1XxxN7mFtz4MQgV7pDFc3V/+7s1h7cr7EnBX6PC7yr3IUYQ"
			+ "OKVZ5U9dWDFzBrjHSQIDAQAB";

	private static final int SDK_PAY_FLAG = 1;

	private static final int SDK_CHECK_FLAG = 2;
	
	private Handler mHandler = new Handler() {
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case SDK_PAY_FLAG: {
				PayResult payResult = new PayResult((String) msg.obj);

				// 支锟斤拷锟斤拷锟斤拷锟截此达拷支锟斤拷锟斤拷锟斤拷签锟斤拷锟斤拷锟斤拷锟街э拷锟斤拷锟角╋拷锟斤拷锟较拷锟角┰际敝э拷锟斤拷锟斤拷峁╋拷墓锟皆匡拷锟斤拷锟角�
				String resultInfo = payResult.getResult();

				String resultStatus = payResult.getResultStatus();

				// 锟叫讹拷resultStatus 为锟斤拷9000锟斤拷锟斤拷锟斤拷支锟斤拷锟缴癸拷锟斤拷锟斤拷锟斤拷状态锟斤拷锟�?锟斤拷刹慰锟斤拷涌锟斤拷牡锟�
				if (TextUtils.equals(resultStatus, "9000")) {
					Toast.makeText(GameCatchFish.this, "支锟斤拷锟缴癸拷",
							Toast.LENGTH_SHORT).show();
				} else {
					// 锟叫讹拷resultStatus 为锟角★拷9000锟斤拷锟斤拷锟斤拷锟斤拷锟街э拷锟绞э拷锟�
					// 锟斤拷8000锟斤拷锟斤拷锟街э拷锟斤拷锟斤拷锟斤拷为支锟斤拷锟斤拷锟斤拷原锟斤拷锟斤拷锟较低吃拷锟斤拷诘却锟街э拷锟斤拷锟斤拷确锟较ｏ拷锟斤拷锟秸斤拷锟斤拷锟角凤拷晒锟斤拷苑锟斤拷锟斤拷锟届步通知为准锟斤拷小锟斤拷锟斤拷状态锟斤拷
					if (TextUtils.equals(resultStatus, "8000")) {
						Toast.makeText(GameCatchFish.this, "支锟斤拷锟斤拷锟饺凤拷锟斤拷锟�",
								Toast.LENGTH_SHORT).show();

					} else {
						// 锟斤拷锟斤拷值锟酵匡拷锟斤拷锟叫讹拷为支锟斤拷失锟杰ｏ拷锟斤拷锟斤拷锟矫伙拷锟斤拷锟斤拷取锟斤拷支锟斤拷锟斤拷锟斤拷锟斤拷系统锟斤拷锟截的达拷锟斤拷
						Toast.makeText(GameCatchFish.this, "支锟斤拷失锟斤拷",
								Toast.LENGTH_SHORT).show();

					}
				}
				break;
			}
			case SDK_CHECK_FLAG: {
				Toast.makeText(GameCatchFish.this, "锟斤拷锟斤拷锟轿拷锟�" + msg.obj,
						Toast.LENGTH_SHORT).show();
				break;
			}
			default:
				break;
			}
		};
	};

	/**
	 * call alipay sdk pay. 锟斤拷锟斤拷SDK支锟斤拷
	 * 
	 */
	public void pay(final String data) {
		System.out.println("pay");
		System.out.println(data);
		JSONTokener jsonTokener = new JSONTokener(data);
		JSONObject jsonObject;
		String name="";
		String desc="";
		String  price ="";
		String tradeNo = "";
		try {
			jsonObject = (JSONObject) jsonTokener.nextValue();
			name = jsonObject.getString("name");
			System.out.println (name);
			desc = jsonObject.getString("desc");
			System.out.println (desc);
			price = jsonObject.getString("price");
			System.out.println (price);
			tradeNo = jsonObject.getString("tradeNo");
			System.out.println (tradeNo);
		} catch (JSONException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
			
		// 锟斤拷锟斤拷
		String orderInfo = getOrderInfo(name, desc, price,tradeNo);
		// 锟皆讹拷锟斤拷锟斤拷RSA 签锟斤拷
		String sign = sign(orderInfo);
		try {
			// 锟斤拷锟斤拷锟絪ign 锟斤拷URL锟斤拷锟斤拷
			sign = URLEncoder.encode(sign, "UTF-8");
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
		}

		// 锟斤拷锟斤拷姆锟斤拷支锟斤拷锟斤拷锟斤拷锟斤拷娣讹拷亩锟斤拷锟斤拷锟较�
		final String payInfo = orderInfo + "&sign=\"" + sign + "\"&"
				+ getSignType();

		Runnable payRunnable = new Runnable() {

			@Override
			public void run() {
				// 锟斤拷锟斤拷PayTask 锟斤拷锟斤拷
				PayTask alipay = new PayTask(app);
				// 锟斤拷锟斤拷支锟斤拷锟接口ｏ拷锟斤拷取支锟斤拷锟斤拷锟�
				String result = alipay.pay(payInfo);

				Message msg = new Message();
				msg.what = SDK_PAY_FLAG;
				msg.obj = result;
				mHandler.sendMessage(msg);
			}
		};

		// 锟斤拷锟斤拷锟届步锟斤拷锟斤拷
		Thread payThread = new Thread(payRunnable);
		payThread.start();
	}

	/**
	 * create the order info. 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷息
	 * 
	 */
	public String getOrderInfo(String subject, String body, String price,final String tradeNo) {
		// 签约锟斤拷锟斤拷锟斤拷锟斤拷锟絀D
		String orderInfo = "partner=" + "\"" + PARTNER + "\"";
		// 签约锟斤拷锟斤拷支锟斤拷锟斤拷锟剿猴拷
		orderInfo += "&seller_id=" + "\"" + SELLER + "\"";
		// 锟教伙拷锟斤拷站唯一锟斤拷锟斤拷锟斤拷
		orderInfo += "&out_trade_no=" + "\"" + tradeNo + "\"";
		// 锟斤拷品锟斤拷锟�
		orderInfo += "&subject=" + "\"" + subject + "\"";
		// 锟斤拷品锟斤拷锟斤拷
		orderInfo += "&body=" + "\"" + body + "\"";
		// 锟斤拷品锟斤拷锟�
		orderInfo += "&total_fee=" + "\"" + price + "\"";
		// 锟斤拷锟斤拷锟斤拷锟届步通知页锟斤拷路锟斤拷
		// orderInfo += "&notify_url=" + "\"" +
		// "http://pay.999qp.com/alipay/notify_url.aspx"
		orderInfo += "&notify_url=" + "\""
				+ "http://pay.qicainiu.com/alipay/notify_url.aspx" + "\"";
		// 锟斤拷锟斤拷涌锟斤拷锟狡ｏ拷 锟教讹拷值
		orderInfo += "&service=\"mobile.securitypay.pay\"";
		// 支锟斤拷锟斤拷锟酵ｏ拷 锟教讹拷值
		orderInfo += "&payment_type=\"1\"";
		// 锟斤拷锟斤拷锟斤拷耄� 锟教讹拷值
		orderInfo += "&_input_charset=\"utf-8\"";
		// 锟斤拷锟斤拷未锟斤拷锟筋交锟阶的筹拷时时锟斤拷
		// 默锟斤拷30锟斤拷锟接ｏ拷一锟斤拷锟斤拷时锟斤拷锟矫笔斤拷锟阶就伙拷锟皆讹拷锟斤拷锟截闭★拷
		// 取值锟斤拷围锟斤拷1m锟斤拷15d锟斤拷
		// m-锟斤拷锟接ｏ拷h-小时锟斤拷d-锟届，1c-锟斤拷锟届（锟斤拷锟桔斤拷锟阶猴拷时锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷0锟斤拷乇眨锟斤拷锟�
		// 锟矫诧拷锟斤拷锟斤拷值锟斤拷锟斤拷锟斤拷小锟斤拷悖拷锟�1.5h锟斤拷锟斤拷转锟斤拷为90m锟斤拷
		orderInfo += "&it_b_pay=\"30m\"";
		// extern_token为锟斤拷锟斤拷锟斤拷锟斤拷权锟斤拷取锟斤拷锟斤拷alipay_open_id,锟斤拷锟较此诧拷锟斤拷锟矫伙拷锟斤拷使锟斤拷锟斤拷权锟斤拷锟剿伙拷锟斤拷锟斤拷支锟斤拷
		// orderInfo += "&extern_token=" + "\"" + extern_token + "\"";
		// 支锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷螅锟角耙筹拷锟斤拷锟阶拷锟斤拷袒锟街革拷锟揭筹拷锟斤拷路锟斤拷锟斤拷锟缴匡拷
		// orderInfo +=
		// "&return_url=\"http://pay.999qp.com/alipay/return_url.aspx\"";
		orderInfo += "&return_url=\"http://pay.qicainiu.com/alipay/return_url.aspx\"";
		// 锟斤拷锟斤拷锟斤拷锟叫匡拷支锟斤拷锟斤拷锟斤拷锟斤拷锟矫此诧拷锟斤拷锟斤拷锟角╋拷锟� 锟教讹拷值 锟斤拷锟斤拷要签约锟斤拷锟斤拷锟斤拷锟斤拷锟叫匡拷锟斤拷锟街э拷锟斤拷锟斤拷锟斤拷锟绞癸拷茫锟�
		// orderInfo += "&paymethod=\"expressGateway\"";
		return orderInfo;
	}

	/**
	 * sign the order info. 锟皆讹拷锟斤拷锟斤拷息锟斤拷锟斤拷签锟斤拷
	 * 
	 * @param content
	 *            锟斤拷签锟斤拷锟斤拷息
	 */
	public String sign(String content) {
		return SignUtils.sign(content, RSA_PRIVATE);
	}

	/**
	 * get the sign type we use. 锟斤拷取签锟斤拷式
	 * 
	 */
	public String getSignType() {
		return "sign_type=\"RSA\"";
	}

	/**
	 * get the out_trade_no for an order. 锟斤拷锟斤拷袒锟斤拷锟斤拷锟斤拷牛锟斤拷锟街碉拷锟斤拷袒锟斤拷锟接︼拷锟斤拷锟轿ㄒ伙拷锟斤拷锟斤拷远锟斤拷锟斤拷式锟芥范锟斤拷
	 * 
	 */
	public String getOutTradeNo() {
		String key =GetOrderIDByPrefix("MFB");	
		System.out.println("---TradeResult:"+key);
		return key;
	}
	
	public String getVersion() {
		try {
			PackageManager manager = this.getPackageManager();
			PackageInfo info = manager.getPackageInfo(this.getPackageName(), 0);
			String version = info.versionName;
			return  version;
		} catch (Exception e) {
		e.printStackTrace();
			return "";
		}
	}
	
	/***
	 * 
	 * @param prefix
	 * @return
	 */
	public  String GetOrderIDByPrefix(String prefix)
    {
        //锟斤拷锟届订锟斤拷锟斤拷 (锟斤拷锟斤拷:20101201102322159111111)
        int orderIDLength = 32;
        int randomLength = 6;
        StringBuffer tradeNoBuffer = new StringBuffer();

        tradeNoBuffer.append(prefix);
        tradeNoBuffer.append(GetDateTimeLongString());

        if ((tradeNoBuffer.length() + randomLength) > orderIDLength)
            randomLength = orderIDLength - tradeNoBuffer.length();

        tradeNoBuffer.append(CreateRandom(randomLength, 1, 0, 0, 0, ""));
        System.out.println("toString:"+tradeNoBuffer.toString());
        return tradeNoBuffer.toString();
    }

	 public  String GetDateTimeLongString()
     {
		SimpleDateFormat format = new SimpleDateFormat("yyyyMMddHHmmssSSS",
					Locale.getDefault());
		Date date = new Date();
		String key = format.format(date);
		System.out.println("Time:"+key);
		return key;
     }
        
	 
	public  String CreateRandom(int length, int useNum, int useLow, int useUpp, int useSpe, String custom)
    {
        byte[] data = new byte[4];
        Random random=new Random();
        String str = "";
        String str2 = custom;
        if (useNum == 1)
        {
            str2 = str2 + "0123456789"; 
        }
        if (useLow == 1)
        {
            str2 = str2 + "abcdefghijklmnopqrstuvwxyz";
        }
        if (useUpp == 1)
        {
            str2 = str2 + "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        }
        if (useSpe == 1)
        {
            str2 = str2 + "!\"#$%&'()*+,-.<=>?@[\\]^_`{|}~";
        }
        for (int i = 0; i < length; i++)
        {
        	int start=random.nextInt(str2.length() - 1);
        	String strTemp= str2.substring(start, start+1);
            str = str +strTemp;
        }
        System.out.println("random:"+str);
        return str;
    }
	
	//锟斤拷锟斤拷锟较凤拷锟斤拷卮锟斤拷锟�
	public static void moreGamePro(String packageName,String activity,String url)
	{
		app.avilibleMoreGames(packageName,activity,url);
	}
	
	private void avilibleMoreGames(String packageName,String activity,String url) {
		// 锟窖帮拷装锟斤拷锟津开筹拷锟斤拷锟借传锟斤拷锟斤拷锟斤拷锟斤拷
		if (isAvilible(this, packageName)) {
			Intent i = new Intent();
			ComponentName cn = new ComponentName(packageName,
					packageName+"."+activity);
			i.setComponent(cn);
			startActivityForResult(i, RESULT_OK);
		} // 未锟斤拷装锟斤拷锟斤拷转锟斤拷market锟斤拷锟截该筹拷锟斤拷
		else {//去锟斤拷锟斤拷
			updateGame(url);
		}
	}
	
	private boolean isAvilible(Context context, String packageName) {
		final PackageManager packageManager = context.getPackageManager();// 锟斤拷取packagemanager
		List<PackageInfo> pinfo = packageManager.getInstalledPackages(0);// 锟斤拷取锟斤拷锟斤拷锟窖帮拷装锟斤拷锟斤拷陌锟斤拷锟较�
		List<String> pName = new ArrayList<String>();// 锟斤拷锟节存储锟斤拷锟斤拷锟窖帮拷装锟斤拷锟斤拷陌锟斤拷锟�
		// 锟斤拷pinfo锟叫斤拷锟斤拷锟斤拷锟斤拷锟斤拷一取锟斤拷锟斤拷压锟斤拷pName list锟斤拷
		if (pinfo != null) {
			for (int i = 0; i < pinfo.size(); i++) {
				String pn = pinfo.get(i).packageName;
				pName.add(pn);
			}
		}
		return pName.contains(packageName);// 锟叫讹拷pName锟斤拷锟角凤拷锟斤拷目锟斤拷锟斤拷锟侥帮拷锟斤拷锟斤拷TRUE锟斤拷没锟斤拷FALSE
	}


}



