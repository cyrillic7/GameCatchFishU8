package com.tencent.tmgp.NewGameCatchFish;



import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLConnection;
import java.util.HashMap;

import com.tencent.tmgp.NewGameCatchFish.R;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.AlertDialog.Builder;
import android.app.Service;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.Uri;
import android.os.Environment;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

public class UpdateManager extends Service
{
	/** 下载�??? */
	private static final int DOWNLOAD = 1;
	/** 下载结束 */
	private static final int DOWNLOAD_FINISH = 2;
	/**下载失败*/
	private static final int DOWNLOADFAIL=3;
	/**下载保存路径 */
	private String mSavePath;
	/** 记录进度条数�??? */
	private int progress;
	/** 保存解析的XML信息 */
	HashMap<String, String> mHashMap;
	/** 更新进度�??? */
	private ProgressBar mProgress;
	/**进度条进度数�???*/
	private TextView updateProgressBar;
	private Dialog mDownloadDialog;
	/** 是否取消更新 */
	private boolean cancelUpdate = false;
	private GameCatchFish game;
	@Override
	public IBinder onBind(Intent intent) {
		// TODO Auto-generated method stub
		return null;
	}
	@Override
	public void onCreate() {
		super.onCreate();
		game=GameCatchFish.app;
		mHashMap=new HashMap<String, String>();
	}
	//@Override
	public void onStart(Intent intent, int startId) {
		super.onStart(intent, startId);
		showDownloadDialog();
	}
	public void onDestroy() {
		super.onDestroy();
		stopService(new Intent(game,UpdateManager.class));
	}
	/**
	 * 显示软件下载对话�???
	 */
	private void showDownloadDialog()
	{
		String name=game.updateUrl.substring( game.updateUrl.lastIndexOf("/")+1,game.updateUrl.length());
		mHashMap.put("url", game.updateUrl);
		mHashMap.put("name", name);
		
		// 构�?�软件下载对话框
		AlertDialog.Builder builder = new Builder(game);
		builder.setTitle("正在下载...");
		// 给下载对话框增加进度�???
		final LayoutInflater inflater = LayoutInflater.from(game);
		View view = inflater.inflate(R.layout.softupdate_view, null);
		builder.setView(view);
		mProgress = (ProgressBar) view.findViewById(R.id.update_progress);
		updateProgressBar=(TextView) view.findViewById(R.id.updateProgressBar);
		
		//game.m_webLayout.addView(view);
		// 屏蔽按键不下�???
		view.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
			}
		});
		builder.setCancelable(false);
		// 取消更新
		builder.setPositiveButton("取消", new android.content.DialogInterface.OnClickListener()
		{
			@Override
			public void onClick(DialogInterface dialog, int which)
			{
				Log.v("update", "取消-----------------");
				restart();
				dialog.dismiss();
				// 设置取消状�??
				cancelUpdate = true;
			}
		});
		
		//builder.setNeutralButton("", listener)
		mDownloadDialog = builder.create();
		
		mDownloadDialog.show();
		// 下载文件
		downloadApk();
	}
	/**
	 * 下载apk文件
	 */
	private void downloadApk()
	{
		// 启动新线程下载软�???
		new downloadApkThread().start();
	}
	/**
	 * 下载文件线程
	 */
	private class downloadApkThread extends Thread
	{
		@Override
		public void run()
		{
			try
			{
				// 判断SD卡是否存在，并且是否具有读写权限
				if (Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED))
				{
					// 获得存储卡的路径
					String sdpath = Environment.getExternalStorageDirectory() + "/";
					mSavePath = sdpath + "download";
					Log.v("update","savePath:"+mSavePath);
					URL url = new URL(mHashMap.get("url"));
					// 创建连接
					URLConnection conn = (URLConnection) url.openConnection();
					conn.connect();
					// 获取文件大小
					int length = conn.getContentLength();
					// 创建输入�???
					InputStream is = conn.getInputStream();
					if(is==null){
						mDownloadDialog.dismiss();
						Toast.makeText(game,"与服务器断开连接!", Toast.LENGTH_LONG).show();
						return;
					}

					File file = new File(mSavePath);
					// 判断文件目录是否存在
					if (!file.exists())
					{
						file.mkdir();
					}
					File apkFile = new File(mSavePath, mHashMap.get("name"));
					FileOutputStream fos = new FileOutputStream(apkFile);
					int count = 0;
					// 缓存
					byte buf[] = new byte[1024];
					// 写入到文件中
					do
					{
						int numread = is.read(buf);
						count += numread;
						// 计算进度条位�???
						progress = (int) (((float) count / length) * 100);
						// 更新进度
						mHandler.sendEmptyMessage(DOWNLOAD);
						if (numread <= 0)
						{
							// 下载完成
							mHandler.sendEmptyMessage(DOWNLOAD_FINISH);
							break;
						}
						// 写入文件
						fos.write(buf, 0, numread);
					} while (!cancelUpdate);// 点击取消就停止下�???.
					fos.close();
					is.close();
				}
			} catch (Exception e)
			{
				mHandler.sendEmptyMessage(DOWNLOADFAIL);
				System.out.println("下载异常:"+e.getMessage());
				e.printStackTrace();
			}
			// 取消下载对话框显�???
			mDownloadDialog.dismiss();
			
		}

	}
	private void showFail(){
		new AlertDialog.Builder(game)
		.setTitle("失败！")
		.setMessage("下载失败，请重新下载")
		.setPositiveButton("确定",
				new DialogInterface.OnClickListener() {

					@Override
					public void onClick(DialogInterface dialog,
							int which) {
//						restart();
						dialog.dismiss();
					}
				}).show();
	}
	private Handler mHandler = new Handler()
	{
		public void handleMessage(Message msg)
		{
			switch (msg.what)
			{
			// 正在下载
			case DOWNLOAD:
				// 设置进度条位�???
				mProgress.setProgress(progress);
				updateProgressBar.setText(progress+"%");
				break;
			case DOWNLOAD_FINISH:
				restart();
				// 安装文件
				installApk();
				break;
			case DOWNLOADFAIL:
				restart();
				//下载失败
				showFail();
				break;
			default:
				break;
			}
		};
	};
	/**
	 * 安装APK文件
	 */
	private void installApk()
	{
		File apkfile = new File(mSavePath, mHashMap.get("name"));
		if (!apkfile.exists())
		{
			return;
		}
		// 通过Intent安装APK文件
		Intent i = new Intent(Intent.ACTION_VIEW);
		i.setDataAndType(Uri.parse("file://" + apkfile.toString()), "application/vnd.android.package-archive");
		game.startActivity(i);
	}
	private void restart(){
		Log.v("update", "restart");
		stopService(new Intent(game,UpdateManager.class));
	}
}
