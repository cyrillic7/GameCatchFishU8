//
// Created by wudb on 14/12/9.
//

#import <Foundation/Foundation.h>
#include "TNAssetData.h"
#include "TNAssetDefine.h"

@interface TNAssetDataIOS : NSObject

@property (nonatomic, copy) NSString *name;
@property (nonatomic, copy) NSString *downloadURL;
@property (nonatomic, copy) NSString *version;
@property (nonatomic, copy) NSString *verifyCode;
@property (nonatomic, copy) NSString *storagePath;

@property (nonatomic, assign) int       progress;

+ (instancetype)instance;

@end

@interface TNAssetsManagerIOS : NSObject {
    TNAssetsManagerDelegateProtocol *m_assetDelegate;
}

//资源管理类实例
+ (instancetype)defaultInstance;
+ (instancetype)instance;

//初始化方法
- (instancetype)init;

//代理
- (void)setAssetsManagerDelegateProtocol:(TNAssetsManagerDelegateProtocol*)delegate;
- (TNAssetsManagerDelegateProtocol*)getAssetsManagerDelegateProtocol;

//添加资源, 多线程不安全
- (void)addAssetData:(TNAssetData*)data;

//移除资源列表中得所有资源
- (void)removeAllAssets;

//删除资源对应下载的包体
- (BOOL)removeAssetPackage:(int)index;
- (void)removeAllAssetPackages;

//获取需要下载更新的资源列表的资源个数
- (int)assetCount;

//根据index获取asset data
- (TNAssetData*)assetWithIndex:(int)index;

//下载全部加进列表的资源, 如果其中一个失败, 则会停止下载
- (void)download;

//验证全部加进列表的资源, 如果其中一个失败, 则会停止验证
- (void)verify;

//解压全部加进列表的资源, 如果其中一个失败, 则会停止解压
- (void)uncompress;

//下载资源
//asset   资源数据
//completion    回调, error为nil表示成功, 否则失败
//progress      下载进度
- (void)downloadWithAsset:(TNAssetDataIOS *)asset
             completion:(void(^)(TNAssetDataIOS *asset, NSError *error))completion
             onProgress:(void(^)(TNAssetDataIOS *asset, float progress))progress;

//验证资源
//asset   资源数据
- (BOOL)verifyWithAsset:(TNAssetDataIOS *)asset;

//解压
//asset   资源数据
- (BOOL)uncompressWithAsset:(TNAssetDataIOS *)asset;

@end