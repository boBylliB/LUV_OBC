	// Sam the intention of this code is to take in the IMU data and ouput it and to read data from the motor encoders
  //Including the necessary libraries for input/output operations.
#include <iostream>
	
  // Including the necessary libraries for SPI communication.
#include <iostream>
#include <vector>
 
// Using a namespace to prevent potential naming conflicts and to logically group
// associated functionalities.
namespace MotorEncoder {
 
    /**
    * @class MotorEncoderSPI
    * Represents a class for receiving data from motor encoders using SPI communication.
    */
    class MotorEncoderSPI {
    private:
        // SPI configuration parameters
        int spiChannel;
        int spiSpeed;
        int spiMode;
 
    public:
        /**
        * Constructs a MotorEncoderSPI object with the specified SPI configuration.
        *
        * @param channel The SPI channel number.
        * @param speed The SPI speed in Hz.
        * @param mode The SPI mode (0, 1, 2, or 3).
        */
        MotorEncoderSPI(int channel, int speed, int mode) : spiChannel(channel), spiSpeed(speed), spiMode(mode) {
            // Initialize SPI communication with the provided configuration
            std::cout << "Initializing SPI communication on channel " << spiChannel << " with speed " << spiSpeed << " Hz and mode " << spiMode << ".\n";
        }
 
        /**
        * Receives data from motor encoders using SPI communication.
        *
        * @return std::vector<int> The data received from the motor encoders.
        */
        std::vector<int> ReceiveData() {
            // Simulating data reception from motor encoders
            std::vector<int> receivedData = {100, 200, 150, 180}; // Sample data
            std::cout << "Received data from motor encoders: ";
            for (int data : receivedData) {
                std::cout << data << " ";
            }
            std::cout << "\n";
 
            return receivedData;
        }
    };
}
 
// Using a namespace to prevent potential naming conflicts and to logically group
// associated functionalities.
namespace IMUData {
 
    /**
    * @class RawIMUData
    * Represents raw data from an Inertial Measurement Unit (IMU) containing accelerometer and gyroscope data.
    */
    class RawIMUData {
    private:
        // Accelerometer data.
        double accelerometerData[3]; // x, y, z axes
 
        // Gyroscope data.
        double gyroscopeData[3]; // x, y, z axes
 
    public:
        /**
        * Constructs raw IMU data with initial accelerometer and gyroscope values.
        *
        * @param accelX Accelerometer data along the x-axis.
        * @param accelY Accelerometer data along the y-axis.
        * @param accelZ Accelerometer data along the z-axis.
        * @param gyroX Gyroscope data along the x-axis.
        * @param gyroY Gyroscope data along the y-axis.
        * @param gyroZ Gyroscope data along the z-axis.
        */
        RawIMUData(double accelX, double accelY, double accelZ, double gyroX, double gyroY, double gyroZ) {
            accelerometerData[0] = accelX;
            accelerometerData[1] = accelY;
            accelerometerData[2] = accelZ;
 
            gyroscopeData[0] = gyroX;
            gyroscopeData[1] = gyroY;
            gyroscopeData[2] = gyroZ;
        }
 
        /**
        * Outputs the raw IMU data containing accelerometer and gyroscope values.
        */
        // This function can probably be erased: 
        void OutputRawData() {
            std::cout << "Accelerometer Data (x, y, z): " << accelerometerData[0] << ", " << accelerometerData[1] << ", " << accelerometerData[2] << std::endl;
            std::cout << "Gyroscope Data (x, y, z): " << gyroscopeData[0] << ", " << gyroscopeData[1] << ", " << gyroscopeData[2] << std::endl;
        }
    };
}
 
int main() {
    // Example: Reading and outputting raw IMU data
    // Creating an instance of RawIMUData with sample accelerometer and gyroscope values.
    IMUData::RawIMUData imuData(1.2, -3.4, 9.8, 0.01, -0.02, 0.03);
 
    // Outputting the raw IMU data.
    imuData.OutputRawData();
    
    // Example: Motor Encoder Data Reception
    // Using the MotorEncoderSPI class to receive data from motor encoders via SPI communication.
    {
        MotorEncoder::MotorEncoderSPI motorEncoder(0, 1000000, 0); // SPI channel 0, speed 1 MHz, mode 0
        std::vector<int> motorData = motorEncoder.ReceiveData();
    }
    return 0;
}