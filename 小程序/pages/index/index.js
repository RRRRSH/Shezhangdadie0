const app = getApp()

Page({
  data: {
    switchValue: "none",
    radioValue: "none",
    checkBoxValue: "none",
    timePickerValue: "09:30",
    sliderValue: 0
  },
  switchChange: function (e) {
    this.setData({
      switchValue: e.detail.value
    }),
      console.log('switch发生change事件，携带value值为：', e.detail.value)
  },
 

  onLoad: function (options) {
  },
})
