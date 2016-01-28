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
package com.tencent.tmgp.NewGameCatchFish;



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


//import com.alipay.sdk.app.PayTask;
import com.tencent.tmgp.NewGameCatchFish.R;
import com.tx.wx.wxapi.SendToWXActivity;
import com.u8.sdk.U8CocosActivity;

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
import android.provider.SyncStateContract.Constants;
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



public class GameCatchFish extends U8CocosActivity {
	public static GameCatchFish app;
	public String updateUrl;
	WebView m_webView;//WebView�ؼ�
    FrameLayout m_webLayout;//FrameLayout����
    LinearLayout m_topLayout;//LinearLayout����
    Button m_backButton;//�رհ�ť
    int    m_webType;
    
    private static native void JniQQLogin(int value,final String account,final String pwd);
	private static native void JniCallPay();
	public static native void JniWXLogin(final String token);
	
	protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);	
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON); 
		app=this;
		//��ʼ��һ���ղ���
		DisplayMetrics metric = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(metric);
        int width = metric.widthPixels;  // ��Ļ��ȣ����أ�
        int height = metric.heightPixels;  // ��Ļ�߶ȣ����أ�
        
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
	/*
	public static void WXLogin()
	{
		 SendAuth.Req req = new SendAuth.Req();
		    req.scope = "snsapi_userinfo";
		    req.state = "wechat_sdk_demo_test";
		    
		    IWXAPI api= WXAPIFactory.createWXAPI(app, com.tx.wx.wxapi.Constants.APP_ID);
		    api.sendReq(req);
	}
	*/
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
	        } else if (subType == TelephonyManager.NETWORK_TYPE_LTE) {// LTE��3g��4g�Ĺ�ɣ���3.9G��ȫ���׼ 
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
		pSendActivity.sendShareTest("�ٲ��������Ǿ����ˣ�          ���΢�ź����������桰 ���˲��㡱������ǰ��������Ϸ֧Ԯ���ѣ�~",flag);

	}
	/*
	 * ��ȡ������Ϣ
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
			System.err.println("ELS - Chart : �ļ��д��������쳣"); 
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
	private boolean   mInitWeb = false;
	
	public void openWebview(final String url,final int webType) {
    	this.runOnUiThread(new Runnable() {//�����߳�����ӱ�Ŀؼ�
            public void run() {   
            	  LayoutInflater flater = LayoutInflater.from(app);
        	      View view = flater.inflate(R.layout.webview_layout, null);
        	      m_webLayout.addView(view);
        	      //���ΰ�����©
        	      view.setOnClickListener(new OnClickListener() {
  					@Override
  					public void onClick(View v) {
  					}
  				});
        	     
        	    m_webType  = webType;
                //��ʼ��webView
                //m_webView = new WebView(game);
        	    m_webView=(WebView)view.findViewById(R.id.webViewQQLogin);
        	    m_webView.setHorizontalScrollBarEnabled(false);//ˮƽ����ʾ
				m_webView.setVerticalScrollBarEnabled(false); //��ֱ����ʾ 
                //����webView�ܹ�ִ��javascript�ű�
                m_webView.getSettings().setJavaScriptEnabled(true);            
                //���ÿ���֧������
                m_webView.getSettings().setSupportZoom(true);//���ó������Ź���
                m_webView.getSettings().setBuiltInZoomControls(true);
                //����URL
                m_webView.loadUrl(url);
                //ʹҳ���ý���
                m_webView.requestFocus();
                //���ҳ�������ӣ����ϣ�������Ӽ����ڵ�ǰbrowser����Ӧ
                m_webView.setWebViewClient(new WebViewClient(){       
                    public boolean shouldOverrideUrlLoading(WebView view, String url) {   
                        if(url.indexOf("tel:")<0){
                        	Log.v("url", url);
                        	 view.loadUrl(url); 
                        	//url�����ֵ��
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
						if (mInitWeb == false)
						{
							mInitWeb = true;
							gressDialog = ProgressDialog.show(m_webView.getContext(), "加载中...", "", true);
			            gressDialog.setCanceledOnTouchOutside(true);
					}
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
                //����ͼ
                m_imageView = new ImageView(game);
                m_imageView.setImageResource(R.drawable.icon);
                m_imageView.setScaleType(ImageView.ScaleType.FIT_XY);
                //��ʼ�����Բ��� ����Ӱ�ť��webView
                m_topLayout = new LinearLayout(game);      
                m_topLayout.setOrientation(LinearLayout.VERTICAL);*/
                m_topLayout=(LinearLayout)view.findViewById(R.id.LinearLayoutLogin);
                //��ʼ�����ذ�ť
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
                //��image�ӵ��������� 
                m_webLayout.addView(m_imageView);
                //��webView���뵽���Բ���
                m_topLayout.addView(m_backButton);
                m_topLayout.addView(m_webView);                
                //�ٰ����Բ��ּ��뵽������
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
 
 	//�̻�PID
	public static final String PARTNER = "2088111930234071";
	// �̻��տ��˺�
	public static final String SELLER = "hzxiangwan@163.com";
	// �̻�˽Կ��pkcs8��ʽ
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
	// ֧������Կ
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

				// ֧�������ش˴�֧������ǩ�������֧����ǩ����Ϣ��ǩԼʱ֧�����ṩ�Ĺ�Կ����ǩ
				String resultInfo = payResult.getResult();

				String resultStatus = payResult.getResultStatus();

				// �ж�resultStatus Ϊ��9000������֧���ɹ�������״̬���?��ɲο��ӿ��ĵ�
				if (TextUtils.equals(resultStatus, "9000")) {
					Toast.makeText(GameCatchFish.this, "֧���ɹ�",
							Toast.LENGTH_SHORT).show();
				} else {
					// �ж�resultStatus Ϊ�ǡ�9000���������֧��ʧ��
					// ��8000�����֧�������Ϊ֧������ԭ�����ϵͳԭ���ڵȴ�֧�����ȷ�ϣ����ս����Ƿ�ɹ��Է�����첽֪ͨΪ׼��С����״̬��
					if (TextUtils.equals(resultStatus, "8000")) {
						Toast.makeText(GameCatchFish.this, "֧�����ȷ����",
								Toast.LENGTH_SHORT).show();

					} else {
						// ����ֵ�Ϳ����ж�Ϊ֧��ʧ�ܣ������û�����ȡ��֧��������ϵͳ���صĴ���
						Toast.makeText(GameCatchFish.this, "֧��ʧ��",
								Toast.LENGTH_SHORT).show();

					}
				}
				break;
			}
			case SDK_CHECK_FLAG: {
				Toast.makeText(GameCatchFish.this, "�����Ϊ��" + msg.obj,
						Toast.LENGTH_SHORT).show();
				break;
			}
			default:
				break;
			}
		};
	};

	/**
	 * call alipay sdk pay. ����SDK֧��
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
			
		/*
		// ����
		String orderInfo = getOrderInfo(name, desc, price,tradeNo);
		// �Զ�����RSA ǩ��
		String sign = sign(orderInfo);
		try {
			// �����sign ��URL����
			sign = URLEncoder.encode(sign, "UTF-8");
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
		}

		// ����ķ��֧��������淶�Ķ�����Ϣ
		final String payInfo = orderInfo + "&sign=\"" + sign + "\"&"
				+ getSignType();

		Runnable payRunnable = new Runnable() {

			@Override
			public void run() {
				// ����PayTask ����
				PayTask alipay = new PayTask(app);
				// ����֧���ӿڣ���ȡ֧�����
				String result = alipay.pay(payInfo);

				Message msg = new Message();
				msg.what = SDK_PAY_FLAG;
				msg.obj = result;
				mHandler.sendMessage(msg);
			}
			
		};

		// �����첽����
		Thread payThread = new Thread(payRunnable);
		payThread.start();*/
	}

	/**
	 * create the order info. ����������Ϣ
	 * 
	 */
	public String getOrderInfo(String subject, String body, String price,final String tradeNo) {
		// ǩԼ���������ID
		String orderInfo = "partner=" + "\"" + PARTNER + "\"";
		// ǩԼ����֧�����˺�
		orderInfo += "&seller_id=" + "\"" + SELLER + "\"";
		// �̻���վΨһ������
		orderInfo += "&out_trade_no=" + "\"" + tradeNo + "\"";
		// ��Ʒ���
		orderInfo += "&subject=" + "\"" + subject + "\"";
		// ��Ʒ����
		orderInfo += "&body=" + "\"" + body + "\"";
		// ��Ʒ���
		orderInfo += "&total_fee=" + "\"" + price + "\"";
		// �������첽֪ͨҳ��·��
		// orderInfo += "&notify_url=" + "\"" +
		// "http://pay.999qp.com/alipay/notify_url.aspx"
		orderInfo += "&notify_url=" + "\""
				+ "http://pay.qicainiu.com/alipay/notify_url.aspx" + "\"";
		// ����ӿ���ƣ� �̶�ֵ
		orderInfo += "&service=\"mobile.securitypay.pay\"";
		// ֧�����ͣ� �̶�ֵ
		orderInfo += "&payment_type=\"1\"";
		// ������룬 �̶�ֵ
		orderInfo += "&_input_charset=\"utf-8\"";
		// ����δ����׵ĳ�ʱʱ��
		// Ĭ��30���ӣ�һ����ʱ���ñʽ��׾ͻ��Զ����رա�
		// ȡֵ��Χ��1m��15d��
		// m-���ӣ�h-Сʱ��d-�죬1c-���죨���۽��׺�ʱ����������0��رգ���
		// �ò�����ֵ������С��㣬��1.5h����ת��Ϊ90m��
		orderInfo += "&it_b_pay=\"30m\"";
		// extern_tokenΪ��������Ȩ��ȡ����alipay_open_id,���ϴ˲����û���ʹ����Ȩ���˻�����֧��
		// orderInfo += "&extern_token=" + "\"" + extern_token + "\"";
		// ֧��������������󣬵�ǰҳ����ת���̻�ָ��ҳ���·�����ɿ�
		// orderInfo +=
		// "&return_url=\"http://pay.999qp.com/alipay/return_url.aspx\"";
		orderInfo += "&return_url=\"http://pay.qicainiu.com/alipay/return_url.aspx\"";
		// �������п�֧���������ô˲������ǩ�� �̶�ֵ ����ҪǩԼ���������п����֧��������ʹ�ã�
		// orderInfo += "&paymethod=\"expressGateway\"";
		return orderInfo;
	}

	/**
	 * sign the order info. �Զ�����Ϣ����ǩ��
	 * 
	 * @param content
	 *            ��ǩ����Ϣ
	 */
	public String sign(String content) {
		return SignUtils.sign(content, RSA_PRIVATE);
	}

	/**
	 * get the sign type we use. ��ȡǩ��ʽ
	 * 
	 */
	public String getSignType() {
		return "sign_type=\"RSA\"";
	}

	/**
	 * get the out_trade_no for an order. ����̻������ţ���ֵ���̻���Ӧ����Ψһ�����Զ����ʽ�淶��
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
        //���충���� (����:20101201102322159111111)
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
	
	//�����Ϸ���ش���
	public static void moreGamePro(String packageName,String activity,String url)
	{
		app.avilibleMoreGames(packageName,activity,url);
	}
	
	private void avilibleMoreGames(String packageName,String activity,String url) {
		// �Ѱ�װ���򿪳����贫��������
		if (isAvilible(this, packageName)) {
			Intent i = new Intent();
			ComponentName cn = new ComponentName(packageName,
					packageName+"."+activity);
			i.setComponent(cn);
			startActivityForResult(i, RESULT_OK);
		} // δ��װ����ת��market���ظó���
		else {//ȥ����
			updateGame(url);
		}
	}
	
	private boolean isAvilible(Context context, String packageName) {
		final PackageManager packageManager = context.getPackageManager();// ��ȡpackagemanager
		List<PackageInfo> pinfo = packageManager.getInstalledPackages(0);// ��ȡ�����Ѱ�װ����İ���Ϣ
		List<String> pName = new ArrayList<String>();// ���ڴ洢�����Ѱ�װ����İ���
		// ��pinfo�н���������һȡ����ѹ��pName list��
		if (pinfo != null) {
			for (int i = 0; i < pinfo.size(); i++) {
				String pn = pinfo.get(i).packageName;
				pName.add(pn);
			}
		}
		return pName.contains(packageName);// �ж�pName���Ƿ���Ŀ�����İ�����TRUE��û��FALSE
	}


}



