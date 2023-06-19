//app.js
App({
  onLaunch: function () {
    //调用API从本地缓存中获取数据
    var logs = wx.getStorageSync('logs') || []
    logs.unshift(Date.now())
    wx.setStorageSync('logs', logs)
  },
  getUserInfo:function(cb){
    var that = this
    if(this.globalData.userInfo){
      typeof cb == "function" && cb(this.globalData.userInfo)
    }else{
      //调用登录接口
      wx.login({
        success: function () {
          wx.getUserInfo({
            success: function (res) {
              that.globalData.userInfo = res.userInfo
              typeof cb == "function" && cb(that.globalData.userInfo)
            }
          })
        }
      })
    }
  },
  globalData:{
    userInfo:null,
    awardsConfig: {},
    runDegs: 0
  }
})
 //背景音乐地址bgrAudioContext: "",bg_is_play: false, //背景音乐是否播放},onLaunch: function() { //启动小程序执行方法this.createBgm(); //背景音乐},createBgm(){this.globalData.bgrAudioContext = wx.createInnerAudioContext();this.globalData.bgrAudioContext.src = this.globalData.bgm_src;this.globalData.bgrAudioContext.loop = true;this.globalData.bg_is_play = false; //默认暂停}
