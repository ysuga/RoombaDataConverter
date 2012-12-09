// -*- C++ -*-
/*!
 * @file  RoombaDataConverter.cpp
 * @brief Roomba Data Converter RTC
 * @date $Date$
 *
 * $Id$
 */

#include "RoombaDataConverter.h"
#include <fstream>
// Module specification
// <rtc-template block="module_spec">
static const char* roombadataconverter_spec[] =
  {
    "implementation_id", "RoombaDataConverter",
    "type_name",         "RoombaDataConverter",
    "description",       "Roomba Data Converter RTC",
    "version",           "1.0.0",
    "vendor",            "SUGAR SWEET ROBOTICS",
    "category",          "Experimenta",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
RoombaDataConverter::RoombaDataConverter(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_imageInIn("imageIn", m_imageIn),
    m_velocityInIn("velocityIn", m_velocityIn),
    m_imageOutOut("imageOut", m_imageOut),
    m_velocityOutOut("velocityOut", m_velocityOut)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
RoombaDataConverter::~RoombaDataConverter()
{
}



RTC::ReturnCode_t RoombaDataConverter::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("imageIn", m_imageInIn);
  addInPort("velocityIn", m_velocityInIn);
  
  // Set OutPort buffer
  addOutPort("imageOut", m_imageOutOut);
  addOutPort("velocityOut", m_velocityOutOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t RoombaDataConverter::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RoombaDataConverter::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RoombaDataConverter::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t RoombaDataConverter::onActivated(RTC::UniqueId ec_id)
{

  //  m_velocityOut.data.length(3);
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RoombaDataConverter::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RoombaDataConverter::onExecute(RTC::UniqueId ec_id)
{
  if(m_velocityInIn.isNew()) {
    m_velocityInIn.read();
    m_velocityOut.data.vx = m_velocityIn.data[0];
    m_velocityOut.data.vy = m_velocityIn.data[1];
    m_velocityOut.data.va = m_velocityIn.data[2];
    m_velocityOutOut.write();
  }

  if(m_imageInIn.isNew()) {
    m_imageInIn.read();
    std::cout << "WxH = " << m_imageIn.width << "x" << m_imageIn.height << " Size = " << m_imageIn.pixels.length() << std::endl;
    cv::Mat mat(m_imageIn.height, m_imageIn.width, CV_8UC3, &(m_imageIn.pixels[0]));
    
    std::vector<uchar> buff;//buffer for coding
    std::vector<int> param = std::vector<int>(2);
    param[0]=CV_IMWRITE_JPEG_QUALITY;
    param[1]=60;//default(95) 0-100
 
    cv::imencode(".jpg",mat,buff,param);
    int size = buff.size();
    std::cout << "Sending JPEG data (size = " << size << std::endl;
    m_imageOut.data.length(size);
    for(int i = 0;i < size;i++) {
      m_imageOut.data[i] = buff[i];
    }
    m_imageOutOut.write();
    static int i;
    if(!i) {
      std::ofstream fout("test.jpg");
      fout.write((char*)&(m_imageOut.data[0]), m_imageOut.data.length());
    }
    //if(m_imageIn.pixels.length() != m_imageOut.data.length()) {
    //m_imageOut.data.length(m_imageIn.pixels.length());
    //}
    //if(m_imageIn.width *  m_imageIn.height != m_imageOut.data.length() ) {
    //      m_imageOut.data.length(m_imageIn.width * m_imageIn.height);
    //    }
    //    for(int i = 0;i < m_imageOut.data.length();i++) {
    //      uint32_t r = m_imageIn.pixels[i*3+0];
    //      uint32_t g = m_imageIn.pixels[i*3+1];
    //      uint32_t b = m_imageIn.pixels[i*3+2];
    //      uint32_t a = 0xff;
    //      m_imageOut.data[i] = (a << 24) || (r << 16) || (g << 8) || (b << 0);
    //    }
    //memcpy((void*)(&(m_imageOut.data[0])), (void*)(&(m_imageIn.pixels[0])), m_imageIn.pixels.length());
    //    m_imageOutOut.write();
  }

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t RoombaDataConverter::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RoombaDataConverter::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RoombaDataConverter::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RoombaDataConverter::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RoombaDataConverter::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void RoombaDataConverterInit(RTC::Manager* manager)
  {
    coil::Properties profile(roombadataconverter_spec);
    manager->registerFactory(profile,
                             RTC::Create<RoombaDataConverter>,
                             RTC::Delete<RoombaDataConverter>);
  }
  
};


