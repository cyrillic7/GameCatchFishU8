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
import com.tencent.mm.sdk.modelmsg.SendAuth;
import com.tencent.mm.sdk.openapi.IWXAPI;
import com.tencent.mm.sdk.openapi.WXAPIFactory;
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



public class GameCatchFish extends Cocos2dxActivity {
	public static GameCatchFish app;
	public String updateUrl;
	WebView m_webView;//WebView控件
    FrameLayout m_webLayout;//FrameLayout布局
    LinearLayout m_topLayout;//LinearLayout布局
    Button m_backButton;//关闭按钮
    int    m_webType;
    
	private static native void JniQQLogin(int value,final String account,final String pwd);
	private static native void JniCallPay();
	public static native void JniWXLogin(final String token);
	
	protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);	
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON); 
		app=this;
		//初始化一个空布局
		DisplayMetrics metric = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(metric);
        int width = metric.widthPixels;  // 屏幕宽度（像素）
        int height = metric.heightPixels;  // 屏幕高度（像素）
        
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
	
	public static void WXLogin()
	{
		 SendAuth.Req req = new SendAuth.Req();
		    req.scope = "snsapi_userinfo";
		    req.state = "wechat_sdk_demo_test";
		    
		    IWXAPI api= WXAPIFactory.createWXAPI(app, com.tx.wx.wxapi.Constants.APP_ID);
		    api.sendReq(req);
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
	        } else if (subType == TelephonyManager.NETWORK_TYPE_LTE) {// LTE是3g到4g的过渡，是3.9G的全球标准 
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
		pSendActivity.sendShareTest("再不捕鱼我们就老了！          您的微信好友邀请您玩“ 达人捕鱼”，立即前往下载游戏支援好友！~",flag);

	}
	/*
	 * 获取订单信息
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
			System.err.println("ELS - Chart : 文件夹创建发生异常"); 
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
    	this.runOnUiThread(new Runnable() {//在主线程里添加别的控件
            public void run() {   
            	  LayoutInflater flater = LayoutInflater.from(app);
        	      View view = flater.inflate(R.layout.webview_layout, null);
        	      m_webLayout.addView(view);
        	      //屏蔽按键不下漏
        	      view.setOnClickListener(new OnClickListener() {
  					@Override
  					public void onClick(View v) {
  					}
  				});
        	     
        	    m_webType  = webType;
                //初始化webView
                //m_webView = new WebView(game);
        	    m_webView=(WebView)view.findViewById(R.id.webViewQQLogin);
        	    m_webView.setHorizontalScrollBarEnabled(false);//水平不显示
				m_webView.setVerticalScrollBarEnabled(false); //垂直不显示 
                //设置webView能够执行javascript脚本
                m_webView.getSettings().setJavaScriptEnabled(true);            
                //设置可以支持缩放
                m_webView.getSettings().setSupportZoom(true);//设置出现缩放工具
                m_webView.getSettings().setBuiltInZoomControls(true);
                //载入URL
                m_webView.loadUrl(url);
                //使页面获得焦点
                m_webView.requestFocus();
                //如果页面中链接，如果希望点击链接继续在当前browser中响应
                m_webView.setWebViewClient(new WebViewClient(){       
                    public boolean shouldOverrideUrlLoading(WebView view, String url) {   
                        if(url.indexOf("tel:")<0){
                        	Log.v("url", url);
                        	 view.loadUrl(url); 
                        	//url参数键值对
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
                //背景图
                m_imageView = new ImageView(game);
                m_imageView.setImageResource(R.drawable.icon);
                m_imageView.setScaleType(ImageView.ScaleType.FIT_XY);
                //初始化线性布局 里面加按钮和webView
                m_topLayout = new LinearLayout(game);      
                m_topLayout.setOrientation(LinearLayout.VERTICAL);*/
                m_topLayout=(LinearLayout)view.findViewById(R.id.LinearLayoutLogin);
                //初始化返回按钮
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
                //把image加到主布局里 
                m_webLayout.addView(m_imageView);
                //把webView加入到线性布局
                m_topLayout.addView(m_backButton);
                m_topLayout.addView(m_webView);                
                //再把线性布局加入到主布局
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
 
 	//商户PID
	public static final String PARTNER = "2088111930234071";
	// 商户收款账号
	public static final String SELLER = "hzxiangwan@163.com";
	// 商户私钥，pkcs8格式
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
	// 支付宝公钥
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

				// 支付宝返回此次支付结果及加签，建议对支付宝签名信息拿签约时支付宝提供的公钥做验签
				String resultInfo = payResult.getResult();

				String resultStatus = payResult.getResultStatus();

				// 判断resultStatus 为“9000”则代表支付成功，具体状态码代表含义可参考接口文档
				if (TextUtils.equals(resultStatus, "9000")) {
					Toast.makeText(GameCatchFish.this, "支付成功",
							Toast.LENGTH_SHORT).show();
				} else {
					// 判断resultStatus 为非“9000”则代表可能支付失败
					// “8000”代表支付结果因为支付渠道原因或者系统原因还在等待支付结果确认，最终交易是否成功以服务端异步通知为准（小概率状态）
					if (TextUtils.equals(resultStatus, "8000")) {
						Toast.makeText(GameCatchFish.this, "支付结果确认中",
								Toast.LENGTH_SHORT).show();

					} else {
						// 其他值就可以判断为支付失败，包括用户主动取消支付，或者系统返回的错误
						Toast.makeText(GameCatchFish.this, "支付失败",
								Toast.LENGTH_SHORT).show();

					}
				}
				break;
			}
			case SDK_CHECK_FLAG: {
				Toast.makeText(GameCatchFish.this, "检查结果为：" + msg.obj,
						Toast.LENGTH_SHORT).show();
				break;
			}
			default:
				break;
			}
		};
	};

	/**
	 * call alipay sdk pay. 调用SDK支付
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
			
		// 订单
		String orderInfo = getOrderInfo(name, desc, price,tradeNo);
		// 对订单做RSA 签名
		String sign = sign(orderInfo);
		try {
			// 仅需对sign 做URL编码
			sign = URLEncoder.encode(sign, "UTF-8");
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
		}

		// 完整的符合支付宝参数规范的订单信息
		final String payInfo = orderInfo + "&sign=\"" + sign + "\"&"
				+ getSignType();

		Runnable payRunnable = new Runnable() {

			@Override
			public void run() {
				// 构造PayTask 对象
				PayTask alipay = new PayTask(app);
				// 调用支付接口，获取支付结果
				String result = alipay.pay(payInfo);

				Message msg = new Message();
				msg.what = SDK_PAY_FLAG;
				msg.obj = result;
				mHandler.sendMessage(msg);
			}
		};

		// 必须异步调用
		Thread payThread = new Thread(payRunnable);
		payThread.start();
	}

	/**
	 * create the order info. 创建订单信息
	 * 
	 */
	public String getOrderInfo(String subject, String body, String price,final String tradeNo) {
		// 签约合作者身份ID
		String orderInfo = "partner=" + "\"" + PARTNER + "\"";
		// 签约卖家支付宝账号
		orderInfo += "&seller_id=" + "\"" + SELLER + "\"";
		// 商户网站唯一订单号
		orderInfo += "&out_trade_no=" + "\"" + tradeNo + "\"";
		// 商品名称
		orderInfo += "&subject=" + "\"" + subject + "\"";
		// 商品详情
		orderInfo += "&body=" + "\"" + body + "\"";
		// 商品金额
		orderInfo += "&total_fee=" + "\"" + price + "\"";
		// 服务器异步通知页面路径
		// orderInfo += "&notify_url=" + "\"" +
		// "http://pay.999qp.com/alipay/notify_url.aspx"
		orderInfo += "&notify_url=" + "\""
				+ "http://pay.qicainiu.com/alipay/notify_url.aspx" + "\"";
		// 服务接口名称， 固定值
		orderInfo += "&service=\"mobile.securitypay.pay\"";
		// 支付类型， 固定值
		orderInfo += "&payment_type=\"1\"";
		// 参数编码， 固定值
		orderInfo += "&_input_charset=\"utf-8\"";
		// 设置未付款交易的超时时间
		// 默认30分钟，一旦超时，该笔交易就会自动被关闭。
		// 取值范围：1m～15d。
		// m-分钟，h-小时，d-天，1c-当天（无论交易何时创建，都在0点关闭）。
		// 该参数数值不接受小数点，如1.5h，可转换为90m。
		orderInfo += "&it_b_pay=\"30m\"";
		// extern_token为经过快登授权获取到的alipay_open_id,带上此参数用户将使用授权的账户进行支付
		// orderInfo += "&extern_token=" + "\"" + extern_token + "\"";
		// 支付宝处理完请求后，当前页面跳转到商户指定页面的路径，可空
		// orderInfo +=
		// "&return_url=\"http://pay.999qp.com/alipay/return_url.aspx\"";
		orderInfo += "&return_url=\"http://pay.qicainiu.com/alipay/return_url.aspx\"";
		// 调用银行卡支付，需配置此参数，参与签名， 固定值 （需要签约《无线银行卡快捷支付》才能使用）
		// orderInfo += "&paymethod=\"expressGateway\"";
		return orderInfo;
	}

	/**
	 * sign the order info. 对订单信息进行签名
	 * 
	 * @param content
	 *            待签名订单信息
	 */
	public String sign(String content) {
		return SignUtils.sign(content, RSA_PRIVATE);
	}

	/**
	 * get the sign type we use. 获取签名方式
	 * 
	 */
	public String getSignType() {
		return "sign_type=\"RSA\"";
	}

	/**
	 * get the out_trade_no for an order. 生成商户订单号，该值在商户端应保持唯一（可自定义格式规范）
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
        //构造订单号 (形如:20101201102322159111111)
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
	
	//更多游戏下载处理
	public static void moreGamePro(String packageName,String activity,String url)
	{
		app.avilibleMoreGames(packageName,activity,url);
	}
	
	private void avilibleMoreGames(String packageName,String activity,String url) {
		// 已安装，打开程序，需传入参数包名
		if (isAvilible(this, packageName)) {
			Intent i = new Intent();
			ComponentName cn = new ComponentName(packageName,
					packageName+"."+activity);
			i.setComponent(cn);
			startActivityForResult(i, RESULT_OK);
		} // 未安装，跳转至market下载该程序
		else {//去下载
			updateGame(url);
		}
	}
	
	private boolean isAvilible(Context context, String packageName) {
		final PackageManager packageManager = context.getPackageManager();// 获取packagemanager
		List<PackageInfo> pinfo = packageManager.getInstalledPackages(0);// 获取所有已安装程序的包信息
		List<String> pName = new ArrayList<String>();// 用于存储所有已安装程序的包名
		// 从pinfo中将包名字逐一取出，压入pName list中
		if (pinfo != null) {
			for (int i = 0; i < pinfo.size(); i++) {
				String pn = pinfo.get(i).packageName;
				pName.add(pn);
			}
		}
		return pName.contains(packageName);// 判断pName中是否有目标程序的包名，有TRUE，没有FALSE
	}


}



