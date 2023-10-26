#include "MotherboardAdapter.hpp"
#include "PyBinding.hpp"
#include "RokiRcb4Adapter.hpp"

using namespace Roki;

PYBIND11_MODULE(Roki, m) {
  m.doc() = "Roki motherboard interface";

  /* Motherboard */

  py::class_<Quaternion>(m, "Quaternion")
      .def(py::init<>())
      .def_readwrite("X", &Quaternion::X)
      .def_readwrite("Y", &Quaternion::Y)
      .def_readwrite("Z", &Quaternion::Z)
      .def_readwrite("W", &Quaternion::W);

  py::class_<Timestamp>(m, "Timestamp")
      .def(py::init<>())
      .def_readwrite("TimeS", &Timestamp::TimeS)
      .def_readwrite("TimeNS", &Timestamp::TimeNS);

  py::class_<IMUFrame>(m, "IMUFrame")
      .def(py::init<>())
      .def_readwrite("Orientation", &IMUFrame::Orientation)
      .def_readwrite("Timestamp", &IMUFrame::Timestamp)
      .def_readwrite("SensorID", &IMUFrame::SensorID);

  py::class_<FrameContainerInfo>(m, "FrameContainerInfo")
      .def(py::init<>())
      .def_readwrite("First", &FrameContainerInfo::First)
      .def_readwrite("NumAv", &FrameContainerInfo::NumAv)
      .def_readwrite("MaxFrames", &FrameContainerInfo::MaxFrames);

  py::class_<BodyQueueInfo>(m, "BodyQueueInfo")
      .def(py::init<>())
      .def_readwrite("Size", &BodyQueueInfo::Size)
      .def_readwrite("Capacity", &BodyQueueInfo::Capacity);

  py::enum_<MbSerial::TTYConfig::StopbitsCount>(m, "Stopbits", py::arithmetic())
      .value("One", MbSerial::TTYConfig::StopbitsCount::One)
      .value("Two", MbSerial::TTYConfig::StopbitsCount::Two);

  py::class_<MbSerial::TTYConfig>(m, "TTYConfig")
      .def(py::init<>())
      .def_readwrite("Port", &MbSerial::TTYConfig::Port)
      .def_readwrite("Baudrate", &MbSerial::TTYConfig::Baudrate)
      .def_readwrite("Stopbits", &MbSerial::TTYConfig::Stopbits)
      .def_readwrite("ParityBit", &MbSerial::TTYConfig::ParityBit)
      .def_readwrite("Timeout", &MbSerial::TTYConfig::Timeout);

  using MA = MotherboardAdapter;

  py::class_<MA>(m, "Motherboard")
      .def(py::init<>())
      .def("GetIMUFrame", &MA::GetIMUFrame)
      .def("GetBodyFrame", &MA::GetBodyFrame)
      .def("GetIMUContainerInfo", &MA::GetIMUContainerInfo)
      .def("GetBodyContainerInfo", &MA::GetBodyContainerInfo)
      .def("ResetStrobeContainers", &MA::ResetStrobeContainers)
      .def("SetIMUStrobeOffset", &MA::SetIMUStrobeOffset)
      .def("SetBodyStrobeOffset", &MA::SetBodyStrobeOffset)
      .def("GetIMULatest", &MA::GetIMULatest)
      .def("GetStrobeWidth", &MA::GetStrobeWidth)
      .def("ConfigureStrobeFilter", &MA::ConfigureStrobeFilter)
      .def("GetBodyQueueInfo", &MA::GetBodyQueueInfo)
      .def("SetBodyQueuePeriod", &MA::SetBodyQueuePeriod)
      .def("GetVersion", &MA::GetVersion);

  /* Rcb4 */

  using Rcb4 = RokiRcb4Adapter;

  py::class_<Rcb4> rcb4(m, "Rcb4");

  py::class_<Rcb4::ServoData>(rcb4, "ServoData")
      .def(py::init<>())
      .def_readwrite("Id", &Rcb4::ServoData::Id)
      .def_readwrite("Sio", &Rcb4::ServoData::Sio)
      .def_readwrite("Data", &Rcb4::ServoData::Data);

  rcb4.def(py::init<MotherboardAdapter &>());
  rcb4.def("checkAcknowledge", &Rcb4::checkAcknowledge);
  rcb4.def("getPio", &Rcb4::getPio);
  rcb4.def("setPio", &Rcb4::setPio);
  rcb4.def("getPioMode", &Rcb4::getPioMode);
  rcb4.def("setPioMode", &Rcb4::setPioMode);
  rcb4.def("setSingleServo",
           static_cast<bool (Rcb4::*)(Rcb4::ServoData, uint8_t)>(
               &Rcb4::setSingleServo));
  rcb4.def("setFreeSingleServo", &Rcb4::setFreeSingleServo);
  rcb4.def("setHoldSingleServo", &Rcb4::setHoldSingleServo);
  rcb4.def("setServoPos", &Rcb4::setServoPos);
  rcb4.def("setServoPosAsync", &Rcb4::setServoPosAsync);
  rcb4.def("setFreePos", &Rcb4::setFreePos);
  rcb4.def("setHoldPos", &Rcb4::setHoldPos);
  rcb4.def("getSinglePos", &Rcb4::getSinglePos);
  rcb4.def("setServoSpeed", &Rcb4::setServoSpeed);
  rcb4.def("setServoStretch", &Rcb4::setServoStretch);
  rcb4.def("moveRamToComCmdSynchronize", &Rcb4::moveRamToComCmdSynchronize);
  rcb4.def("motionPlay", &Rcb4::motionPlay);

  rcb4.def("getError", &Rcb4::GetError);
  rcb4.def("isOk", &Rcb4::IsOk);
}