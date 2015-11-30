package com.tx.wx.wxapi;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import com.tencent.tmgp.GameCatchFish;
import com.tencent.tmgp.R;
import com.tencent.mm.sdk.modelmsg.SendMessageToWX;
import com.tencent.mm.sdk.modelmsg.WXMediaMessage;
import com.tencent.mm.sdk.modelmsg.WXWebpageObject;
import com.tencent.mm.sdk.openapi.IWXAPI;
import com.tencent.mm.sdk.openapi.WXAPIFactory;

public class SendToWXActivity {

	public void sendShareTest(String text,int flag){
		 WXWebpageObject webpage = new WXWebpageObject();  
		 webpage.webpageUrl = "http://m.qicainiu.com/Home/Download";  
		 WXMediaMessage msg = new WXMediaMessage(webpage);  
		 msg.title = (String) GameCatchFish.app.getResources().getText(R.string.app_name);
		 msg.description = text;  
		 //杩欓噷鏇挎崲涓�寮犺嚜宸卞伐绋嬮噷鐨勫浘鐗囪祫婧�  
		 Bitmap thumb = BitmapFactory.decodeResource(GameCatchFish.app.getResources(), R.drawable.icon);  
		 msg.setThumbImage(thumb);        
		 SendMessageToWX.Req req = new SendMessageToWX.Req();  
		 req.transaction = String.valueOf(System.currentTimeMillis());  
		 req.message = msg;  
		 req.scene = flag==0?SendMessageToWX.Req.WXSceneSession:SendMessageToWX.Req.WXSceneTimeline;  
		 IWXAPI api= WXAPIFactory.createWXAPI(GameCatchFish.app, Constants.APP_ID);
		api.sendReq(req);
	}
	/*private String buildTransaction(final String type) {
		return (type == null) ? String.valueOf(System.currentTimeMillis()) : type + System.currentTimeMillis();
	}*/
}
