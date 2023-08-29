#include "PyBinding.hpp"
#include "MotherboardAdapter.hpp"

using namespace Roki;

PYBIND11_MODULE(Roki, m) {
  m.doc() = "Roki motherboard interface";

  py::class_<Quaternion>(m, "Quaternion")
    .def(py::init<>())
    .def_readwrite("X", &Quaternion::X)
    .def_readwrite("Y", &Quaternion::X)
    .def_readwrite("Z", &Quaternion::X)
    .def_readwrite("W", &Quaternion::X);

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
}