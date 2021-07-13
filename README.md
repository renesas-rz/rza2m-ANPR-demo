# rza2m-ANPR-demo

Automatic Number Plate Recognition (ANPR) with RZ/A2M  
(e-AI demo applying “Renesas e-AI translator” and “TensorFlow Lite for MCU”)

<p align="center"> 
	<img src="https://github.com/renesas-rz/rza2m-ANPR-demo/blob/main/ANPR_eAI_translator.JPG" alt="">
	<img src="https://github.com/renesas-rz/rza2m-ANPR-demo/blob/main/ANPR_demo_concept.JPG" alt="">
</p>

The ANPR demo detects typical German, single-row number plates and their characters from images taken by a camera.  
To predict the number plate content, the characters are classified with a 6 layer CNN.  
Finally, the result is shown on a display (HDMI monitor).


### Supported platforms:

1. RZ/A2M Evaluation Board Kit
   
   &nbsp;![RZ/A2M-Evaluation-Kit - RZ/A2M Evaluation Kit | Renesas](https://www.renesas.com/eu/en/products/microcontrollers-microprocessors/rz-cortex-a-mpus/rza2m-evaluation-kit-rza2m-evaluation-kit)
   
   e²studio project name: “ANPR_RZA2M_EBK“

2. Gadget Renesas board “GR-MANGO”
   
   &nbsp;![GR-MANGO | Renesas](https://www.renesas.com/eu/en/products/gadget-renesas/boards/gr-mango)
   
   e²studio project name: “ANPR_RZA2M_GR_MANGO”


### Key features:

Performance of 5-6fps by accelerating the image pre-processing with the Dynamically Reconfigurable Processor (DRP).

The CNN is implemented in 4 selectable representations:  
1. Renesas e-AI translator   (32bit floating point model) 
2. TensorFlow Lite for MCU   (32bit floating point model)  
3. TensorFlow Lite for MCU   (post-training quantization model (8bit integer))  
4. TensorFlow Lite for MCU   (quantization aware training model (8bit integer))  

### More:

Please check the 
[Application Note](https://github.com/renesas-rz/rza2m-ANPR-demo/blob/main/ANPR_RZA2M_EBK/doc/RZA2M_ANPR_demo_ApplicationNote_20210706.pdf)
for more details.

Please contact your local Renesas Sales representative in case you like to get more information about [RZ/A2M](https://www.renesas.com/eu/en/products/microcontrollers-microprocessors/rz-cortex-a-mpus/rza2m-high-speed-embedded-ai-based-image-processing-microprocessors-dynamically-reconfigurable-processor), [DRP](https://www.renesas.com/eu/en/application/technologies/drp) and [e-AI](https://www.renesas.com/eu/en/application/technologies/e-ai).

## Q&A
Please contact us below if you have any questions.　
&nbsp;[Q&A Forum](https://renesasrulz.com/rz/rz-a2m-drp/f/rz-a2m-and-drp-forum)  
