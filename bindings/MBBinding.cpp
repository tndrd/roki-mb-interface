#include "PyBinding.hpp"
#include "MotherboardAdapter.hpp"
#include "RokiRcb4Adapter.hpp"

using namespace Roki;

PYBIND11_MODULE(Roki, m)
{
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

  py::class_<IMUInfo>(m, "IMUInfo")
      .def(py::init<>())
      .def_readwrite("First", &IMUInfo::First)
      .def_readwrite("NumAv", &IMUInfo::NumAv)
      .def_readwrite("MaxFrames", &IMUInfo::MaxFrames);

  py::class_<QueueInfo>(m, "QueueInfo")
      .def(py::init<>())
      .def_readwrite("NumRequests", &QueueInfo::NumRequests)
      .def_readwrite("NumResponces", &QueueInfo::NumResponces);

  py::enum_<SerialInterface::TTYConfig::StopbitsCount>(m, "Stopbits", py::arithmetic())
      .value("One", SerialInterface::TTYConfig::StopbitsCount::One)
      .value("Two", SerialInterface::TTYConfig::StopbitsCount::Two);

  py::class_<SerialInterface::TTYConfig>(m, "TTYConfig")
      .def(py::init<>())
      .def_readwrite("Port", &SerialInterface::TTYConfig::Port)
      .def_readwrite("Baudrate", &SerialInterface::TTYConfig::Baudrate)
      .def_readwrite("Stopbits", &SerialInterface::TTYConfig::Stopbits)
      .def_readwrite("ParityBit", &SerialInterface::TTYConfig::ParityBit)
      .def_readwrite("Timeout", &SerialInterface::TTYConfig::Timeout);

  py::register_exception<MotherboardException>(m, "MotherboardException");

  py::class_<MotherboardAdapter>(m, "Motherboard")
      .def(py::init<>())
      .def("Configure", &MotherboardAdapter::Configure)
      .def("GetOrientationBySeq", &MotherboardAdapter::GetOrientationBySeq)
      .def("GetCurrentOrientation", &MotherboardAdapter::GetCurrentOrientation)
      .def("GetIMUInfo", &MotherboardAdapter::GetIMUInfo)
      .def("ResetIMUCounter", &MotherboardAdapter::ResetIMUCounter)
      .def("GetQueueInfo", &MotherboardAdapter::GetQueueInfo);

  /* Rcb4 */

  using Rcb4 = RokiRcb4Adapter;

  py::class_<Rcb4> rcb4(m, "Rcb4");

  py::class_<Rcb4::ServoData>(m, "ServoData")
      .def(py::init<>())
      .def_readwrite("Id", &Rcb4::ServoData::Id)
      .def_readwrite("Sio", &Rcb4::ServoData::Sio)
      .def_readwrite("Data", &Rcb4::ServoData::Data);

  rcb4.def("checkAcknowledge", &Rcb4::checkAcknowledge);
  rcb4.def("getPio", &Rcb4::getPio);
  rcb4.def("setPio", &Rcb4::setPio);
  rcb4.def("getPioMode", &Rcb4::getPioMode);
  rcb4.def("setPioMode", &Rcb4::setPioMode);
  rcb4.def("setSingleServo", static_cast<bool (Rcb4::*)(Rcb4::ServoData, uint8_t)>(&Rcb4::setSingleServo));
  rcb4.def("setFreeSingleServo", &Rcb4::setFreeSingleServo);
  rcb4.def("setHoldSingleServo", &Rcb4::setHoldSingleServo);
  rcb4.def("setServoPos", &Rcb4::setServoPos);
  rcb4.def("setServoPosAsync", &Rcb4::setServoPosAsync);
  rcb4.def("setFreePos", &Rcb4::setFreePos);
  rcb4.def("setHoldPos", &Rcb4::setHoldPos);
  rcb4.def("getSinglePos", &Rcb4::getSinglePos);
  rcb4.def("setServoSpeed", &Rcb4::setServoSpeed);
  rcb4.def("setServoStretch", &Rcb4::setServoStretch);

  // rcb4.
}