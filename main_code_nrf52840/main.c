
//Include Modules:
#include "custom_log.h"
#include "custom_timer.h"
#include "custom_detection_system_struct_data.h"
#include "custom_qspi.h"
#include "boards.h"
#include "custom_twi.h"
#include "custom_ble_manager.h"
#include "custom_device_status_struct_data.h"


//Main Function:
int main(void)
{

    //Init Log for debugging:
    logInit();

    //Print Message:
    NRF_LOG_INFO("");
    NRF_LOG_INFO("*********************");
    NRF_LOG_INFO("**** Setup Start ****");
    NRF_LOG_INFO("*********************");
    NRF_LOG_INFO("");


    //Init Timers:
    timerInit();

    //Initialize other Peripherals:
    qspiInit();                       //Init QSPI for external flash (qspiInit() must be after the timers are started, otherwise the qspi throws error on the first burn (but after reset works ok)).
    bsp_board_init(BSP_INIT_LEDS);    //Init digital output
    twiInit();                        //Init TWI for ADS1015

    //Initialize BLE:
    ble_stack_init();
    gap_params_init();
    gatt_init();
    services_init();
    advertising_init();
    conn_params_init();

    //Start Advertising:
    advertising_start();

    //Set initial status of the device:
    deviceStatus_saveStructData_init();


    //Print Message:
    NRF_LOG_INFO("");
    NRF_LOG_INFO("**********************");
    NRF_LOG_INFO("***** Loop Start *****");
    NRF_LOG_INFO("**********************");
    NRF_LOG_INFO("");

    //Enter main loop:
    while(1)
    {
        ////////////////////////////////////////////////////////////////
        /// Detection System Task //////////////////////////////////////
        ////////////////////////////////////////////////////////////////
        static uint8_t counter = 0;

        if(deviceStatus_getStructData_isMeasuring())
        {
            if(timerDetectionSystem_GetFlag())
            {

                if(secondsGetFlag())
                {
                    secondsClearFlag();
                
                    static uint8_t mosfetActuator;
                    static uint16_t lightSensor;
                    static uint16_t time;
                
                    switch(counter) 
                    {
                        case 0 :
                        {
                            //Save struct data before and Change the mosfet state for detection:
                            mosfetActuator = (uint8_t) bsp_board_led_state_get(counter);
                            lightSensor    = ads1015Read(counter);
                            NRF_LOG_INFO("lightSensor[%d]: %d", counter, lightSensor);
                            detectionSystem_saveStructData_before(counter, mosfetActuator, lightSensor);
                            bsp_board_led_on(counter);
                            
                            //Increase counter:
                            counter++;
                            break;
                        }
                        case 1 :
                        {
                            //Save struct data after and Change the mosfet state for non-detection:
                            mosfetActuator = (uint8_t) bsp_board_led_state_get(counter-1);
                            lightSensor    = ads1015Read(counter-1);
                            time           = (uint16_t) secondsGetTime();
                            NRF_LOG_INFO("lightSensor[%d]: %d", (counter-1), lightSensor);
                            detectionSystem_saveStructData_after((counter-1), mosfetActuator, lightSensor, time);
                            bsp_board_led_off(counter-1);
                
                            //Save struct data before and Change the mosfet state for detection:
                            mosfetActuator = (uint8_t) bsp_board_led_state_get(counter);
                            lightSensor    = ads1015Read(counter);
                            NRF_LOG_INFO("lightSensor[%d]: %d", counter, lightSensor);
                            detectionSystem_saveStructData_before(counter, mosfetActuator, lightSensor);
                            bsp_board_led_on(counter);
                
                            //Increase counter:
                            counter++;
                            break;
                        }
                        case 2 :
                        {
                            //Save struct data after and Change the mosfet state for non-detection:
                            mosfetActuator = (uint8_t) bsp_board_led_state_get(counter-1);
                            lightSensor    = ads1015Read(counter-1);
                            time           = (uint16_t) secondsGetTime();
                            NRF_LOG_INFO("lightSensor[%d]: %d", (counter-1), lightSensor);
                            detectionSystem_saveStructData_after((counter-1), mosfetActuator, lightSensor, time);
                            bsp_board_led_off(counter-1);
                
                            //Save struct data before and Change the mosfet state for detection:
                            mosfetActuator = (uint8_t) bsp_board_led_state_get(counter);
                            lightSensor    = ads1015Read(counter);
                            NRF_LOG_INFO("lightSensor[%d]: %d", counter, lightSensor);
                            detectionSystem_saveStructData_before(counter, mosfetActuator, lightSensor);
                            bsp_board_led_on(counter);
                
                            //Increase counter:
                            counter++;
                            break;
                        }
                        case 3 :
                        {
                            //Save struct data after and Change the mosfet state for non-detection:
                            mosfetActuator = (uint8_t) bsp_board_led_state_get(counter-1);
                            lightSensor    = ads1015Read(counter-1);
                            time           = (uint16_t) secondsGetTime();
                            NRF_LOG_INFO("lightSensor[%d]: %d", (counter-1), lightSensor);
                            detectionSystem_saveStructData_after((counter-1), mosfetActuator, lightSensor, time);
                            bsp_board_led_off(counter-1);
                
                            //Save struct data before and Change the mosfet state for detection:
                            mosfetActuator = (uint8_t) bsp_board_led_state_get(counter);
                            lightSensor    = ads1015Read(counter);
                            NRF_LOG_INFO("lightSensor[%d]: %d", counter, lightSensor);
                            detectionSystem_saveStructData_before(counter, mosfetActuator, lightSensor);
                            bsp_board_led_on(counter);
                
                            //Increase counter:
                            counter++;
                            break;
                        }
                        case 4 :
                        {
                            //Save struct data after and Change the mosfet state for non-detection:
                            mosfetActuator = (uint8_t) bsp_board_led_state_get(counter-1);
                            lightSensor    = ads1015Read(counter-1);
                            time           = (uint16_t) secondsGetTime();
                            NRF_LOG_INFO("lightSensor[%d]: %d", (counter-1), lightSensor);
                            detectionSystem_saveStructData_after((counter-1), mosfetActuator, lightSensor, time);
                            bsp_board_led_off(counter-1);
                
                            //Get the detection system struct data:
                            detection_system_data dsData = detectionSystem_getStructData();
                            NRF_LOG_INFO("time = %d. mosfetBefore = %d. sensorBefore = %d. mosfetAfter = %d. sensorAfter = %d", dsData.time[0], dsData.mosfetActuator_before[0], dsData.lightSensor_before[0], dsData.mosfetActuator_after[0], dsData.lightSensor_after[0]);
                            NRF_LOG_INFO("time = %d. mosfetBefore = %d. sensorBefore = %d. mosfetAfter = %d. sensorAfter = %d", dsData.time[1], dsData.mosfetActuator_before[1], dsData.lightSensor_before[1], dsData.mosfetActuator_after[1], dsData.lightSensor_after[1]);
                            NRF_LOG_INFO("time = %d. mosfetBefore = %d. sensorBefore = %d. mosfetAfter = %d. sensorAfter = %d", dsData.time[2], dsData.mosfetActuator_before[2], dsData.lightSensor_before[2], dsData.mosfetActuator_after[2], dsData.lightSensor_after[2]);
                            NRF_LOG_INFO("time = %d. mosfetBefore = %d. sensorBefore = %d. mosfetAfter = %d. sensorAfter = %d", dsData.time[3], dsData.mosfetActuator_before[3], dsData.lightSensor_before[3], dsData.mosfetActuator_after[3], dsData.lightSensor_after[3]);
                            
            
                            //(1) First:
                            qspiPushSampleInExternalFlash(dsData);
                            
                            //If the command from the phone app is for stopping the detection system task, then stop it:
                            if(!deviceStatus_getStructData_commandFromPhone()){
                                NRF_LOG_INFO("MAIN: Detection System Task Stoped.");

                                //Stop the detection system timers:
                                timerDetectionSystem_Stop();
                                secondsStop();
                                hundredMillisStop();

                                //Update the device status data (now it's not measuring):
                                deviceStatus_saveStructData_isMeasuring(false);
                                NRF_LOG_INFO("MAIN: isMeasuring = %d.", deviceStatus_getStructData_isMeasuring());

                                //Update device status and notify STAT characteristic:
                                device_status_data dataCusStat = deviceStatus_getStructData();
                                bleCusStatSendData(dataCusStat);
                                NRF_LOG_INFO("MAIN: Send notification of the STAT characteristic. commandFromPhone = %d. isMeasuring = %d.", deviceStatus_getStructData_commandFromPhone(), deviceStatus_getStructData_isMeasuring());
                                                            
                            }
            
                            //Reset counter and timer flag:
                            counter = 0;
                            timerDetectionSystem_ClearFlag();
                            break;
                        }
                    }
                
                }
            
                //If the nRF52840 is connected and the notifications are enabled, then try to send BT data every 0.1[s]:
                if(bleGetCusSensNotificationFlag())
                {
                    if(hundredMillisGetFlag())
                    {
                        hundredMillisClearFlag();
                    
                        //(2) Second:
                        qspiReadExternalFlashAndSendBleDataIfPossible();
                    }
                }
            
            }
        }
       
    }
}


/**
 * @}
 */