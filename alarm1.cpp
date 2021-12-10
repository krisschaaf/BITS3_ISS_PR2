/* ******************************************************************************
 * @file    main.cpp
 * @author  CLab
 * @version V1.0.0
 * @date    2-December-2016
 * @brief   Simple Example application for using the X_NUCLEO_IKS01A1 
 *          MEMS Inertial & Environmental Sensor Nucleo expansion board.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
*/ 

/* Includes */
#include "mbed.h"
#include "XNucleoIKS01A2.h"

/* Instantiate the expansion board */
static XNucleoIKS01A2 *mems_expansion_board = XNucleoIKS01A2::instance(D14, D15, D4, D5);

/* Retrieve the composing elements of the expansion board */
static LSM303AGRMagSensor *magnetometer = mems_expansion_board->magnetometer;
static HTS221Sensor *hum_temp = mems_expansion_board->ht_sensor;
static LPS22HBSensor *press_temp = mems_expansion_board->pt_sensor;
static LSM6DSLSensor *acc_gyro = mems_expansion_board->acc_gyro;
static LSM303AGRAccSensor *accelerometer = mems_expansion_board->accelerometer;


//returns mgValue in Degree: y = 0,09 * x 
int32_t getDegree(int32_t mgValue)	{	
	return 0.09*mgValue;
}

/* Simple main function */
int main() {
  int32_t axes[3];
	int32_t triggerVal = 6000;
	int indexAchse = -1;
	
  /* Enable all sensors */
  hum_temp->enable();
  press_temp->enable();
  magnetometer->enable();
  accelerometer->enable();
  acc_gyro->enable_x();
  acc_gyro->enable_g();
	
	while(1) {
		
	  acc_gyro->get_g_axes(axes);		
		
		for(int i = 0; i < 3; i++)	{
			if(axes[i] < 0)	{
				axes[i] = -(axes[i]);
			}
		}
		
		for(int i = 0; i < 3; i++)	{			
			
			if(axes[i] >= triggerVal)	{						
				
				if(axes[0] > axes[1] && axes[0] > axes[2])		{
						printf("in X Richtung");
						indexAchse = 0;
				}
				else if(axes[1] > axes[0] && axes[1] > axes[2])		{
						printf("in Y Richtung");
						indexAchse = 1;
				}
				else if(axes[2] > axes[1] && axes[2] > axes[0])		{
						printf("in Z Richtung");
						indexAchse = 2;
				}
				else	{
					printf("ungültige Werte ");
				}
				
				printf("\r\n");
				
				while(axes[indexAchse] >= triggerVal)	{
					acc_gyro->get_g_axes(axes);	
					
					for(int i = 0; i < 3; i++)	{
					if(axes[i] < 0)	{
						axes[i] = -(axes[i]);
						}
					}
					wait(1.00);
				}
				
				indexAchse = -1;
				
			}
		}
  }
}

	
