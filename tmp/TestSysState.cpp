#include "Motherboard.hpp"
#include "MotherboardAliases.hpp"

#include <chrono>
#include <thread>

int main()
{
    Roki::Motherboard mb;
    Roki::TTYConfig serviceConfig;
    Roki::TTYConfig streamConfig;

    serviceConfig.Port = "/dev/ttyAMA2";
    serviceConfig.Baudrate = 115200;
    serviceConfig.ParityBit = true;
    serviceConfig.Stopbits = serviceConfig.STOPBITS_ONE;
    serviceConfig.Timeout = 2;

    streamConfig.Port = "/dev/ttyAMA1";
    streamConfig.Baudrate = 115200;
    streamConfig.ParityBit = true;
    streamConfig.Stopbits = streamConfig.STOPBITS_ONE;
    streamConfig.Timeout = 2;

    if (!mb.Configure(serviceConfig, streamConfig))
    {
        std::cout << mb.GetError() << std::endl;
        exit(1);
    }

    if (!mb.Start())
    {
        std::cout << mb.GetError() << std::endl;
        exit(1);
    }

    while (true)
    {
        Roki::SystemState state;

        if (!mb.GetState(state))
        {
            std::cout << mb.GetError() << std::endl;
            exit(1);
        }

        std::cout << "Body info: " << std::endl;
        std::cout << " Requests: " << state.Body.NumRequests << std::endl;
        std::cout << " Responces: " << state.Body.NumResponces << std::endl;

        std::cout << std::endl;

        std::cout << "Imu info: " << std::endl;
        std::cout << "  First:  " << state.Imu.First << std::endl;
        std::cout << "  NumAv:  " << state.Imu.NumAv << std::endl;
        std::cout << "  MaxFr:  " << state.Imu.MaxFrames << std::endl;

        std::cout << std::endl;

        std::cout << "Quaternion: " << std::endl;
        std::cout << "  x:  " << state.Orientation.Orientation.X << std::endl;
        std::cout << "  y:  " << state.Orientation.Orientation.Y << std::endl;
        std::cout << "  z:  " << state.Orientation.Orientation.Z << std::endl;
        std::cout << "  w:  " << state.Orientation.Orientation.W << std::endl;
        // std::cout << "  Acc: " << state.Orientation.Orientation.Accuracy << std::endl;

        std::cout << "Timestamp: " << std::endl;
        std::cout << "  s:  " << state.Orientation.Timestamp.TimeS << std::endl;
        std::cout << "  ns: " << state.Orientation.Timestamp.TimeNS << std::endl;

        std::cout << "Sensor ID: " << +state.Orientation.SensorID << std::endl;

        std::cout << "---------------" << std::endl;
    }
    return 0;
}
