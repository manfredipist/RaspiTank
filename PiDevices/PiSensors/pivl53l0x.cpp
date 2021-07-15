#include "pivl53l0x.h"

PiVL53L0X::PiVL53L0X(uint8_t i2c_bus, uint8_t i2c_address, QObject *parent) : QObject(parent), i2c_bus(i2c_bus), i2c_address(i2c_address)
{
    i2c = new QI2C(i2c_bus,i2c_address,this);
}

PiVL53L0X::~PiVL53L0X(){
    i2c->i2cClose();
}

void PiVL53L0X::initialize(){
    dataInit();
    staticInit();
    performRefCalibration();
    performRefSpadManagement();

    qDebug()<<"VL53L0X Initialization: SUCCESS";
}

double PiVL53L0X::getDistance(){
    if(isDataReady()){
        performRefSignalMeasurement();
        return measurement;
    }
    else
        return -1;
}

bool PiVL53L0X::isDataReady(){
    uint8_t status = i2c->readByte8(VL53L0X_REG_RESULT_RANGE_STATUS);
    return status & 0x01;
}


void PiVL53L0X::dataInit(){
    // set i2c standard mode
    i2c->writeByte8(0x88, 0x00);

    // read whoami
    qInfo()<<i2c->readByte8(0xC0);

    // use internal default settings
    i2c->writeByte8(0x80, 0x01);
    i2c->writeByte8(0xFF, 0x01);
    i2c->writeByte8(0x00, 0x00);

    i2c->readByte8(0x91);

    i2c->writeByte8(0x00, 0x01);
    i2c->writeByte8(0xFF, 0x00);
    i2c->writeByte8(0x80, 0x00);

    i2c->writeByte8(VL53L0X_REG_SYSTEM_SEQUENCE_CONFIG, 0xFF);
}

void PiVL53L0X::staticInit(){
    i2c->writeByte8(0xFF, 0x01);
    i2c->readByte8(0x84);
    i2c->writeByte8(0xFF, 0x00);

    // read the sequence config and save it
    static_seq_config = i2c->readByte8(VL53L0X_REG_SYSTEM_SEQUENCE_CONFIG);
}

void PiVL53L0X::performRefCalibration(){
    performVhvCalibration();
    performPhaseCalibration();

    // restore static sequence config
    i2c->writeByte8(VL53L0X_REG_SYSTEM_SEQUENCE_CONFIG, static_seq_config);
}

void PiVL53L0X::performVhvCalibration(){
    // run vhv
    i2c->writeByte8(VL53L0X_REG_SYSTEM_SEQUENCE_CONFIG, 0x01);

    performSingleRefCalibration(0x40);

    // read vhv from device
    refCalibrationIo(0xCB);

    // restore static sequence config
    i2c->writeByte8(VL53L0X_REG_SYSTEM_SEQUENCE_CONFIG, static_seq_config);

}

void PiVL53L0X::performPhaseCalibration(){
    // run phase cal
    i2c->writeByte8(VL53L0X_REG_SYSTEM_SEQUENCE_CONFIG, 0x02);

    performSingleRefCalibration(0x0);

    // read phase cal from device
    refCalibrationIo(0xEE);

    // restore static sequence config
    i2c->writeByte8(VL53L0X_REG_SYSTEM_SEQUENCE_CONFIG, static_seq_config);

}

void PiVL53L0X::performSingleRefCalibration(uint8_t byte){
    i2c->writeByte8(VL53L0X_REG_SYSRANGE_START, VL53L0X_REG_SYSRANGE_MODE_START_STOP | byte);
    i2c->writeByte8(VL53L0X_REG_SYSRANGE_START, 0x00);
}

void PiVL53L0X::refCalibrationIo(uint8_t byte){
    // read vhv from device
    i2c->writeByte8(0xFF, 0x01);
    i2c->writeByte8(0x00, 0x00);
    i2c->writeByte8(0xFF, 0x00);

    i2c->readByte8(byte);

    i2c->writeByte8(0xFF, 0x01);
    i2c->writeByte8(0x00, 0x00);
    i2c->writeByte8(0xFF, 0x00);
}

void PiVL53L0X::performRefSpadManagement(){
    i2c->writeByte8(0xFF, 0x01);
    i2c->writeByte8(VL53L0X_REG_DYNAMIC_SPAD_REF_EN_START_OFFSET, 0x00);
    i2c->writeByte8(VL53L0X_REG_DYNAMIC_SPAD_NUM_REQUESTED_REF_SPAD, 0x2C);
    i2c->writeByte8(0xFF, 0x00);
    i2c->writeByte8(VL53L0X_REG_GLOBAL_CONFIG_REF_EN_START_SELECT, 0xB4);
    i2c->writeByte8(VL53L0X_REG_POWER_MANAGEMENT_GO1_POWER_FORCE, 0);

    performRefCalibration();
    performRefSignalMeasurement();
}

void PiVL53L0X::performRefSignalMeasurement(){
    i2c->writeByte8(VL53L0X_REG_SYSTEM_SEQUENCE_CONFIG, 0xC0);

    performSingleRangingMeasurement();

    i2c->writeByte8(0xFF, 0x01);
    i2c->writeByte8(0xFF, 0x00);

    // restore static sequence config
    i2c->writeByte8(VL53L0X_REG_SYSTEM_SEQUENCE_CONFIG, static_seq_config);
}

void PiVL53L0X::performSingleRangingMeasurement(){
    performSingleMeasurement();
    getRangingMeasurementData();
}

void PiVL53L0X::performSingleMeasurement(){
    startMeasurement();
}

void PiVL53L0X::startMeasurement(){
    i2c->writeByte8(0x80, 0x01);
    i2c->writeByte8(0xFF, 0x01);
    i2c->writeByte8(0x00, 0x00);

    i2c->readByte8(0x91);

    i2c->writeByte8(0x00, 0x01);
    i2c->writeByte8(0xFF, 0x00);
    i2c->writeByte8(0x80, 0x00);

    // device mode single ranging
    i2c->writeByte8(VL53L0X_REG_SYSRANGE_START, 0x01);
}

void PiVL53L0X::getRangingMeasurementData(){
    uint8_t raw_data[12];
    i2c->readBytes8(VL53L0X_REG_RESULT_RANGE_STATUS, 12, raw_data);

    uint16_t range_millimeter = (raw_data[11]<<8)|raw_data[10];
    uint16_t signal_rate = (raw_data[7]<<8)|raw_data[6];
    uint16_t ambient_rate = (raw_data[9]<<8)|raw_data[8];
    uint16_t effective_spad_rtn_count = (raw_data[3]<<8)|raw_data[2];
    uint8_t device_range_status = raw_data[0];

//    qInfo()<<"Range:"<<range_millimeter<<"mm";
//    qInfo()<<"Signal Rate:"<<signal_rate;
//    qInfo()<<"Ambient Rate:"<<ambient_rate;
//    qInfo()<<"Ambient Rate:"<<effective_spad_rtn_count;
//    qInfo()<<"Status"<<device_range_status;

    // update measurement
    measurement = range_millimeter;
}
