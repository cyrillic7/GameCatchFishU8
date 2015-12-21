package com.tencent.tmgp;

import java.util.HashMap;
import java.util.Map;

public class CRequest {
	 /**
     * 鍘绘�?url涓殑璺緞锛岀�?涓嬭姹傚弬鏁伴儴鍒�?
     * @param strURL url鍦板�?
     * @return url璇锋眰鍙傛暟閮ㄥ�?
     */
    private static String TruncateUrlPage(String strURL)
    {
    String strAllParam=null;
      String[] arrSplit=null;
      
      strURL=strURL.trim();
      
      arrSplit=strURL.split("[?]");
      if(strURL.length()>1)
      {
          if(arrSplit.length>1)
          {
                  if(arrSplit[1]!=null)
                  {
                  strAllParam=arrSplit[1];
                  }
          }
      }
      
    return strAllParam;    
    }
    /**
     * 瑙ｆ瀽鍑簎rl鍙傛暟涓殑閿�煎�?
     * 濡� "index.jsp?Action=del&id=123"锛岃В鏋愬嚭Action:del,id:123瀛樺叆map涓�
     * @param URL  url鍦板�?
     * @return  url璇锋眰鍙傛暟閮ㄥ�?
     */
    public static Map<String, String> URLRequest(String URL)
    {
    Map<String, String> mapRequest = new HashMap<String, String>();
    
      String[] arrSplit=null;
      
    String strUrlParam=TruncateUrlPage(URL);
    if(strUrlParam==null)
    {
        return mapRequest;
    }
      //姣忎釜閿�间负涓�缁�
    arrSplit=strUrlParam.split("[&]");
    for(String strSplit:arrSplit)
    {
          String[] arrSplitEqual=null;          
          arrSplitEqual= strSplit.split("[=]"); 
          
          //瑙ｆ瀽鍑洪敭鍊�
          if(arrSplitEqual.length>1)
          {
              //姝ｇ‘瑙ｆ�??
              mapRequest.put(arrSplitEqual[0], arrSplitEqual[1]);
              
          }
          else
          {
              if(arrSplitEqual[0]!="")
              {
              //鍙湁鍙傛暟娌℃湁鍊硷紝涓嶅姞鍏�?
              mapRequest.put(arrSplitEqual[0], "");        
              }
          }
    }    
    return mapRequest;    
    }
}
