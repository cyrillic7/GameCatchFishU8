#import "TNAssetsManagerIOS.h"
#import "AFNetworking.h"
#import <CommonCrypto/CommonDigest.h>
#import "SSZipArchive.h"

@implementation TNAssetDataIOS
+ (instancetype)instance {
    return [[[self class] alloc] init];
}
@end


@interface TNAssetsManagerIOS ()

@property (nonatomic, strong) NSMutableArray *assetList;
@property (nonatomic, strong) AFHTTPRequestOperationManager *operationManager;

//检查对应url和版本的包体是否存在, 存在返回true, 否则返回false
- (BOOL)checkAssetPackageExist:(TNAssetDataIOS*) assetData;

//把url和版本号连接起来, hash后作为Zip文件名
- (NSString *)getZipFileName:(TNAssetDataIOS *)assetData;

//获取zip包全路径
- (NSString *)getFullZipPath:(TNAssetDataIOS*)assetData;

//md5 string
- (NSString *)MD5String:(NSString *)text;

//将TNAssetData 转换为 TNAssetDataIOS
- (TNAssetDataIOS *)assetDataToIOS:(TNAssetData*)assetData;

//将TNAssetDataIOS 转换为 TNAssetData
- (TNAssetData *)assetDataToCpp:(TNAssetDataIOS *)assetDataIOS;

//回调成功
- (void)handleSuccessCallback:(TNAssetsManagerActionCode)code index:(int)index delay:(BOOL)delay;
//回调失败
- (void)handleFailureCallback:(TNAssetsManagerActionCode)code index:(int)index;
//回调进度
- (void)handleProgressCallback:(TNAssetsManagerActionCode)code percent:(int)percent;

- (BOOL)removeAssetPackageIOS:(TNAssetDataIOS *)assetData;

//获得所有包得下载进度和
- (int)getTotalDownloadProgress;

@end


@implementation TNAssetsManagerIOS

+ (instancetype)defaultInstance {
//    static TNAssetsManagerIOS *__instance = nil;
//    static dispatch_once_t onceToken;
//    dispatch_once(&onceToken, ^{
        // code to be executed once
//        __instance = [[[self class] alloc] init];
//    });
//    return __instance;
}

+ (instancetype)instance {
    return [[[self class] alloc] init];
}

- (instancetype)init {
    self = [super init];
    if (self) {
        self.assetList = [NSMutableArray array];

        self.operationManager = [AFHTTPRequestOperationManager manager];

        AFHTTPResponseSerializer *s =[AFHTTPResponseSerializer serializer];
        NSMutableSet *types = [NSMutableSet setWithSet:s.acceptableContentTypes];
        [types addObject:@"application/zip"];
        s.acceptableContentTypes = types;
        [self.operationManager setResponseSerializer:s];

        self.operationManager.shouldUseCredentialStorage = NO;
    }
    return self;
}

//代理
- (void)setAssetsManagerDelegateProtocol:(TNAssetsManagerDelegateProtocol*)delegate {
    m_assetDelegate = delegate;
}

- (TNAssetsManagerDelegateProtocol*)getAssetsManagerDelegateProtocol {
    return m_assetDelegate;
}

- (BOOL)checkAssetPackageExist:(TNAssetDataIOS *)assetData {
    if (NULL != assetData) {
        NSString *path = [self getFullZipPath:assetData];
        NSData *data = [NSData dataWithContentsOfFile:path];
        bool exist = [[NSFileManager defaultManager] fileExistsAtPath:path];
        return exist && (NULL != data && [data length] > 0);
    }
    return NO;
}

- (NSString *)getZipFileName:(TNAssetDataIOS *)assetData {
    if (nil != assetData) {
        NSString *name = [assetData.downloadURL stringByAppendingString:assetData.version];
        return [NSString stringWithFormat:@"%@.zip", [self MD5String:name]];
    }
    return nil;
}

- (NSString *)getFullZipPath:(TNAssetDataIOS *)assetData {
    if (nil != assetData) {
        return [assetData.storagePath stringByAppendingPathComponent:[self getZipFileName:assetData]];
    }
    return nil;
}


- (NSString *)MD5String:(NSString *)text {
    if (nil != text) {
        // Create pointer to the string as UTF8
        const char *ptr = [text UTF8String];

        // Create byte array of unsigned chars
        unsigned char md5Buffer[CC_MD5_DIGEST_LENGTH];

        // Create 16 byte MD5 hash value, store in buffer
        CC_MD5(ptr, strlen(ptr), md5Buffer);

        // Convert MD5 value in the buffer to NSString of hex values
        NSMutableString *output = [NSMutableString stringWithCapacity:CC_MD5_DIGEST_LENGTH * 2];
        for(int i = 0; i < CC_MD5_DIGEST_LENGTH; i++)
            [output appendFormat:@"%02x",md5Buffer[i]];

        return output;
    }

    return nil;
}

- (TNAssetDataIOS *)assetDataToIOS:(TNAssetData *)assetData {
    if (NULL != assetData) {
        TNAssetDataIOS *iosData = [TNAssetDataIOS instance];
        iosData.name = [NSString stringWithUTF8String:assetData->getName().c_str()];
        iosData.downloadURL = [NSString stringWithUTF8String:assetData->getDownloadUrl().c_str()];
        iosData.version = [NSString stringWithUTF8String:assetData->getVersion().c_str()];
        iosData.verifyCode = [NSString stringWithUTF8String:assetData->getVerifyCode().c_str()];
        iosData.storagePath = [NSString stringWithUTF8String:assetData->getStoragePath().c_str()];
        return iosData;
    }
    return nil;
}

- (TNAssetData *)assetDataToCpp:(TNAssetDataIOS *)assetDataIOS {
    if (nil != assetDataIOS) {
        TNAssetData *data = TNAssetData::create();
        data->setName([assetDataIOS.name UTF8String]);
        data->setDownloadUrl([assetDataIOS.downloadURL UTF8String]);
        data->setVersion([assetDataIOS.version UTF8String]);
        data->setVerifyCode([assetDataIOS.verifyCode UTF8String]);
        data->setStoragePath([assetDataIOS.storagePath UTF8String]);
        return data;
    }
    return NULL;
}

- (void)handleSuccessCallback:(TNAssetsManagerActionCode)code index:(int)index delay:(BOOL)delay {
    if (delay) {  //add from v2.2.3
        //人为延迟, 使得界面上的进度看上去不像是被卡住了
        int64_t delayInSeconds = 0.3;
        /*
         *@parameter 1,时间参照，从此刻开始计时
         *@parameter 2,延时多久，此处为秒级，还有纳秒等。10ull * NSEC_PER_MSEC
         */
        dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, delayInSeconds);
        dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
            if (NULL != m_assetDelegate) {
                m_assetDelegate->onSuccess(code, index);
            }
        });
    } else {
        dispatch_async(dispatch_get_main_queue(), ^{
            if (NULL != m_assetDelegate) {
                m_assetDelegate->onSuccess(code, index);
            }
        });
    }
}

- (void)handleFailureCallback:(TNAssetsManagerActionCode)code index:(int)index {
    dispatch_async(dispatch_get_main_queue(), ^{
        if (NULL != m_assetDelegate) {
            m_assetDelegate->onError(code, index);
        }
    });
}

- (void)handleProgressCallback:(TNAssetsManagerActionCode)code percent:(int)percent {
    dispatch_async(dispatch_get_main_queue(), ^{
        if (NULL != m_assetDelegate) {
            m_assetDelegate->onProgress(code, percent);
        }
    });
}

- (BOOL)removeAssetPackageIOS:(TNAssetDataIOS *)assetData {
    if (nil != assetData) {
        NSString *path = [self getFullZipPath:assetData];
        return [[NSFileManager defaultManager] removeItemAtPath:path error:nil];
    }
    return NO;
}


- (void)addAssetData:(TNAssetData *)data {
    if (NULL != data) {
        [self.assetList addObject:[self assetDataToIOS:data]];
    }
}

- (void)removeAllAssets {
    [self.assetList removeAllObjects];
}

- (BOOL)removeAssetPackage:(int)index {
    if (index >= 0 && index < [self.assetList count]) {
        TNAssetDataIOS *dataIOS = [self.assetList objectAtIndex:index];
        return [self removeAssetPackageIOS:dataIOS];
    }
    return NO;
}

- (void)removeAllAssetPackages {
    for (int i = 0; i < [self.assetList count]; ++i) {
        TNAssetDataIOS *dataIOS = [self.assetList objectAtIndex:i];
        [self removeAssetPackageIOS:dataIOS];
    }
}

- (int)assetCount {
    return [self.assetList count];
}

- (TNAssetData *)assetWithIndex:(int)index {
    if (index >= 0 && index < [self.assetList count]) {
        TNAssetDataIOS *dataIOS = [self.assetList objectAtIndex:index];
        return [self assetDataToCpp:dataIOS];
    }
    return NULL;
}

- (int)getTotalDownloadProgress {
    int progress = 0;
    
    for (int i = 0; i < [self.assetList count]; i ++) {
        TNAssetDataIOS *data = [self.assetList objectAtIndex:i];
        progress += data.progress;
    }
    
    return progress;
}

- (void)download {
    //记录成功的个数
    __block int successCount = 0;
    
    //记录是否已经回调了失败, 不算Invalid asset data错误
    __block BOOL isCallFailure = NO;

    for (int i = 0; i < [self.assetList count]; ++i) {
        TNAssetDataIOS *assetData = [self.assetList objectAtIndex:i];
        if (assetData.downloadURL.length > 0 && assetData.storagePath.length > 0 && assetData.version.length > 0) {
            //检查包体是否存在并且正确
            bool needDownload = true;
            //已经下载下来的资源包, 如果不完整, 即删除
            if ([self checkAssetPackageExist:assetData]) {
                if ([self verifyWithAsset:assetData]) {
                    needDownload = false;
                } else {
                    [self removeAssetPackageIOS:assetData];
                }
            }
            if (needDownload) { //不存在
                [self downloadWithAsset:assetData completion:^(TNAssetDataIOS *asset, NSError *error) {
                    if (nil == error) {
                        [self handleSuccessCallback:kDownload index:i delay:NO];
                        NSLog(@"[ download success ] => [ downloadUrl: %@, version: %@ ]", assetData.downloadURL, assetData.version);

                        ++ successCount;
                        //全部下载成功了
                        if (successCount == [self.assetList count]) {
                            [self handleSuccessCallback:kDownload index:kAllAssetsFinishIndex delay:NO];
                        }
                    } else {
                        if (!isCallFailure) {
                            isCallFailure = YES;
                            [self handleFailureCallback:kDownload index:i];
                        }
                        
                        NSLog(@"[ download fail, error: %@ ] => [ downloadUrl: %@, version: %@ ]", [error localizedDescription], assetData.downloadURL, assetData.version);
                    }
                } onProgress:^(TNAssetDataIOS *asset, float progress) {
                    if (nil != asset) {
                        asset.progress = (int)(progress * (1 * 100 / [self.assetList count]));
                    }
                    
                    [self handleProgressCallback:kDownload percent:[self getTotalDownloadProgress]];
                }];
            }else {
                ++ successCount;
                assetData.progress = (1 * 100 / [self.assetList count]);
                [self handleProgressCallback:kDownload percent:[self getTotalDownloadProgress]];
                NSLog(@"[ download sucess, exist file ] => [ downloadUrl: %@, version: %@ ]", assetData.downloadURL, assetData.version);

                //全部下载成功了
                if (successCount == [self.assetList count]) {
                    [self handleSuccessCallback:kDownload index:kAllAssetsFinishIndex delay:NO];
                }
            }
        } else {
            [self handleFailureCallback:kInvalidAssetData index:i];
            NSLog(@"[ download fail, invalid asset ] => [ downloadUrl: %@, version: %@ ]", assetData.downloadURL, assetData.version);
        }
    }
}

- (void)verify {
    //开启线程, 执行验证
    dispatch_async(dispatch_get_global_queue(0, 0), ^{
        BOOL isSuccess = YES;
        for (int i = 0; i < [self.assetList count]; ++i) {
            TNAssetDataIOS *assetData = [self.assetList objectAtIndex:i];
            if (![self verifyWithAsset:assetData] ) {
                if (isSuccess) {
                    [self handleFailureCallback:kVerifyPackage index:i];
                } else {
                    [self removeAssetPackage:i];
                }
                isSuccess = NO;
                
//                break;
            } else if(isSuccess){
                int percent = ((i + 1) * 100 / [self.assetList count]);
                [self handleProgressCallback:kVerifyPackage percent:percent];
                [self handleSuccessCallback:kVerifyPackage index:i delay:NO];
//                NSLog(@"[ verify success ] => [ url: %@, version: %@ ]", assetData.downloadURL, assetData.version);
            }
        }
        if (isSuccess) { //表示验证全部成功了
            [self handleSuccessCallback:kVerifyPackage index:kAllAssetsFinishIndex delay:YES];
        }
    });
}

- (void)uncompress {
    dispatch_async(dispatch_get_global_queue(0, 0), ^{
        BOOL isSuccess = YES;
        for (int i = 0; i < [self.assetList count]; ++i) {
            TNAssetDataIOS *assetData = [self.assetList objectAtIndex:i];
            if (![self uncompressWithAsset:assetData]) {
                if (isSuccess) {
                    [self handleFailureCallback:kUncompress index:i];
                } else {
                    [self removeAssetPackage:i];
                }
                isSuccess = NO;
                
//                break;
            } else if(isSuccess){
                int percent = ((i + 1) * 100 / [self.assetList count]);
                [self handleProgressCallback:kUncompress percent:percent];
                [self handleSuccessCallback:kUncompress index:i delay:NO];
                NSLog(@"[ uncompress success ] => [ url: %@, version: %@ ]", assetData.downloadURL, assetData.version);
            }
        }
        if (isSuccess) { //表示解压全部成功了
            [self handleSuccessCallback:kUncompress index:kAllAssetsFinishIndex delay:YES];
        }
    });
}


- (void)downloadWithAsset:(TNAssetDataIOS*)asset
               completion:(void(^)(TNAssetDataIOS *asset, NSError *error))completion
               onProgress:(void(^)(TNAssetDataIOS *asset, float progress))progressBlock {
    if (nil != asset) {
        NSURL *url = [NSURL URLWithString:asset.downloadURL];
        NSString *destination = [self getFullZipPath:asset];
        
        NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];

        AFHTTPRequestOperation *op = [self.operationManager HTTPRequestOperationWithRequest:request success:^(AFHTTPRequestOperation *operation, id responseObject) {
            if (nil != completion) {  //下载成功
                completion(asset, nil);
            }
        } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
//            [self.operationManager.operationQueue cancelAllOperations];
            if (nil != completion) {  //下载失败
                completion(asset, error);
            }
        }];

        [op setDownloadProgressBlock:^(NSUInteger bytesRead, long long totalBytesRead, long long totalBytesExpectedToRead) {
            //下载进度
            float progress = (float)totalBytesRead / (float)totalBytesExpectedToRead;
            if (nil != progressBlock) {
                progressBlock(asset, progress);
            }
        }];

        op.outputStream = [NSOutputStream outputStreamToFileAtPath:destination append:NO];
        [self.operationManager.operationQueue addOperation:op];
    }
}

- (BOOL)verifyWithAsset:(TNAssetDataIOS *)asset {
    if (nil != asset && asset.verifyCode.length > 0) {
        NSString *path = [self getFullZipPath:asset];
        NSData *zipData = [NSData dataWithContentsOfFile:path];
        if (nil != zipData) {
            // Create byte array of unsigned chars
            unsigned char md5Buffer[CC_MD5_DIGEST_LENGTH];

            // Create 16 byte MD5 hash value, store in buffer
            CC_MD5(zipData.bytes, zipData.length, md5Buffer);

            // Convert unsigned char buffer to NSString of hex values
            NSMutableString *output = [NSMutableString stringWithCapacity:CC_MD5_DIGEST_LENGTH * 2];
            for(int i = 0; i < CC_MD5_DIGEST_LENGTH; i++)
                [output appendFormat:@"%02x",md5Buffer[i]];

            bool success = [output isEqualToString:asset.verifyCode];
            if (success) {
                NSLog(@"[ verify success ], [ url: %@, generateCode: %@, serverCode: %@ ]", asset.downloadURL, output, asset.verifyCode);
            } else {
                NSLog(@"[ verify fail ], [ url: %@, generateCode: %@, serverCode: %@ ]", asset.downloadURL, output, asset.verifyCode);
            }
            return success;
        }
    }
    return NO;
}

- (BOOL)uncompressWithAsset:(TNAssetDataIOS *)asset {
    if (nil != asset) {
        NSString *path = [self getFullZipPath:asset];
        return [SSZipArchive unzipFileAtPath:path toDestination:asset.storagePath];
    }
    return NO;
}

@end