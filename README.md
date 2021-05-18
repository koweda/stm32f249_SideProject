## Environment
Windows 10 (Surface Pro 2017)
Python 3.7.3
TensorFlow 2.0
ebf_stm32f429

## Principle
1.Use RFID card switch mode
2.STM32 use USART transport data ,pc receive by pySerial
3.After the PC-side image recognizes the person, the STM32 is notified. If the STM32 does not operate in the normal mode, if it is in the alert mode, an alarm will be issued and a message will be sent back to the PC
4.When the PC receives the return, it will use the communication software (Telegram) to inform the user

## References
YoloV3 Implemented in TensorFlow 2.0
https://github.com/zzh8829/yolov3-tf2

ebf_stm32f429_tiaozhanzhe
https://github.com/Embdefire/ebf_stm32f429_tiaozhanzhe_std_code
