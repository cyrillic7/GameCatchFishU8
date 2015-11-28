#ifndef __TNAssetDefine__
#define __TNAssetDefine__

//一组资源全部完成下载或者验证或者解压事件后对应返回的index
#define kAllAssetsFinishIndex       -1

//资源管理类相应事件动作
typedef enum {
    //创建文件错误
    kCreateFileError,
    //网络错误
    kNetworkError,
    //传入的asset data不合法
    kInvalidAssetData,
    //下载
    kDownload,
    //验证
    kVerifyPackage,
    //解压
    kUncompress,
}TNAssetsManagerActionCode;

class TNAssetsManagerDelegateProtocol
{
public:
    //错误回调
    //code 错误码
    //index 相应的事件(下载, 验证, 解压, 删除)下出错的index
    virtual void onError(TNAssetsManagerActionCode code, int index) {};

    //进度回调
    //percent   0-100
    virtual void onProgress(TNAssetsManagerActionCode code, int percent) {};

    //成功回调, 每一个资源的相应事件成功才会回调此方法
    virtual void onSuccess(TNAssetsManagerActionCode code, int index) {};
};

#endif