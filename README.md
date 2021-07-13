# rza2m-ANPR-demo
Automatic Number Plate Recognition (ANPR) with RZ/A2M
(eAI demo applying “Renesas eAI translator” and “TensorFlow Lite for MCU”)

&nbsp;![sample](https://github.com/renesas-rz/rza2m-ANPR-demo/blob/main/ANPR_eAI_translator.JPG)  



The ANPR demo detects typical German, single-row number plates and their characters from images taken by a camera.
The characters are classified with a 6 layer CNN to predict the number plate content.
The result is shown on a display (HDMI monitor).

&nbsp;![sample](https://github.com/renesas-rz/rza2m-ANPR-demo/blob/main/ANPR_demo_concept.JPG)  

Image pre-processing is accelerated with a Dynamically Reconfigurable Processor (DRP).
The CNN is implemented in 4 selectable representations:
	1. Renesas eAI translator	(32bit floating point model)
	2. TensorFlow Lite for MCU	(32bit floating point model)
	3. TensorFlow Lite for MCU	(post-training quantization model (8bit integer))
	4. TensorFlow Lite for MCU	(quantization aware training model (8bit integer))

Please check the 
&nbsp;[Application Note](https://github.com/renesas-rz/rza2m-ANPR-demo/blob/main/ANPR_RZA2M_EBK/doc/RZA2M_ANPR_demo_ApplicationNote_20210706.pdf)
for a few more details.

Please contact your local Renesas Sales representative in case you like to get more information about RZ/A2M, DRP and eAI.


The software is available for 2 different platforms:

1.	RZ/A2M Evaluation Board Kit
&nbsp;![RZ/A2M-Evaluation-Kit - RZ/A2M Evaluation Kit | Renesas](https://www.renesas.com/eu/en/products/microcontrollers-microprocessors/rz-cortex-a-mpus/rza2m-evaluation-kit-rza2m-evaluation-kit)
e²studio project name: “ANPR_RZA2M_EBK“

2.	Gadget Renesas board “GR-MANGO”		
&nbsp;![GR-MANGO | Renesas](https://www.renesas.com/eu/en/products/gadget-renesas/boards/gr-mango)
e²studio project name: “ANPR_RZA2M_GR_MANGO”



## Q&A
Please contact us below if you have any questions.　
&nbsp;[Q&A Forum](https://renesasrulz.com/rz/rz-a2m-drp/f/rz-a2m-and-drp-forum)  
